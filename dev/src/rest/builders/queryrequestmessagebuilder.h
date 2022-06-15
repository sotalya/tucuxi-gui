//@@license@@

#ifndef QUERYREQUESTMESSAGEBUILDER_H
#define QUERYREQUESTMESSAGEBUILDER_H

#include "xmlmessagebuilder.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

class QueryRequestMessageBuilder : public XmlMessageBuilder
{
    Q_OBJECT

public:
    explicit QueryRequestMessageBuilder(QObject *parent = nullptr);
    virtual ~QueryRequestMessageBuilder() Q_DECL_OVERRIDE;

    virtual Message *message() const Q_DECL_OVERRIDE;
    virtual MessageContent messageContent() const Q_DECL_OVERRIDE;
};

}
}
}

#endif // QUERYREQUESTMESSAGEBUILDER_H
