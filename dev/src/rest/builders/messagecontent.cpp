#include "messagecontent.h"

MessageContent::MessageContent() :
    content(),
    lists()
{

}

MessageContent::~MessageContent()
{

}

QString MessageContent::value(const QString &key, const QString &defaultValue) const
{
    return content.value(key, defaultValue);
}

bool MessageContent::isValueSet(const QString &key) const
{
    return content.contains(key);
}

void MessageContent::setValue(const QString &key, const QString &value)
{
    content.insert(key, value);
}

void MessageContent::removeValue(const QString &key)
{
    content.remove(key);
}

QList<MessageContent> MessageContent::list(const QString &key) const
{
    return lists.values(key);
}

int MessageContent::listCount(const QString &key) const
{
    return lists.values(key).count();
}

void MessageContent::addToList(const QString &key, const MessageContent &item)
{
    lists.insert(key, item);
}

void MessageContent::removeList(const QString &key)
{
    lists.remove(key);
}
