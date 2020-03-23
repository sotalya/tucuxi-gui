#include "requestmapper.h"
#include "querymapper.h"
#include "notifymapper.h"

RequestMapper::RequestMapper(QObject *parent) :
    HttpRequestHandler(parent),
    ErrorHandler(),
    queryMapper(new QueryMapper(this)),
    notifyMapper(new NotifyMapper(this))
{

}

RequestMapper::~RequestMapper()
{

}

void RequestMapper::service(HttpRequest &request, HttpResponse &response)
{
    QByteArray path = request.getPath();
    qDebug().nospace().noquote() << "RequestMapper: path='" << path.data() << "'";

    if (path == "/services/query") {
        queryMapper->service(request, response);
        return;
    }
    if (path == "/services/notify") {
        notifyMapper->service(request, response);
        return;
    }

    notFound(response);
}

QString RequestMapper::handlerName() const
{
    return metaObject()->className();
}
