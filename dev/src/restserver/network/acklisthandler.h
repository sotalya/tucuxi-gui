#ifndef ACKLISTHANDLER_H
#define ACKLISTHANDLER_H

#include "httprequesthandler.h"
#include "errorhandler.h"

class AckListHandler : public HttpRequestHandler, public ErrorHandler
{
    Q_OBJECT

public:
    explicit AckListHandler(QObject *parent = nullptr);
    virtual ~AckListHandler() Q_DECL_OVERRIDE;

    virtual void service(HttpRequest &request, HttpResponse &response) Q_DECL_OVERRIDE;

private:
    virtual QString handlerName() const Q_DECL_OVERRIDE;
};

#endif // ACKLISTHANDLER_H
