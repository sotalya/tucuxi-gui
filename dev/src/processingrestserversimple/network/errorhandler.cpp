//@@license@@

#include "errorhandler.h"
#include "httpresponse.h"

ErrorHandler::ErrorHandler()
{

}

ErrorHandler::~ErrorHandler()
{

}

void ErrorHandler::notFound(HttpResponse &response) const
{
    response.setStatus(404, "Not found");
    response.write("No such document.", true);
    qDebug().nospace().noquote() << handlerName() << ": invalid path";
}

void ErrorHandler::noContent(HttpResponse &response) const
{
    response.setStatus(204, "No content");
    qDebug().nospace().noquote() << handlerName() << ": no request available";
}

void ErrorHandler::badRequest(HttpResponse &response, const QString &message) const
{
    response.setStatus(400, "Bad request");
    response.write((message + ".").toUtf8(), true);
    qDebug().nospace().noquote() << handlerName() << ": " << message.toLower();
}

void ErrorHandler::internalError(HttpResponse &response, const QString &message) const
{
    response.setStatus(500, "Internal server error");
    response.write((message + ".").toUtf8(), true);
    qDebug().nospace().noquote() << handlerName() << ": " << message.toLower();
}
