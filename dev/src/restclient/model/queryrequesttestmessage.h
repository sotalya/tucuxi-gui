//@@license@@

#ifndef QUERYREQUESTTESTMESSAGE_H
#define QUERYREQUESTTESTMESSAGE_H

#include "rest/model/queryrequestxmlmessage.h"

class QueryRequestTestMessage : public QueryRequestXmlMessage
{
    Q_OBJECT

public:
    explicit QueryRequestTestMessage(const QString &forRequestId, QObject *parent = nullptr);
    virtual ~QueryRequestTestMessage() Q_DECL_OVERRIDE;

    virtual QByteArray data() const Q_DECL_OVERRIDE;

private:
    QString _requestId;
};

#endif // QUERYREQUESTTESTMESSAGE_H
