#include "ackrequesthandler.h"

AckRequestHandler::AckRequestHandler(QObject *parent) :
    HttpRequestHandler(parent),
    ErrorHandler()
{

}

AckRequestHandler::~AckRequestHandler()
{

}

void AckRequestHandler::service(HttpRequest &request, HttpResponse &response)
{
    Q_UNUSED(request);
    Q_UNUSED(response);
}

QString AckRequestHandler::handlerName() const
{
    return metaObject()->className();
}
