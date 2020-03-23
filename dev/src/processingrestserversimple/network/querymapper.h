#ifndef QUERYMAPPER_H
#define QUERYMAPPER_H

class QueryListHandler;
class ProcessingRequestHandler;
class AckListHandler;
class AckRequestHandler;

#include "httprequesthandler.h"
#include "errorhandler.h"

class QueryMapper : public HttpRequestHandler, public ErrorHandler
{
    Q_OBJECT

public:
    explicit QueryMapper(QObject *parent = nullptr);
    virtual ~QueryMapper() Q_DECL_OVERRIDE;

    virtual void service(HttpRequest &request, HttpResponse &response) Q_DECL_OVERRIDE;

private:
    virtual QString handlerName() const Q_DECL_OVERRIDE;

    ProcessingRequestHandler *processingRequestHandler;
};

#endif // QUERYMAPPER_H
