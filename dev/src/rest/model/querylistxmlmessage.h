#ifndef QUERYLISTXMLMESSAGE_H
#define QUERYLISTXMLMESSAGE_H

#include "outgoingxmlmessage.h"

class QueryListXmlMessage : public OutgoingXmlMessage
{
    Q_OBJECT

public:
    explicit QueryListXmlMessage(QObject *parent = nullptr);
    virtual ~QueryListXmlMessage() Q_DECL_OVERRIDE;

protected:
    virtual QString schemaPath() const Q_DECL_OVERRIDE;
    virtual QString messageType() const Q_DECL_OVERRIDE;

private:
    static const char *const _MESSAGE_TYPE;
};

#endif // QUERYLISTXMLMESSAGE_H
