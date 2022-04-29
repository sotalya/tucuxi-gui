//@@license@@

#ifndef REPLYREQUESTTESTMESSAGE_H
#define REPLYREQUESTTESTMESSAGE_H

#include "testmessagevalidator.h"
#include "rest/model/replyrequestxmlmessage.h"

class ReplyRequestTestMessage : public ReplyRequestXmlMessage, public TestMessageValidator
{
    Q_OBJECT

public:
    explicit ReplyRequestTestMessage(const QString &forRequestId, QObject *parent = nullptr);
    virtual ~ReplyRequestTestMessage() Q_DECL_OVERRIDE;

    virtual void setData(QByteArray data) Q_DECL_OVERRIDE;

private:
    QString _requestId;
};

#endif // REPLYREQUESTTESTMESSAGE_H
