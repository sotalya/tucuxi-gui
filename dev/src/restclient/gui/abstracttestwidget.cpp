//@@license@@

#include "abstracttestwidget.h"

#include "rest/network/networkaccessmanager.h"
#include "rest/network/mirthrequest.h"
#include "rest/model/message.h"
#include "core/utils/logging.h"
#include "errors_restclient.h"

#include <QNetworkReply>
#include <QPushButton>
#include <QProgressBar>
#include <QPlainTextEdit>

AbstractTestWidget::AbstractTestWidget(MirthRequest *request, QWidget *parent) :
    QWidget(parent),
    _request(request),
    _reply(),
    _ackReq(),
    _ackMsg(0)
{
    connect(this, SIGNAL(errors(QString)), this, SLOT(displayError(QString)));
    connect(this, SIGNAL(stopped(AbstractTestWidget*)), this, SLOT(displaySuccess()));
}

AbstractTestWidget::~AbstractTestWidget()
{

}

void AbstractTestWidget::enable()
{
    pushButton()->setEnabled(true);
}

void AbstractTestWidget::disable()
{
    pushButton()->setEnabled(false);
}

void AbstractTestWidget::reset()
{
    progressBar()->setValue(0);
    textArea()->clear();
}

void AbstractTestWidget::setQueryMessage(Message *message)
{
    _request->setMessage(message);
}

void AbstractTestWidget::setReplyMessage(Message *message)
{
    _reply.reset(message);
}

void AbstractTestWidget::setButtonText(const QString &text)
{
    pushButton()->setText(text);
}

QString AbstractTestWidget::buttonText()
{
    return pushButton()->text();
}

void AbstractTestWidget::setAcknowledgement(MirthRequest *request, Message *message)
{
    Q_ASSERT(request);
    Q_ASSERT(message);

    _ackReq.reset(request);
    _ackMsg = message;

    request->setMessage(message);
}

QDomDocument AbstractTestWidget::buildAck(const QString &controlId, const QString &ackType, const QString &ackCode, const QString &ackMessage)
{
    QDomDocument doc;

    QDomElement rootNode = doc.createElement("data");
    doc.appendChild(rootNode);

    QDomAttr versionAttr = doc.createAttribute("version");
    versionAttr.setNodeValue("0.2");
    rootNode.setAttributeNode(versionAttr);

    QDomAttr langAttr = doc.createAttribute("lang");
    langAttr.setNodeValue("en");
    rootNode.setAttributeNode(langAttr);

    QDomAttr typeAttr = doc.createAttribute("type");
    typeAttr.setNodeValue(ackType);
    rootNode.setAttributeNode(typeAttr);

    QDomAttr controlIdAttr = doc.createAttribute("controlId");
    controlIdAttr.setNodeValue("66");
    rootNode.setAttributeNode(controlIdAttr);

    QDomAttr xsiAttr = doc.createAttribute("xmlns:xsi");
    xsiAttr.setNodeValue("http://www.w3.org/2001/XMLSchema-instance");
    rootNode.setAttributeNode(xsiAttr);

    QDomAttr nnslAttr = doc.createAttribute("xsi:noNamespaceSchemaLocation");
    nnslAttr.setNodeValue("eep.xsd");
    rootNode.setAttributeNode(nnslAttr);

    QDomElement replyControlIdNode = doc.createElement("replyControlId");
    rootNode.appendChild(replyControlIdNode);

    QDomText replyControlIdText = doc.createTextNode(controlId);
    replyControlIdNode.appendChild(replyControlIdText);

    QDomElement ackCodeNode = doc.createElement("acknowledgmentCode");
    rootNode.appendChild(ackCodeNode);

    QDomText ackCodeText = doc.createTextNode(ackCode);
    ackCodeNode.appendChild(ackCodeText);

    QDomElement ackMessageNode = doc.createElement("acknowledgmentMessage");
    rootNode.appendChild(ackMessageNode);

    QDomText ackMessageText = doc.createTextNode(ackMessage);
    ackMessageNode.appendChild(ackMessageText);

    return doc;
}

void AbstractTestWidget::sendAck(const QDomDocument &reply, const QString &ackType, const QString &ackCode, const QString &ackMessage)
{
    Q_ASSERT(!_ackReq.isNull());

    QString controlId = reply.documentElement().attributeNode("controlId").value();
    Q_ASSERT(!controlId.isEmpty());

    _ackMsg->setData(buildAck(controlId, ackType, ackCode, ackMessage).toByteArray());
    if (!_ackMsg->isValid()) {
        displayError(_ackMsg->errorString());
        return;
    }

    NetworkAccessManager::getInstance().post(*_ackReq.data(), _ackReq->postData());
    connect(&NetworkAccessManager::getInstance(), SIGNAL(finished(QNetworkReply *)), this, SLOT(ackFinished(QNetworkReply*)));
}

void AbstractTestWidget::displayError(const QString &errorMsg)
{
    textArea()->setPlainText(errorMsg);
}

void AbstractTestWidget::displaySuccess()
{
    Q_ASSERT(!_reply.isNull());
    textArea()->setPlainText(_reply->isValid() ? tr("SUCCESS: The message is valid!") : tr("FAILURE: ") + _reply->errorString());
}

void AbstractTestWidget::start()
{
    Q_ASSERT(!_request.isNull());

    disable();

    QNetworkReply *reply = NetworkAccessManager::getInstance().post(*_request.data(), _request->postData());
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(setProgress(qint64,qint64)));
    connect(&NetworkAccessManager::getInstance(), SIGNAL(finished(QNetworkReply *)), this, SLOT(requestFinished(QNetworkReply *)));

    emit started(this);
}

void AbstractTestWidget::setProgress(qint64 received, qint64 total)
{
    if (total == 0) {
        progressBar()->setValue(100);
        return;
    }

    progressBar()->setValue(qreal(received) / total * 100);
}

void AbstractTestWidget::requestFinished(QNetworkReply *reply)
{
    Q_ASSERT(!_reply.isNull());

    enable();

    disconnect(&NetworkAccessManager::getInstance(), SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));

    if (reply->error() > 0) {
        emit errors(reply->errorString());
        reply->deleteLater();
        return;
    }

    QString response = QString(reply->readAll());

    /*
    int size = reply->bytesAvailable();
    char *data = new char(size);
    if (size > 0) {
        if (reply->read(data, size-1) == -1) {
            EXLOG(QtFatalMsg, Tucuxi::restclient::NOEZERROR, tr("QIODevice read failure"));
        }
    }
    */

    QFile file("out.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    out << response.toUtf8();
//    out << data;
    file.close();
    EXLOG(QtDebugMsg, Tucuxi::restclient::NOEZERROR, tr("response: %1").arg(response));
//    _reply->setData(reply->readAll());
    _reply->setData(response.toUtf8());
//    _reply->setData(QByteArray::fromRawData(data, size-1));
    reply->deleteLater();

    emit stopped(this);
}

void AbstractTestWidget::ackFinished(QNetworkReply *reply)
{
    disconnect(&NetworkAccessManager::getInstance(), SIGNAL(finished(QNetworkReply*)), this, SLOT(ackFinished(QNetworkReply*)));

    if (reply->error() > 0)
        displayError(reply->errorString());

    reply->deleteLater();
}
