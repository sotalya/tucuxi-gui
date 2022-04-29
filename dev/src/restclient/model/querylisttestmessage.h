//@@license@@

#ifndef QUERYLISTTESTMESSAGE_H
#define QUERYLISTTESTMESSAGE_H

#include "rest/model/querylistxmlmessage.h"

class QueryListTestMessage : public QueryListXmlMessage
{
    Q_OBJECT

public:
    explicit QueryListTestMessage(QObject *parent = nullptr);
    virtual ~QueryListTestMessage() Q_DECL_OVERRIDE;

    virtual QByteArray data() const Q_DECL_OVERRIDE;
};

#endif // QUERYLISTTESTMESSAGE_H
