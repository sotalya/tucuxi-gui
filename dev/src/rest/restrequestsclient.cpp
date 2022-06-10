//@@license@@

#include "restrequestsclient.h"

#include "core/core.h"
#include "core/corefactory.h"
#include "core/corerepository.h"

#include "core/dal/drugtreatment.h"
#include "core/dal/drugresponseanalysis.h"

#include "apputils/src/appcore.h"
#include "apputils/src/drugmanager.h"
#include "apputils/src/apputilsfactory.h"
#include "apputils/src/apputilsrepository.h"

#include "admin/src/adminfactory.h"
#include "admin/src/stdadminrepository.h"
#include "admin/src/dal/person.h"
#include "admin/src/dal/partialrequest.h"
#include "admin/src/dal/partialrequest.h"
#include "admin/src/dal/interpretationrequest.h"

#include "rest/builders/messagecontent.h"
#include "rest/builders/querylistmessagebuilder.h"
#include "rest/builders/queryrequestmessagebuilder.h"
#include "rest/builders/replylistmessagebuilder.h"
#include "rest/builders/replyrequestmessagebuilder.h"
#include "rest/builders/acklistmessagebuilder.h"
#include "rest/builders/ackrequestmessagebuilder.h"
#include "rest/model/querylistxmlmessage.h"
#include "rest/model/replylistxmlmessage.h"
#include "rest/model/queryrequestxmlmessage.h"
#include "rest/model/replyrequestxmlmessage.h"
#include "rest/network/networkaccessmanager.h"
#include "rest/network/querylistrequest.h"
#include "rest/network/queryrequestrequest.h"
#include "rest/network/acklistrequest.h"
#include "rest/network/ackrequestrequest.h"
#include "rest/builders/interpretationrequestbuilder.h"

#include "rest/builders/drugidtranslator.h"

#include <QMessageBox>
#include <QNetworkReply>
#include <QDateTime>
#include <QTextStream>
#include <QTextCodec>

#include "cli/rlutil.h"
#include "restlogger.h"

using namespace Tucuxi::GuiCore;

RestRequestsClient::RestRequestsClient(QObject *parent) :
    RequestsClientProcessing(parent),
    _queryListBuilder(new QueryListMessageBuilder(this)),
    _queryRequestBuilder(new QueryRequestMessageBuilder(this)),
    _replyListBuilder(new ReplyListMessageBuilder(this)),
    _replyRequestBuilder(new ReplyRequestMessageBuilder(this)),
    _ackListBuilder(new AckListMessageBuilder(this)),
    _ackRequestBuilder(new AckRequestMessageBuilder(this)),
    _queryList(new QueryListRequest),
    _queryRequest(new QueryRequestRequest),
    _ackList(new AckListRequest),
    _ackRequest(new AckRequestRequest)
{

    _restLogger = RestLogger::getInstance();
}

RestRequestsClient::~RestRequestsClient()
{

}


void RestRequestsClient::queryList(QDateTime from,
                                       QDateTime to,
                                       bool state)
{

    connect(&NetworkAccessManager::getInstance(), SIGNAL(finished(QNetworkReply*)), this, SLOT(processQueryList(QNetworkReply*)));

    MessageContent content;
    content.setValue("from", from.toString(Qt::ISODate));
    content.setValue("to", to.toString(Qt::ISODate));
    content.setValue("state", state? "open" : "closed");

    _queryListBuilder->setMessageContent(content);
    _queryList->setMessage(_queryListBuilder->message());

    EXLOG(QtDebugMsg, NOEZERROR, tr("built the query_list"));
    NetworkAccessManager::getInstance().post(*_queryList.data(), _queryList->postData());
    EXLOG(QtDebugMsg, NOEZERROR, tr("Sent query_list request with from: %1, to: %2, state: %3").arg(from.toString()).arg(to.toString()).arg(state));
    //print out the headers and the xml payload
    QString headers = "";
    foreach (QByteArray hdr, _queryList.data()->rawHeaderList()) {
        headers = headers + " " + QString(hdr);
    }
    for(int i = 0; i < 8; ++i) {
        headers = headers + " " + _queryList.data()->header(QNetworkRequest::KnownHeaders(i)).toString();
    }

    EXLOG(QtDebugMsg, NOEZERROR, tr("Request headers: %1").arg(headers));
    EXLOG(QtDebugMsg, NOEZERROR, tr("Request post data: %1").arg(QString(_queryList.data()->postData())));
}


