#ifndef NOTIFICATIONTESTWIDGET_H
#define NOTIFICATIONTESTWIDGET_H

#include "requesttestwidget.h"

class NotificationTestWidget : public RequestTestWidget
{
    Q_OBJECT

public:
    explicit NotificationTestWidget(QWidget *parent = 0);
    virtual ~NotificationTestWidget() Q_DECL_OVERRIDE;

    virtual void setQueryMessage(Message *message) Q_DECL_OVERRIDE;
};

#endif // NOTIFICATIONTESTWIDGET_H
