#ifndef NOTIFYMAPPER_H
#define NOTIFYMAPPER_H

class NotificationHandler;

#include "httprequesthandler.h"
#include "errorhandler.h"

class NotifyMapper : public HttpRequestHandler, public ErrorHandler
{
    Q_OBJECT

public:
    explicit NotifyMapper(QObject *parent = nullptr);
    virtual ~NotifyMapper() Q_DECL_OVERRIDE;

    virtual void service(HttpRequest &request, HttpResponse &response) Q_DECL_OVERRIDE;

private:
    virtual QString handlerName() const Q_DECL_OVERRIDE;

    NotificationHandler *notificationHandler;
};

#endif // NOTIFYMAPPER_H
