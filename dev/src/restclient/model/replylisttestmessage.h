//@@license@@

#ifndef REPLYLISTTESTMESSAGE_H
#define REPLYLISTTESTMESSAGE_H

#include "testmessagevalidator.h"
#include "rest/model/replylistxmlmessage.h"

class ReplyListTestMessage : public ReplyListXmlMessage, public TestMessageValidator
{
    Q_OBJECT

public:
    explicit ReplyListTestMessage(QObject *parent = nullptr);
    virtual ~ReplyListTestMessage() Q_DECL_OVERRIDE;

    virtual void setData(QByteArray data) Q_DECL_OVERRIDE;
};

#endif // REPLYLISTTESTMESSAGE_H
