//@@license@@

#ifndef QUERYREQUESTXMLMESSAGE_H
#define QUERYREQUESTXMLMESSAGE_H

#include "outgoingxmlmessage.h"

class QueryRequestXmlMessage : public OutgoingXmlMessage
{
    Q_OBJECT

public:
    explicit QueryRequestXmlMessage(QObject *parent = nullptr);
    virtual ~QueryRequestXmlMessage() Q_DECL_OVERRIDE;

protected:
    virtual QString schemaPath() const Q_DECL_OVERRIDE;
    virtual QString messageType() const Q_DECL_OVERRIDE;

private:
    static const char *const _MESSAGE_TYPE;
};

#endif // QUERYREQUESTXMLMESSAGE_H
