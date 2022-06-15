//@@license@@

#include "xmlresttointerpretationrequest.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {


XmlRestToInterpretationRequest::XmlRestToInterpretationRequest()
{
}

InterpretationRequest *XmlRestToInterpretationRequest::interpretationRequestFromXml(const QByteArray &data)
{

    // To implement that use the reader and maybe some hint from repplyrequestxmlmessage.h
    return 0;
}

}
}
}
