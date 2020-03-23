#ifndef REPLYREQUESTMESSAGEBUILDER_H
#define REPLYREQUESTMESSAGEBUILDER_H

#include "xmlmessagebuilder.h"

class ReplyRequestMessageBuilder : public XmlMessageBuilder
{
    Q_OBJECT

public:
    explicit ReplyRequestMessageBuilder(QObject *parent = nullptr);
    virtual ~ReplyRequestMessageBuilder() Q_DECL_OVERRIDE;

    virtual Message *message() const Q_DECL_OVERRIDE;
    virtual MessageContent messageContent() const Q_DECL_OVERRIDE;
};

#endif // REPLYREQUESTMESSAGEBUILDER_H
