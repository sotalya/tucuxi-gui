#include "querymapper.h"
#include "processingrequesthandler.h"

#include <QDomDocument>

QueryMapper::QueryMapper(QObject *parent) :
    HttpRequestHandler(parent),
    ErrorHandler(),
    processingRequestHandler(new ProcessingRequestHandler(this))
{

}

QueryMapper::~QueryMapper()
{

}


void QueryMapper::service(HttpRequest &request, HttpResponse &response)
{
    QDomDocument doc;
    QString body = request.getBody();
    if (!doc.setContent(request.getBody())) {
        badRequest(response, "Malformed request");
        return;
    }

    QString requestType = doc.documentElement().attributeNode("type").value();

    if (requestType == "processing_request") {
#ifndef ARTIFICIAL_TEST
        QString requestId = doc.documentElement().firstChildElement("requestId").firstChild().toText().data();
        processingRequestHandler->setRequestId(requestId);
#endif // ARTIFICIAL_TEST
        processingRequestHandler->service(request, response);
        return;
    }

    if (requestType.isEmpty())
        requestType = "<empty>";

    badRequest(response, QString("Unknown or invalid request type '%1'.").arg(requestType));
}

QString QueryMapper::handlerName() const
{
    return metaObject()->className();
}
