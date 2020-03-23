#ifndef QUERYREQUESTHANDLER_H
#define QUERYREQUESTHANDLER_H

class QDomDocument;

#include "httprequesthandler.h"
#include "errorhandler.h"

class QueryRequestHandler : public HttpRequestHandler, public ErrorHandler
{
    Q_OBJECT

public:
    explicit QueryRequestHandler(QObject *parent = nullptr);
    virtual ~QueryRequestHandler() Q_DECL_OVERRIDE;

    virtual void service(HttpRequest &request, HttpResponse &response) Q_DECL_OVERRIDE;

    void setRequestId(const QString &requestId);

private:
    virtual QString handlerName() const Q_DECL_OVERRIDE;
    void updateReply(QDomDocument &reply) const;

    QString requestId;
};

#endif // QUERYREQUESTHANDLER_H
