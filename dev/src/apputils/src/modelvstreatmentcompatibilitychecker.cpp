#include "modelvstreatmentcompatibilitychecker.h"

#include "core/dal/drug/drug.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/dosage.h"

ModelVsTreatmentCompatibilityChecker::ModelVsTreatmentCompatibilityChecker()
{

}

bool ModelVsTreatmentCompatibilityChecker::check(ezechiel::GuiCore::DrugModel *model, ezechiel::GuiCore::DrugTreatment *treatment)
{
    bool ok = false;
    ezechiel::GuiCore::DosageHistory *history = treatment->getDosages();
    foreach (ezechiel::GuiCore::Dosage *dosage, history->getList()) {
        ok = false;
        ezechiel::GuiCore::AdminList *possibleRouteList = model->getAdme()->getIntakes();

        // This check with DEFAULT is meant to be a bit laxist in case the information system connected
        // through the REST interface does not embed the route of administration
        if (dosage->getRoute()->getRoute() == ezechiel::GuiCore::Admin::Route::DEFAULT) {
            ok = true;
        }
        else {
            foreach (ezechiel::GuiCore::Admin *possibleRoute, possibleRouteList->getList()) {
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
