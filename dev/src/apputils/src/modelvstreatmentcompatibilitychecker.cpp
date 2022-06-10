#include "modelvstreatmentcompatibilitychecker.h"

#include "core/dal/drug/drug.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/dosage.h"

ModelVsTreatmentCompatibilityChecker::ModelVsTreatmentCompatibilityChecker()
{

}

bool ModelVsTreatmentCompatibilityChecker::check(Tucuxi::Gui::Core::DrugModel *model, Tucuxi::Gui::Core::DrugTreatment *treatment)
{
    bool ok = false;
    Tucuxi::Gui::Core::DosageHistory *history = treatment->getDosages();
    foreach (Tucuxi::Gui::Core::Dosage *dosage, history->getList()) {
        ok = false;
        Tucuxi::Gui::Core::AdminList *possibleRouteList = model->getAdme()->getIntakes();

        // This check with DEFAULT is meant to be a bit laxist in case the information system connected
        // through the REST interface does not embed the route of administration
        if (dosage->getRoute()->getRoute() == Tucuxi::Gui::Core::Admin::Route::DEFAULT) {
            ok = true;
        }
        else {
            foreach (Tucuxi::Gui::Core::Admin *possibleRoute, possibleRouteList->getList()) {
                // We need at least a possible route for each dosage
                if (dosage->getRoute()->getRoute() == possibleRoute->getRoute() ) {
                    ok = true;
                }
            }
        }
        if (!ok)
            return false;
    }
    return true;
}
