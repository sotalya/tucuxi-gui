#include "interpretationrequest.h"
#include "admin/src/adminfactory.h"
#include "core/corefactory.h"

#include "core/dal/drugtreatment.h"
#include "admin/src/dal/practician.h"
#include "admin/src/dal/patient.h"
#include "admin/src/dal/clinical.h"


AUTO_PROPERTY_IMPL(InterpretationRequest, Tucuxi::Gui::Core::DrugTreatment*, treatment, Treatment)
AUTO_PROPERTY_IMPL(InterpretationRequest, Patient*, patient, Patient)
AUTO_PROPERTY_IMPL(InterpretationRequest, Practician*, practician, Practician)

AUTO_PROPERTY_IMPL(InterpretationRequest, QString, comedication, Comedication)
AUTO_PROPERTY_IMPL(InterpretationRequest, QString, diagnosis, Diagnosis)
AUTO_PROPERTY_IMPL(InterpretationRequest, QString, indication, Indication)
AUTO_PROPERTY_IMPL(InterpretationRequest, QString, response, Response)
AUTO_PROPERTY_IMPL(InterpretationRequest, QString, toxicity, Toxicity)
AUTO_PROPERTY_IMPL(InterpretationRequest, QString, source, Source)
AUTO_PROPERTY_IMPL(InterpretationRequest, ClinicalSet*, clinicals, Clinicals)


InterpretationRequest::InterpretationRequest(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent) :
    Entity(repository,parent)
{
    setId(-1);
    _patient = AdminFactory::createEntity<Patient>(repository, this);
    _practician = AdminFactory::createEntity<Practician>(repository, this);
    _treatment = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::DrugTreatment>(repository, this);
    _clinicals = AdminFactory::createEntity<ClinicalSet>(repository, this);
}

InterpretationRequest::InterpretationRequest(Tucuxi::Gui::Core::AbstractRepository *repository, const int &id, QObject *parent) :
    Entity(repository,id, parent)
{
    setId(id);
}