void RestRequestsClient::queryRequest(const QString &requestId, const QString &patientId, const QString &drugId)
{
    MessageContent content;
    content.setValue("request.id", requestId);
    content.setValue("patient.id", patientId);
    content.setValue("drug.id", drugId);
    _queryRequestBuilder->setMessageContent(content);
    _queryRequest->setMessage(_queryRequestBuilder->message());

    EXLOG(QtDebugMsg, NOEZERROR, tr("Posting query_request"));
    NetworkAccessManager::getInstance().post(*_queryRequest.data(), _queryRequest->postData());
    EXLOG(QtDebugMsg, NOEZERROR, tr("Sent query Request with requestId: %1, patientId: %2, drugId: %3").arg(requestId).arg(patientId).arg(drugId));

    QTextStream informer(stdout);
    rlutil::setColor(rlutil::CYAN);
    informer <<  "Sent query Request" << endl;
    informer <<  "******************" << endl;
    rlutil::setColor(rlutil::GREEN);
    informer << "RequestId:\t"; informer.flush();
    rlutil::resetColor();
    informer << content.value("request.id"); informer.flush();
    rlutil::setColor(rlutil::GREEN);
    informer << "\tPatientId:\t"; informer.flush();
    rlutil::resetColor();
    informer << content.value("patient.id"); informer.flush();
    rlutil::setColor(rlutil::GREEN);
    informer << "\tDrugId:\t"; informer.flush();
    rlutil::resetColor();
    informer << content.value("drug.drugId");
    rlutil::resetColor();
    informer << endl;

    connect(&NetworkAccessManager::getInstance(), SIGNAL(finished(QNetworkReply*)), this, SLOT(processQueryRequest(QNetworkReply*)));
}


void RestRequestsClient::ackList(const QString &replyControlId, const QString &ackCode, const QString &ackMessage)
{
    //Send asynchronously

    MessageContent content;
    content.setValue("replyControlId", replyControlId);
    content.setValue("ackCode", ackCode);
    if (!ackMessage.isEmpty())
        content.setValue("ackMessage", ackMessage);

    _ackListBuilder->setMessageContent(content);
    _ackList->setMessage(_ackListBuilder->message());

    NetworkAccessManager::getInstance().post(*_ackList.data(), _ackList->postData());
    EXLOG(QtDebugMsg, NOEZERROR, tr("Sent ack List with controlId: %1, ackCode: %2, ackMessage: %3").arg(replyControlId).arg(ackCode).arg(ackMessage));
    connect(&NetworkAccessManager::getInstance(), SIGNAL(finished(QNetworkReply*)), this, SLOT(processAckList(QNetworkReply*)));
}


void RestRequestsClient::ackRequest(const QString &replyControlId, const QString &ackCode, const QString &ackMessage)
{
    //Send asynchronously
    MessageContent content;
    content.setValue("replyControlId", replyControlId);
    content.setValue("ackCode", ackCode);
    if (!ackMessage.isEmpty())
        content.setValue("ackMessage", ackMessage);

    _ackRequestBuilder->setMessageContent(content);
    _ackRequest->setMessage(_ackRequestBuilder->message());

    NetworkAccessManager::getInstance().post(*_ackRequest.data(), _ackRequest->postData());
    EXLOG(QtDebugMsg, NOEZERROR, tr("Sent ack Request with controlId: %1, ackCode: %2, ackMessage: %3").arg(replyControlId).arg(ackCode).arg(ackMessage));
    connect(&NetworkAccessManager::getInstance(), SIGNAL(finished(QNetworkReply*)), this, SLOT(processAckRequest(QNetworkReply*)));
}

void RestRequestsClient::processQueryList(QNetworkReply *reply)
{
    disconnect(&NetworkAccessManager::getInstance(), SIGNAL(finished(QNetworkReply*)), this, SLOT(processQueryList(QNetworkReply*)));

    EXLOG(QtDebugMsg, NOEZERROR, tr("Processing network reply..."));
    //print out the headers and the xml payload
    QString headers = "";
    foreach (QByteArray hdr, reply->rawHeaderList()) {
        headers = headers + " " + QString(hdr);
    }
    for(int i = 0; i < 8; ++i) {
        headers = headers + " " +reply->header(QNetworkRequest::KnownHeaders(i)).toString();
    }

    EXLOG(QtDebugMsg, NOEZERROR, tr("Response headers: %1").arg(headers));
    QString response = QString(reply->readAll());

    EXLOG(QtDebugMsg, NOEZERROR, tr("Response post data: %1").arg(response));

    if (reply->error() > 0) {
        EXLOG(QtWarningMsg, NOEZERROR, tr("An error occured while retreiving the requests list (%1)").arg(reply->errorString()));
        reply->deleteLater();
        return;
    }

    QString controlId;

    // The following method is really where everything happens
    analyzeList(response, controlId);

    reply->deleteLater();

    emit queryListFinished();

    ackList(controlId, "AA");
}



