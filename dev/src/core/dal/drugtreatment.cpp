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


#include "core/dal/drugtreatment.h"
#include "core/core.h"
//#include "core_errors.h"
#include "admin/src/stdadminrepository.h"
#include <QDebug>

#include "core/dal/corepatient.h"
#include "core/dal/drug/drug.h"
#include "core/dal/drug/target.h"
#include "core/dal/covariate.h"
#include "core/dal/coremeasure.h"
#include "core/dal/dosage.h"
#include "core/dal/drug/concentrations.h"
#include "core/dal/drug/doses.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


AUTO_PROPERTY_IMPL(DrugTreatment, QString, name, Name)
AUTO_PROPERTY_IMPL(DrugTreatment, QString, activeSubstanceId, ActiveSubstanceId)
AUTO_PROPERTY_IMPL(DrugTreatment, CorePatient*, patient, Patient)
//AUTO_PROPERTY_IMPL(DrugTreatment, ActiveSubstance*, drug, Drug)
AUTO_PROPERTY_IMPL(DrugTreatment, DosageHistory*, dosages, Dosages)
AUTO_PROPERTY_IMPL(DrugTreatment, DosageHistory*, adjustments, Adjustments)
AUTO_PROPERTY_IMPL(DrugTreatment, PatientVariateList*, covariates, Covariates)
AUTO_PROPERTY_IMPL(DrugTreatment, CoreMeasureList*, measures, Measures)
AUTO_PROPERTY_IMPL(DrugTreatment, TargetList*, targets, Targets)

#define PNA_MAX 100

DrugTreatment::DrugTreatment(AbstractRepository *repository, QObject* parent)
    : Entity(repository),
      _patient(nullptr),
  //    _drug(nullptr),
      _dosages(Tucuxi::Gui::Core::CoreFactory::createEntity<DosageHistory>(repository, this)),
      _adjustments(Tucuxi::Gui::Core::CoreFactory::createEntity<DosageHistory>(repository, this)),
//      _drug(CoreFactory::createEntity<Drug>(repository, this)),
      _covariates(CoreFactory::createEntity<PatientVariateList>(repository, this)),
      _measures(CoreFactory::createEntity<CoreMeasureList>(repository, this)),
      _targets(CoreFactory::createEntity<TargetList>(repository, this))
{
}
/*
Unit DrugTreatment::doseUnit() const {
    return getDrug()->getDoses()->getQuantity()->unit().name();
}

Unit DrugTreatment::concentrationUnit() const {
    return getDrug()->getConcentrations()->getQuantity().unit();
}
*/

} // namespace Core
} // namespace Gui
} // namespace Tucuxi
