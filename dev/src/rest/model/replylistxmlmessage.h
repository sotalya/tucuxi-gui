#ifndef REPLYLISTXMLMESSAGE_H
#define REPLYLISTXMLMESSAGE_H

#include "ingoingxmlmessage.h"

class ReplyListXmlMessage : public IngoingXmlMessage
{
    Q_OBJECT

public:
    explicit ReplyListXmlMessage(QObject *parent = nullptr);
    virtual ~ReplyListXmlMessage() Q_DECL_OVERRIDE;

protected:
    virtual QString schemaPath() const Q_DECL_OVERRIDE;
    virtual QString messageType() const Q_DECL_OVERRIDE;

private:
    static const char *const _MESSAGE_TYPE;
};

#endif // REPLYLISTXMLMESSAGE_H
