#include "acklisthandler.h"

AckListHandler::AckListHandler(QObject *parent) :
    HttpRequestHandler(parent),
    ErrorHandler()
{

}

AckListHandler::~AckListHandler()
{

}

void AckListHandler::service(HttpRequest &request, HttpResponse &response)
{
    Q_UNUSED(request);
    Q_UNUSED(response);
}

QString AckListHandler::handlerName() const
{
    return metaObject()->className();
}
