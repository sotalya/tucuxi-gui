//@@license@@

#include "validationtabcontroller.h"


//STD_PROPERTY_IMPL(ValidationTabController, QString, posology, Posology)
//STD_PROPERTY_IMPL(ValidationTabController, QDateTime, nextControlDate, nextControlDate)

using namespace Tucuxi::Gui::GuiUtils;

ValidationTabController::ValidationTabController(QObject *parent)
    : AbstractViewController(parent)
{
}


QString ValidationTabController::getShortCutText(int section, int key, int modifiers)
{
    if ((modifiers & Qt::AltModifier) || (modifiers & Qt::ControlModifier)) {
            QString s = _sentencesPalettes->getSection(section)->getSentencePerKey(key, modifiers);
            return s;
    }
    return "";
}
