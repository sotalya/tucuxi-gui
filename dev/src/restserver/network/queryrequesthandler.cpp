
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QString>
#include <QVariant>
#include <QDomDocument>

#include "queryrequesthandler.h"
#include "restserver/utils/settings.h"

#include "core/utils/xmlvalidator.h"

#include "rest/model/queryrequestxmlmessage.h"
#include "rest/utils/controlid.h"

QueryRequestHandler::QueryRequestHandler(QObject *parent) :
    HttpRequestHandler(parent),
    ErrorHandler()
{

}

QueryRequestHandler::~QueryRequestHandler()
{

}

void QueryRequestHandler::setRequestId(const QString &requestId)
{
    this->requestId = requestId;
}

#include <iostream>

void QueryRequestHandler::service(HttpRequest &request, HttpResponse &response)
{
    QueryRequestXmlMessage message;
    message.setData(request.getBody());
    if (!message.isValid()) {
        badRequest(response, "Malformed request");
        return;
    }

    QFile source(Settings::getInstance().get("source").toString());
    if (!source.open(QIODevice::ReadOnly | QIODevice::Text)) {
        internalError(response, "Missing source");
        return;
    }


#ifdef ARTIFICIAL_TEST
    Tucuxi::Gui::Core::XmlValidator validator;
    if (!validator.validate(&source, Tucuxi::Gui::Core::XmlValidator::Reply_Request)) {
        internalError(response, QString("Malformed source (%1)").arg(validator.errorMessage()));
        return;
    }

    source.seek(0);
    QDomDocument reply_request;
    if (!reply_request.setContent(&source)) {
        internalError(response, "Unkown convertion error");
        return;
    }

    updateReply(reply_request);
    if (!validator.validate(reply_request.toByteArray(), Tucuxi::Gui::Core::XmlValidator::Reply_Request)) {
        internalError(response,QString("Malformed response (%1)").arg(validator.errorMessage()));
        return;
    }

    response.setHeader("Content-Type", "text/xml");
    response.write(reply_request.toByteArray(2), true);

#else

    // We look for the request file in the same directory as the list of requests.
    // We look for the file with the same request ID as the one asked by the client
    // and send the raw file data.

    QFileInfo fi(Settings::getInstance().get("source").toString());
    QDir dir = fi.dir();

    QStringList filters;
    filters << "Req_*.xml";
    dir.setNameFilters(filters);

    QStringList reqFiles = dir.entryList();

    foreach (QString fileName, reqFiles) {
        QFile reqFile(dir.absoluteFilePath(fileName));
        QDomDocument doc;
        if (!doc.setContent(&reqFile)) {
            return;}

            QString requestId = doc.documentElement().firstChildElement("dataset").firstChildElement("requestId").firstChild().toText().data();
            if (requestId.compare(this->requestId) == 0) {
                std::cout << "Matched Request id : " << qPrintable(requestId) << std::endl;
                // Got it, thats the right file
                reqFile.seek(0);
                QByteArray content = reqFile.readAll();
                response.setHeader("Content-Type", "text/xml");
                response.write(content, true);
                return;
            }
    }

#endif // ARTIFICIAL_TEST
}

QString QueryRequestHandler::handlerName() const
{
    return metaObject()->className();
}

void QueryRequestHandler::updateReply(QDomDocument &reply) const
{
    reply.documentElement().attributeNode("controlId").setNodeValue(QString::number(ControlId::getInstance().next()));
}
