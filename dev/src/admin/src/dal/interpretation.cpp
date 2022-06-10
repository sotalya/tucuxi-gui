#include "admin/src/dal/interpretation.h"

#include "interpretationrequest.h"
#include "interpretationanalysis.h"
#include "core/corefactory.h"
#include "core/dal/dosage.h"
#include "core/dal/drugresponseanalysis.h"
#include "admin/src/dal/validationstatus.h"
#include "admin/src/dal/practician.h"

Interpretation::Interpretation(Tucuxi::GuiCore::AbstractRepository *repository, QObject *parent) :
    Entity(repository,parent), _intepretationType(InterpretationType::Undefined)
{
    setId(-1);
    _request = Tucuxi::GuiCore::CoreFactory::createEntity<InterpretationRequest>(repository, this);
    _analysis = Tucuxi::GuiCore::CoreFactory::createEntity<InterpretationAnalysis>(repository, this);
    _drugResponseAnalysis = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::DrugResponseAnalysis>(repository, this);
    _analyst = Tucuxi::GuiCore::CoreFactory::createEntity<Practician>(repository, this);
    _validationStatus = Tucuxi::GuiCore::CoreFactory::createEntity<ValidationStatus>(repository, this);
    _selectedAdjustment = nullptr;
    _adjustmentDateIsSet = false;
    _adjustmentWithRestPeriod = false;
    _adjustmentWithLoadingDose = false;
}

AUTO_PROPERTY_IMPL(Interpretation, Interpretation::InterpretationType, intepretationType, InterpretationType)
AUTO_PROPERTY_IMPL(Interpretation, InterpretationAnalysis*, analysis, Analysis)
AUTO_PROPERTY_IMPL(Interpretation, InterpretationRequest*, request, Request)
AUTO_PROPERTY_IMPL(Interpretation, Tucuxi::GuiCore::DrugResponseAnalysis*, drugResponseAnalysis, DrugResponseAnalysis)

AUTO_PROPERTY_IMPL(Interpretation, Practician*, analyst, Analyst)
AUTO_PROPERTY_IMPL(Interpretation, ValidationStatus*, validationStatus, ValidationStatus)


AUTO_PROPERTY_IMPL(Interpretation, bool, adjustmentDateIsSet, AdjustmentDateIsSet)
AUTO_PROPERTY_IMPL(Interpretation, bool, adjustmentWithLoadingDose, AdjustmentWithLoadingDose)
AUTO_PROPERTY_IMPL(Interpretation, bool, adjustmentWithRestPeriod, AdjustmentWithRestPeriod)
AUTO_PROPERTY_IMPL(Interpretation, QDateTime, adjustmentDate, AdjustmentDate)
AUTO_PROPERTY_IMPL(Interpretation, Tucuxi::GuiCore::AdjustmentDosage*, selectedAdjustment, SelectedAdjustment)
AUTO_PROPERTY_IMPL(Interpretation, QDateTime, startInterpretationTime, StartInterpretationTime)
AUTO_PROPERTY_IMPL(Interpretation, QDateTime, validateInterpretationTime, ValidateInterpretationTime)

bool Interpretation::justForDebuggingPurpose()
{
    return true;
}
