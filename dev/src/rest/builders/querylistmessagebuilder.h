//@@license@@

#ifndef QUERYLISTMESSAGEBUILDER_H
#define QUERYLISTMESSAGEBUILDER_H

#include "xmlmessagebuilder.h"

class QueryListMessageBuilder : public XmlMessageBuilder
{
    Q_OBJECT

public:
    explicit QueryListMessageBuilder(QObject *parent = nullptr);
    virtual ~QueryListMessageBuilder() Q_DECL_OVERRIDE;

    virtual Message *message() const Q_DECL_OVERRIDE;
    virtual MessageContent messageContent() const Q_DECL_OVERRIDE;
};

#endif // QUERYLISTMESSAGEBUILDER_H
