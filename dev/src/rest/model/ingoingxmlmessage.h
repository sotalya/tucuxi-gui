//@@license@@

#ifndef INGOINGXMLMESSAGE_H
#define INGOINGXMLMESSAGE_H

#include "xmlmessage.h"

class IngoingXmlMessage : public XmlMessage
{
    Q_OBJECT

public:
    explicit IngoingXmlMessage(QObject *parent = nullptr);
    virtual ~IngoingXmlMessage() Q_DECL_OVERRIDE;
};

#endif // INGOINGXMLMESSAGE_H
