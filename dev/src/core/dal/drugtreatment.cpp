//@@license@@

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

namespace ezechiel {
namespace core {

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
      _dosages(ezechiel::core::CoreFactory::createEntity<DosageHistory>(repository, this)),
      _adjustments(ezechiel::core::CoreFactory::createEntity<DosageHistory>(repository, this)),
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

} // namespace core
} // namespace ezechiel
