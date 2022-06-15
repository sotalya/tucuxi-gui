//@@license@@

#ifndef INTERPRETATIONREQUESTTOXMLREST_H
#define INTERPRETATIONREQUESTTOXMLREST_H

#include <QXmlStreamWriter>

#include "admin/src/dal/interpretationrequest.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

class InterpretationRequestToXmlRest
{
public:
    InterpretationRequestToXmlRest();

    void interpretationRequestToXmlRest(const InterpretationRequest &interpretation, QByteArray &data);

protected:
    QXmlStreamWriter writer;
};

}
}
}

#endif // INTERPRETATIONREQUESTTOXMLREST_H
