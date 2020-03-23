#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QString>
#include <QVariant>
#include <QDomDocument>

#include "processingrequesthandler.h"
#include "processingrestserversimple/utils/settings.h"

#include "core/utils/xmlvalidator.h"

#include "rest/model/queryrequestxmlmessage.h"
#include "rest/utils/controlid.h"


ProcessingRequestHandler::ProcessingRequestHandler(QObject *parent) :
    HttpRequestHandler(parent),
    ErrorHandler()
{

}

ProcessingRequestHandler::~ProcessingRequestHandler()
{

}

void ProcessingRequestHandler::setRequestId(const QString &requestId)
{
    this->requestId = requestId;
}

#include <iostream>

void ProcessingRequestHandler::service(HttpRequest &request, HttpResponse &response)
{
    QDomDocument requestXml;
    if (!requestXml.setContent(request.getBody())) {
        internalError(response, "Unkown conversion error");
        return;
    }

    QString requestId = requestXml.documentElement().firstChildElement("requestId").firstChild().toText().data();

    QFileInfo fi(Settings::getInstance().get("source").toString());
    QDir dir = fi.dir();

    QStringList reqFiles;
    reqFiles << QString("Req_%1.xml").arg(requestId);
    reqFiles << "Req.xml";

    foreach (QString fileName, reqFiles) {
        QFile reqFile(fileName);
        // Answer the first valid file
        if (prepareAnswer(fileName, response))
            return;
    }
}

bool ProcessingRequestHandler::prepareAnswer(const QString &fileName, HttpResponse &response)
{
    QFile reqFile(fileName);
    QDomDocument doc;
    if (!doc.setContent(&reqFile)) {
        std::cout << "Can not get reply file content" << std::endl;
        return false;
    }

    QString requestId = doc.documentElement().firstChildElement("dataset").firstChildElement("requestId").firstChild().toText().data();
    std::cout << "Matched Request id : " << qPrintable(requestId) << std::endl;
    // Got it, thats the right file
    reqFile.seek(0);
    QByteArray content = reqFile.readAll();
    response.setHeader("Content-Type", "text/xml");
    // Without the following header field, Mozilla XMLHttpRequest client on windows does not work
    response.setHeader("Access-Control-Allow-Origin", "*");
    response.write(content, true);
    return true;

}


QString ProcessingRequestHandler::handlerName() const
{
    return metaObject()->className();
}

void ProcessingRequestHandler::updateReply(QDomDocument &/*reply*/) const
{
    //reply.documentElement().attributeNode("controlId").setNodeValue(QString::number(ControlId::getInstance().next()));
}
