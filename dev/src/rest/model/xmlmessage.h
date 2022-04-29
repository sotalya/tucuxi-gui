//@@license@@

#ifndef XMLMESSAGE_H
#define XMLMESSAGE_H

#include "message.h"

class XmlMessage : public Message
{
    Q_OBJECT

public:
    explicit XmlMessage(QObject *parent = nullptr);
    virtual ~XmlMessage() Q_DECL_OVERRIDE = 0;

    virtual void setData(QByteArray data) Q_DECL_OVERRIDE;
    virtual QByteArray data() const Q_DECL_OVERRIDE;

    virtual bool isValid() Q_DECL_OVERRIDE;

protected:
    virtual QString schemaPath() const = 0;
    virtual QString messageType() const = 0;

    QByteArray _data;
};

#endif // XMLMESSAGE_H
