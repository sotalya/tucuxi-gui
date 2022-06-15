//@@license@@

#ifndef NOTIFICATIONXMLMESSAGE_H
#define NOTIFICATIONXMLMESSAGE_H

#include "outgoingxmlmessage.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

class NotificationXmlMessage : public OutgoingXmlMessage
{
    Q_OBJECT

public:
    explicit NotificationXmlMessage(QObject *parent = nullptr);
    virtual ~NotificationXmlMessage() Q_DECL_OVERRIDE;

protected:
    virtual QString schemaPath() const Q_DECL_OVERRIDE;
    virtual QString messageType() const Q_DECL_OVERRIDE;

private:
    static const char *const _MESSAGE_TYPE;
};

}
}
}

#endif // NOTIFICATIONXMLMESSAGE_H
