//@@license@@

#ifndef XMLTOINTERPRETATIONREQUEST_H
#define XMLTOINTERPRETATIONREQUEST_H

#include <QXmlStreamReader>

#include "admin/src/dal/interpretationrequest.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {


class XmlRestToInterpretationRequest
{
public:
    XmlRestToInterpretationRequest();

    Tucuxi::Gui::Admin::InterpretationRequest *interpretationRequestFromXml(const QByteArray &data);

protected:
    QXmlStreamReader reader;
};

}
}
}

#endif // XMLTOINTERPRETATIONREQUEST_H
