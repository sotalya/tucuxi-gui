//@@license@@

#ifndef MESSAGECONTENT_H
#define MESSAGECONTENT_H

#include <QString>
#include <QMap>
#include <QMultiMap>

namespace Tucuxi {
namespace Gui {
namespace Rest {


class MessageContent
{

public:
    MessageContent();
    ~MessageContent();

    QString value(const QString &key, const QString &defaultValue = QString()) const;
    bool isValueSet(const QString &key) const;
    void setValue(const QString &key, const QString &value);
    void removeValue(const QString &key);

    QList<MessageContent> list(const QString &key) const;
    int listCount(const QString &key) const;
    void addToList(const QString &key, const MessageContent &item);
    void removeList(const QString &key);

private:
    QMap<QString, QString> content;
    QMultiMap<QString, MessageContent> lists;
};

}
}
}

#endif // MESSAGECONTENT_H
