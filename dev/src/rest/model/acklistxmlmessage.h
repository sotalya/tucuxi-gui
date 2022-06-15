//@@license@@

#ifndef ACKLISTXMLMESSAGE_H
#define ACKLISTXMLMESSAGE_H

#include "outgoingxmlmessage.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {


class AckListXmlMessage : public OutgoingXmlMessage
{
    Q_OBJECT

public:
    explicit AckListXmlMessage(QObject *parent = nullptr);
    virtual ~AckListXmlMessage();

protected:
    virtual QString schemaPath() const Q_DECL_OVERRIDE;
    virtual QString messageType() const Q_DECL_OVERRIDE;

private:
    static const char *const _MESSAGE_TYPE;
};

}
}
}

#endif // ACKLISTXMLMESSAGE_H
