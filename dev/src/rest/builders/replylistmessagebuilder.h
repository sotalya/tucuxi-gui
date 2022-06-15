//@@license@@

#ifndef REPLYLISTMESSAGEBUILDER_H
#define REPLYLISTMESSAGEBUILDER_H

#include "xmlmessagebuilder.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {


class ReplyListMessageBuilder : public XmlMessageBuilder
{
    Q_OBJECT

public:
    explicit ReplyListMessageBuilder(QObject *parent = nullptr);
    virtual ~ReplyListMessageBuilder() Q_DECL_OVERRIDE;

    virtual Message *message() const Q_DECL_OVERRIDE;
    virtual MessageContent messageContent() const Q_DECL_OVERRIDE;
};

}
}
}

#endif // REPLYLISTMESSAGEBUILDER_H
