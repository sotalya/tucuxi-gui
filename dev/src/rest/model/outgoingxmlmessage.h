//@@license@@

#ifndef OUTGOINGXMLMESSAGE_H
#define OUTGOINGXMLMESSAGE_H

#include "xmlmessage.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

class OutgoingXmlMessage : public XmlMessage
{
    Q_OBJECT

public:
    explicit OutgoingXmlMessage(QObject *parent = nullptr);
    virtual ~OutgoingXmlMessage() Q_DECL_OVERRIDE;
};

}
}
}

#endif // OUTGOINGXMLMESSAGE_H
