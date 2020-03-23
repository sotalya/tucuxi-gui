#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

class HttpResponse;

#include <QString>

class ErrorHandler
{

public:
    ErrorHandler();
    virtual ~ErrorHandler();

protected:
    void notFound(HttpResponse &response) const;
    void noContent(HttpResponse &response) const;
    void badRequest(HttpResponse &response, const QString &message) const;
    void internalError(HttpResponse &response, const QString &message) const;

private:
    virtual QString handlerName() const = 0;
};

#endif // ERRORHANDLER_H
