#include "message.h"

Message::Message(QObject *parent) :
    QObject(parent),
    _errorString()
{

}

Message::~Message()
{

}

QString Message::errorString() const
{
    return _errorString;
}

void Message::setErrorString(const QString &errorString)
{
    _errorString = errorString;
}

void Message::appendErrorString(const QString &errorString)
{
    _errorString.append(errorString);
}
