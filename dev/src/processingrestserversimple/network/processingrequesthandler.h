#ifndef QUERYREQUESTHANDLER_H
#define QUERYREQUESTHANDLER_H

class QDomDocument;

#include "httprequesthandler.h"
#include "errorhandler.h"

class ProcessingRequestHandler : public HttpRequestHandler, public ErrorHandler
{
    Q_OBJECT

public:
    explicit ProcessingRequestHandler(QObject *parent = nullptr);
    virtual ~ProcessingRequestHandler() Q_DECL_OVERRIDE;

    virtual void service(HttpRequest &request, HttpResponse &response) Q_DECL_OVERRIDE;

    void setRequestId(const QString &requestId);

private:
    virtual QString handlerName() const Q_DECL_OVERRIDE;
    void updateReply(QDomDocument &reply) const;

    bool prepareAnswer(const QString &fileName, HttpResponse &response);

    QString requestId;
};

#endif // QUERYREQUESTHANDLER_H
