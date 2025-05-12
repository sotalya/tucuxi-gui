/*
 * Tucuxi - Tucuxi-gui software.
 * This software is able to perform prediction of drug concentration in blood
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV.
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


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
        Tucuxi::Gui::Core::DMAdminList *possibleRouteList = model->getAdme()->getIntakes();

        // This check with DEFAULT is meant to be a bit laxist in case the information system connected
        // through the REST interface does not embed the route of administration
        if (dosage->getRoute()->getRoute() == Tucuxi::Gui::Core::DMAdmin::Route::DEFAULT) {
            ok = true;
        }
        else {
            foreach (Tucuxi::Gui::Core::DMAdmin *possibleRoute, possibleRouteList->getList()) {
                // We need at least a possible route for each dosage
                // TODO: Check if this check is relevant after all. The next one seems more apropriate
                if (dosage->getRoute()->getRoute() == possibleRoute->getRoute() ) {
                    ok = true;
                }

                if (dosage->getRoute()->getAdministrationRoute() == possibleRoute->getAdministrationRoute()) {
                    ok = true;
                }
            }
        }
        if (!ok)
            return false;
    }
    return true;
}
