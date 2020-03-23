#ifndef REQUESTMAPPER_H
#define REQUESTMAPPER_H

class QueryMapper;
class NotifyMapper;

#include "httprequesthandler.h"
#include "errorhandler.h"

class RequestMapper : public HttpRequestHandler, public ErrorHandler
{
    Q_OBJECT

public:
    explicit RequestMapper(QObject *parent = nullptr);
    virtual ~RequestMapper() Q_DECL_OVERRIDE;

    virtual void service(HttpRequest &request, HttpResponse &response) Q_DECL_OVERRIDE;

private:
    virtual QString handlerName() const Q_DECL_OVERRIDE;

    QueryMapper *queryMapper;
    NotifyMapper *notifyMapper;
};

#endif // REQUESTMAPPER_H
