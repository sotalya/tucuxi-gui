//@@license@@

#ifndef ACKREQUESTMESSAGEBUILDER_H
#define ACKREQUESTMESSAGEBUILDER_H

#include "xmlmessagebuilder.h"

class AckRequestMessageBuilder : public XmlMessageBuilder
{
    Q_OBJECT

public:
    explicit AckRequestMessageBuilder(QObject *parent = nullptr);
    virtual ~AckRequestMessageBuilder() Q_DECL_OVERRIDE;

    virtual Message *message() const Q_DECL_OVERRIDE;
    virtual MessageContent messageContent() const Q_DECL_OVERRIDE;
};

#endif // ACKREQUESTMESSAGEBUILDER_H
