//@@license@@

#include "drugtabcontroller.h"
#include "core/dal/drug/drug.h"
#include "guiutils/src/drugtohtml.h"


STD_PROPERTY_IMPL(DrugTabController, QString, drugHtmlDescription, DrugHtmlDescription)

DrugTabController::DrugTabController(QObject *parent) : AbstractViewController(parent),
    _drugHtmlDescription(tr("No drug selected"))
{

}


void DrugTabController::setDrugInfo(Tucuxi::Gui::Core::ActiveSubstance *substance)
{
    DrugToHtml translator;
    setDrugHtmlDescription(translator.activeSubstanceToHtml(substance));

}

void DrugTabController::setDrugModelInfo(Tucuxi::Gui::Core::DrugModel *drugModel)
{
    DrugToHtml translator;
    setDrugHtmlDescription(translator.drugToHtml(drugModel));
}