void RestRequestsClient::processQueryRequest(QNetworkReply *reply)
{
//    EXLOG(QtCriticalMsg, NOEZERROR, tr("Received query list reply: %1").arg(QString(reply->readAll())));
    disconnect(&NetworkAccessManager::getInstance(), SIGNAL(finished(QNetworkReply*)), this, SLOT(processQueryRequest(QNetworkReply*)));

    EXLOG(QtDebugMsg, NOEZERROR, tr("Processing network reply..."));
    //print out the headers and the xml payload
    QString headers = "";
    foreach (QByteArray hdr, reply->rawHeaderList()) {
        headers = headers + " " + QString(hdr);
    }
    for(int i = 0; i < 8; ++i) {
        headers = headers + " " +reply->header(QNetworkRequest::KnownHeaders(i)).toString();
    }

    EXLOG(QtDebugMsg, NOEZERROR, tr("Response headers: %1").arg(headers));
    QString response = QString(reply->readAll());
    EXLOG(QtDebugMsg, NOEZERROR, tr("Response post data: %1").arg(response));

    if (reply->error() > 0) {
        EXLOG(QtWarningMsg, NOEZERROR, tr("An error occured while retreiving the requests list (%1)").arg(reply->errorString()));
        reply->deleteLater();
        return;
    }
    if (reply->error() > 0) {
//        QMessageBox::critical(0, tr("Network error"), tr("An error occured while retreiving the selected request (%1)").arg(reply->errorString()));
        EXLOG(QtCriticalMsg, NOEZERROR, tr("The content of the requests list is invalid (%1)").arg(reply->errorString()));
        reply->deleteLater();
        return;
    }

    ReplyRequestXmlMessage replyRequestMessage;
    replyRequestMessage.setData(response.toUtf8());

    if (!replyRequestMessage.isValid()) {
//        QMessageBox::critical(0, tr("Reply error"), tr("The content of the request is invalid (%1)").arg(replyRequestMessage.errorString()));
        EXLOG(QtCriticalMsg, NOEZERROR, tr("The content of the reply request is invalid (%1)").arg(replyRequestMessage.errorString()));
        reply->deleteLater();


        /********************************************
         * TODO: This is required by the CLI application
         * Not well designed
         *******************************************/
        if (CORE->runningMode() == Tucuxi::GuiCore::Core::CLI)
            emit ackRequestFinished();

        return;
    }

    _restLogger->logRequest(response.toUtf8());

    _replyRequestBuilder->setMessage(&replyRequestMessage);
    MessageContent content = _replyRequestBuilder->messageContent();

    // The following method is really where everything happens
    analyzeRequest(response);

    //ToDo: update code below according to the result above
    ackRequest(content.value("controlId"), "AA");
    reply->deleteLater();

    emit queryRequestFinished();
}

void RestRequestsClient::processAckList(QNetworkReply *reply)
{
    disconnect(&NetworkAccessManager::getInstance(), SIGNAL(finished(QNetworkReply*)), this, SLOT(processAckList(QNetworkReply*)));

    if (reply->error() > 0) {
//        QMessageBox::warning(0, tr("Network error"), tr("An error occured while sending the list acknowledgment (%1)").arg(reply->errorString()));
        EXLOG(QtCriticalMsg, NOEZERROR, tr("An error occured while sending the list acknowledgment (%1)").arg(reply->errorString()));
    }

    reply->deleteLater();
    emit ackListFinished();
}



void RestRequestsClient::processAckRequest(QNetworkReply *reply)
{
    disconnect(&NetworkAccessManager::getInstance(), SIGNAL(finished(QNetworkReply*)), this, SLOT(processAckRequest(QNetworkReply*)));
    EXLOG(QtDebugMsg, NOEZERROR, tr("Processing Ack Request..."));

    if (reply->error() > 0)
//        QMessageBox::warning(0, tr("Network error"), tr("An error occured while sending the request acknowledgment (%1)").arg(reply->errorString()));
        EXLOG(QtCriticalMsg, NOEZERROR, tr("An error occured while sending the request acknowledgment (%1)").arg(reply->errorString()));
    reply->deleteLater();
    emit ackRequestFinished();
    EXLOG(QtDebugMsg, NOEZERROR, tr("Processed Ack Request..."));
}
