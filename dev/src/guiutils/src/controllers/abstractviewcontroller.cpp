//@@license@@

#include "abstractviewcontroller.h"
#include "interpretationcontroller.h"

using namespace Tucuxi::Gui::GuiUtils;

AbstractViewController::AbstractViewController(QObject *parent) : QObject(parent)
{
}

void AbstractViewController::setMasterController(InterpretationController *controller)
{
    masterController = controller;
}
