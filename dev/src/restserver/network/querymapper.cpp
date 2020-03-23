#include "querymapper.h"
#include "querylisthandler.h"
#include "queryrequesthandler.h"
#include "acklisthandler.h"
#include "ackrequesthandler.h"

#include <QDomDocument>

QueryMapper::QueryMapper(QObject *parent) :
    HttpRequestHandler(parent),
    ErrorHandler(),
    queryListHandler(new QueryListHandler(this)),
    queryRequestHandler(new QueryRequestHandler(this)),
    ackListHandler(new AckListHandler(this)),
    ackRequestHandler(new AckRequestHandler(this))
{

}

QueryMapper::~QueryMapper()
{

}


void QueryMapper::service(HttpRequest &request, HttpResponse &response)
{
    QDomDocument doc;
    if (!doc.setContent(request.getBody())) {
        badRequest(response, "Malformed request");
        return;
    }

    QString requestType = doc.documentElement().attributeNode("type").value();

    if (requestType == "query_list") {
        queryListHandler->service(request, response);
        return;
    }
    if (requestType == "ack_list") {
        ackListHandler->service(request, response);
        return;
    }
    if (requestType == "query_request") {
#ifndef ARTIFICIAL_TEST
        QString requestId = doc.documentElement().firstChildElement("requestId").firstChild().toText().data();
        queryRequestHandler->setRequestId(requestId);
#endif // ARTIFICIAL_TEST
        queryRequestHandler->service(request, response);
        return;
    }
    if (requestType == "ack_request") {
        ackRequestHandler->service(request, response);
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
