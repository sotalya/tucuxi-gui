#ifndef ACKREQUESTHANDLER_H
#define ACKREQUESTHANDLER_H

#include "httprequesthandler.h"
#include "errorhandler.h"

class AckRequestHandler : public HttpRequestHandler, public ErrorHandler
{
    Q_OBJECT

public:
    explicit AckRequestHandler(QObject *parent = nullptr);
    virtual ~AckRequestHandler() Q_DECL_OVERRIDE;

    virtual void service(HttpRequest &request, HttpResponse &response) Q_DECL_OVERRIDE;

private:
    virtual QString handlerName() const Q_DECL_OVERRIDE;
};

#endif // ACKREQUESTHANDLER_H
