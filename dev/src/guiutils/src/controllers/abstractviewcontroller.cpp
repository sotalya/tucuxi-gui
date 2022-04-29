//@@license@@

#include "abstractviewcontroller.h"
#include "interpretationcontroller.h"

AbstractViewController::AbstractViewController(QObject *parent) : QObject(parent)
{
}

void AbstractViewController::setMasterController(InterpretationController *controller)
{
    masterController = controller;
}
