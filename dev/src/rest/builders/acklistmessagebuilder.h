//@@license@@

#ifndef ACKLISTMESSAGEBUILDER_H
#define ACKLISTMESSAGEBUILDER_H

#include "xmlmessagebuilder.h"

class AckListMessageBuilder : public XmlMessageBuilder
{
    Q_OBJECT

public:
    explicit AckListMessageBuilder(QObject *parent = nullptr);
    virtual ~AckListMessageBuilder() Q_DECL_OVERRIDE;

    virtual Message *message() const Q_DECL_OVERRIDE;
    virtual MessageContent messageContent() const Q_DECL_OVERRIDE;
};

#endif // ACKLISTMESSAGEBUILDER_H
