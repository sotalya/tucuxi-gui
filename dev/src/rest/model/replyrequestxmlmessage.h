#ifndef REPLYREQUESTXMLMESSAGE_H
#define REPLYREQUESTXMLMESSAGE_H

#include "ingoingxmlmessage.h"

class ReplyRequestXmlMessage : public IngoingXmlMessage
{
    Q_OBJECT

public:
    explicit ReplyRequestXmlMessage(QObject *parent = nullptr);
    virtual ~ReplyRequestXmlMessage() Q_DECL_OVERRIDE;

protected:
    virtual QString schemaPath() const Q_DECL_OVERRIDE;
    virtual QString messageType() const Q_DECL_OVERRIDE;

private:
    static const char *const _MESSAGE_TYPE;
};

#endif // REPLYREQUESTXMLMESSAGE_H
