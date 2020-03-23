#ifndef QUERYLISTHANDLER_H
#define QUERYLISTHANDLER_H

#include "httprequesthandler.h"
#include "errorhandler.h"

#include <QDomDocument>

class QueryListHandler : public HttpRequestHandler, public ErrorHandler
{
    Q_OBJECT

public:
    explicit QueryListHandler(QObject *parent = nullptr);
    virtual ~QueryListHandler() Q_DECL_OVERRIDE;

    virtual void service(HttpRequest &request, HttpResponse &response) Q_DECL_OVERRIDE;

private:
    virtual QString handlerName() const Q_DECL_OVERRIDE;
    QDomDocument buildReply(const QDomDocument &source) const;
};

#endif // QUERYLISTHANDLER_H
