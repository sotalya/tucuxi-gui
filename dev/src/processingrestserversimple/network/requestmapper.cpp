//@@license@@

#include "requestmapper.h"
#include "querymapper.h"

RequestMapper::RequestMapper(QObject *parent) :
    HttpRequestHandler(parent),
    ErrorHandler(),
    queryMapper(new QueryMapper(this))
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

    notFound(response);
}

QString RequestMapper::handlerName() const
{
    return metaObject()->className();
}
