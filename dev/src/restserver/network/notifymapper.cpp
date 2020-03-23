#include "notifymapper.h"
#include "notificationhandler.h"

#include <QDomDocument>

NotifyMapper::NotifyMapper(QObject *parent) :
    HttpRequestHandler(parent),
    ErrorHandler(),
    notificationHandler(new NotificationHandler(this))
{

}

NotifyMapper::~NotifyMapper()
{

}

void NotifyMapper::service(HttpRequest &request, HttpResponse &response)
{
    QDomDocument doc;
    if (!doc.setContent(request.getBody())) {
        badRequest(response, "Malformed request");
        return;
    }

    QString requestType = doc.documentElement().attributeNode("type").value();

    if (requestType == "notification") {
        notificationHandler->service(request, response);
        return;
    }

    badRequest(response, QString("Unknown or invalid request type '%1'.").arg(requestType));
}

QString NotifyMapper::handlerName() const
{
    return metaObject()->className();
}
