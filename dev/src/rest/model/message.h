#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QTextStream>
#include <QDomDocument>

class Message : public QObject
{
    Q_OBJECT

public:
    explicit Message(QObject *parent = nullptr);
    virtual ~Message();

    virtual void setData(QByteArray data) = 0;
    virtual QByteArray data() const = 0;

    virtual bool isValid() = 0;

    QString errorString() const;

protected:
    void setErrorString(const QString &errorString);
    void appendErrorString(const QString &errorString);

private:
    QString _errorString;
};

#endif // MESSAGE_H
