//@@license@@

#ifndef XMLMESSAGEBUILDER_H
#define XMLMESSAGEBUILDER_H



#include "messagebuilder.h"

#include <QString>
#include <QDomElement>

class QDomDocument;

namespace Tucuxi {
namespace Gui {
namespace Rest {




class XmlMessageBuilder : public MessageBuilder
{
    Q_OBJECT

public:
    explicit XmlMessageBuilder(QObject *parent = nullptr);
    virtual ~XmlMessageBuilder() Q_DECL_OVERRIDE;

protected:
    virtual QDomElement addDocumentElement(QDomDocument &doc, const QString &messageType, const QString &errorCondition = QString()) const;
    virtual void addNode(QDomDocument &doc, QDomElement &parent, const QString &key, const QString &tag, bool required = true) const;
};

}
}
}

#endif // XMLMESSAGEBUILDER_H
