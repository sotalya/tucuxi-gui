#ifndef NOTIFICATIONHANDLER_H
#define NOTIFICATIONHANDLER_H

#include "httprequesthandler.h"
#include "errorhandler.h"

#include <QDomDocument>

class NotificationHandler : public HttpRequestHandler, public ErrorHandler
{
    Q_OBJECT

public:
    explicit NotificationHandler(QObject *parent = nullptr);
    virtual ~NotificationHandler() Q_DECL_OVERRIDE;

    virtual void service(HttpRequest &request, HttpResponse &response) Q_DECL_OVERRIDE;

private:
    virtual QString handlerName() const Q_DECL_OVERRIDE;
    QDomDocument buildAck(const QDomDocument &source) const;
};

#endif // NOTIFICATIONHANDLER_H
