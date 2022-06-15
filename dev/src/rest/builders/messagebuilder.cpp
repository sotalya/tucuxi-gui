//@@license@@

#include "messagebuilder.h"

#include "rest/model/message.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {


MessageBuilder::MessageBuilder(QObject *parent) :
    QObject(parent),
    _message(nullptr),
    _content()
{

}

MessageBuilder::~MessageBuilder()
{

}

void MessageBuilder::setMessage(Message *message)
{
    Q_ASSERT(message);
    Q_ASSERT(message->isValid());
    _message = message;
}

void MessageBuilder::setMessageContent(const MessageContent &content)
{
    _content = content;
}

}
}
}
