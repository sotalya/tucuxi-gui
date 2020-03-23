#include "notificationtestwidget.h"
#include "restclient/model/acknotificationtestmessage.h"

#include "rest/network/notificationrequest.h"
#include "rest/model/message.h"

NotificationTestWidget::NotificationTestWidget(QWidget *parent) :
    RequestTestWidget(new NotificationRequest, parent)
{

}

NotificationTestWidget::~NotificationTestWidget()
{

}

void NotificationTestWidget::setQueryMessage(Message *message)
{
    _request->setMessage(message);

    QDomDocument doc;
    if (!doc.setContent(message->data()))
        qFatal(qPrintable(tr("Could retrieve notification message content")));

    QString controlId = doc.documentElement().attributeNode("controlId").value();
    if (controlId.isEmpty())
        qFatal(qPrintable(tr("Could retrieve notification message control ID")));

    setReplyMessage(new AckNotificationTestMessage(buildAck(controlId, "ack_notification", "AA")));
}
