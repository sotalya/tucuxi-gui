//@@license@@

#include "controlid.h"

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
