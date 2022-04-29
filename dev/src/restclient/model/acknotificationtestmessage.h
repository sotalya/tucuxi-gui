//@@license@@

#ifndef ACKNOTIFICATIONTESTMESSAGE_H
#define ACKNOTIFICATIONTESTMESSAGE_H

#include "testmessagevalidator.h"
#include "rest/model/acknotificationxmlmessage.h"

class AckNotificationTestMessage : public AckNotificationXmlMessage, public TestMessageValidator
{
    Q_OBJECT

public:
    explicit AckNotificationTestMessage(const QDomDocument &expected = QDomDocument(), QObject *parent = nullptr);
    virtual ~AckNotificationTestMessage() Q_DECL_OVERRIDE;

    virtual void setData(QByteArray data) Q_DECL_OVERRIDE;

private:
    QDomDocument _expected;
};

#endif // ACKNOTIFICATIONTESTMESSAGE_H
