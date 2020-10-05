#include "validationtabcontroller.h"


//STD_PROPERTY_IMPL(ValidationTabController, QString, posology, Posology)
//STD_PROPERTY_IMPL(ValidationTabController, QDateTime, nextControlDate, nextControlDate)

ValidationTabController::ValidationTabController(QObject *parent)
    : AbstractViewController(parent)
{
}


QString ValidationTabController::getShortCutText(int section, int key, int modifiers)
{
    if ((modifiers & Qt::AltModifier) && (modifiers & Qt::CTRL)) {
        if (key != Qt::AltModifier) {
            QString s = _sentencesPalettes->getSection(section)->getSentencePerKey(key);
            return s;
        }
    }
    return "";
}
