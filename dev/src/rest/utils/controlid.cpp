//@@license@@

#include "controlid.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

ControlId &ControlId::getInstance()
{
    static ControlId controlId;
    return controlId;
}

qint64 ControlId::next()
{
    return ++currentId;
}

ControlId::ControlId() :
    QObject(),
    currentId(Q_INT64_C(0))
{

}

ControlId::~ControlId()
{

}

}
}
}
