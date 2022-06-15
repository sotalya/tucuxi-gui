//@@license@@

#ifndef ACKREQUESTXMLMESSAGE_H
#define ACKREQUESTXMLMESSAGE_H

#include "outgoingxmlmessage.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

class AckRequestXmlMessage : public OutgoingXmlMessage
{
    Q_OBJECT

public:
    explicit AckRequestXmlMessage(QObject *parent = nullptr);
    virtual ~AckRequestXmlMessage();

protected:
    virtual QString schemaPath() const Q_DECL_OVERRIDE;
    virtual QString messageType() const Q_DECL_OVERRIDE;

private:
    static const char *const _MESSAGE_TYPE;
};

}
}
}

#endif // ACKREQUESTXMLMESSAGE_H
