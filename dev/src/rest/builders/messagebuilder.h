//@@license@@

#ifndef MESSAGEBUILDER_H
#define MESSAGEBUILDER_H

#include "messagecontent.h"

#include <QObject>

namespace Tucuxi {
namespace Gui {
namespace Rest {


class Message;

class MessageBuilder : public QObject
{
    Q_OBJECT

public:
    explicit MessageBuilder(QObject *parent = nullptr);
    virtual ~MessageBuilder() Q_DECL_OVERRIDE;

    virtual Message *message() const = 0;
    virtual void setMessage(Message *message);

    virtual MessageContent messageContent() const = 0;
    virtual void setMessageContent(const MessageContent &content);

protected:
    const Message *_message;
    MessageContent _content;
};

}
}
}

#endif // MESSAGEBUILDER_H
