//@@license@@

#ifndef NOTIFICATIONTESTMESSAGE_H
#define NOTIFICATIONTESTMESSAGE_H

#include "rest/model/notificationxmlmessage.h"

class NotificationTestMessage : public NotificationXmlMessage
{
    Q_OBJECT

public:
    explicit NotificationTestMessage(const QString &forRequestId, QObject *parent = nullptr);
    virtual ~NotificationTestMessage() Q_DECL_OVERRIDE;

    virtual QByteArray data() const Q_DECL_OVERRIDE;

private:
    QString _requestId;
};

#endif // NOTIFICATIONTESTMESSAGE_H
