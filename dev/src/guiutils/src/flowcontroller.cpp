//@@license@@

#include "flowcontroller.h"
#include "admin/src/dal/interpretation.h"
#include "admin/src/dal/interpretationrequest.h"
#include "core/dal/chartdata.h"
#include "core/dal/drugtreatment.h"
#include "admin/src/dal/validationstatus.h"
#include "core/dal/dosage.h"
#include "core/dal/drug/target.h"
#include "core/dal/covariate.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drug/drugvariate.h"
#include "core/dal/drug/drug.h"
#include "core/utils/logging.h"
#include "core/dal/coremeasure.h"
#include "guiutils/errors_guiutils.h"
#include "core/utils/connect.h"
#include "core/dal/uncastedvalue.h"

#include "guiutils/src/controllers/interpretationcontroller.h"

#include "core/core.h"

typedef ValidationStatus::StatusType StatusType;

FlowController::FlowController(QObject *parent) :
    QObject(parent),
    interpretation(nullptr)
{
    initialize();
    interpretationController = dynamic_cast<InterpretationController*>(parent);
}

FlowController::~FlowController()
{
    delete oldStatus;
}

void FlowController::initialize()
{
    currentStatus = ezechiel::GuiCore::CoreFactory::createEntity<ValidationStatus>(ABSTRACTREPO,this);
    oldStatus = ezechiel::GuiCore::CoreFactory::createEntity<ValidationStatus>(ABSTRACTREPO,this);
}

void FlowController::setFlowView(QObject *flowView)
{
    this->flowView = flowView;
}

void FlowController::setInterpretation(Interpretation *interpretation)
{
    this->interpretation = interpretation;
    this->currentStatus = interpretation->getValidationStatus();
}

void FlowController::propagateStatus(DataStatusType s, StepType::Enum from)
{
    for(int i = int(from); i <= StepType::last; i++) {
        currentStatus->setDataStatus(StepType::convert(i), s);
    }
}

void FlowController::propagateUnvalidate(StepType::Enum from)
{
    for(int i = int(from); i <= StepType::last; i++) {
        currentStatus->setValidationStatus(StepType::convert(i), ValidationStatusType::UnValidated);
    }
}

void FlowController::propagateEnable(bool s, StepType::Enum from)
{
    for(int i = int(from); i <= StepType::last; i++) {
        currentStatus->setEnable(StepType::convert(i), s);
    }
}

void FlowController::validateTab(StepType::Enum tabNumber)
{
    currentStatus->validate(tabNumber);
    validating = tabNumber;
    evaluate();
}

void FlowController::toggleValidation(StepType::Enum tabNumber)
{
    currentStatus->toggleValidation(tabNumber);
    evaluate();
}

void FlowController::invalidateTab(StepType::Enum tabNumber)
{
    invalidating = tabNumber;
    evaluate();
}

void FlowController::patientChanged(int index)
{
    invalidateInterpretation();
    propagateUnvalidate(StepType::Patient);
    start();
    if (index==-1) {
        currentStatus->setDataStatus(StepType::Patient, DataStatusType::InvalidData);
//        EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, tr("Set patientTab to status invalid"))
    }
    else {
        currentStatus->setDataStatus(StepType::Patient, DataStatusType::ValidData);
        if (!interpretationController->isFlowRequest()) {
            currentStatus->setValidationStatus(StepType::Patient, ValidationStatusType::Validated);
        }
//        EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, tr("Set patientTab to status valid"))
    }
    end();
}

void FlowController::drugChanged(int index)
{
    invalidateInterpretation();
    propagateUnvalidate(StepType::Drug);
    start();
    if (index==-1) {
        currentStatus->setDataStatus(StepType::Drug, DataStatusType::InvalidData);
//        EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, tr("Set drugTab to status invalid"))
    }
    else if (currentStatus->getDataStatus(StepType::Patient) == DataStatusType::ValidData) {
        currentStatus->setDataStatus(StepType::Drug, DataStatusType::ValidData);
        if (!interpretationController->isFlowRequest()) {
            currentStatus->setValidationStatus(StepType::Drug, ValidationStatusType::Validated);
        }
//        EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, tr("Set drugTab to status valid"))
    }
    end();
}

void FlowController::generateEnables()
{
    ezechiel::GuiCore::DrugTreatment* drugTreatment = interpretation->getDrugResponseAnalysis()->getTreatment();

    // Patient
    if (currentStatus->getDataStatus(StepType::Patient) != DataStatusType::ValidData) {
//        EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, tr("Propagate disable drug"))
        propagateEnable(false, StepType::Drug);
        return;
    }

    // Drug
    currentStatus->setEnable(StepType::Drug, true);
    if (currentStatus->getDataStatus(StepType::Drug) != DataStatusType::ValidData) {
//        EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, tr("Propagate disable dosage"))
        propagateEnable(false, StepType::Dosage);
        return;
    }

    // Dosage
    currentStatus->setEnable(StepType::Dosage, true);
//    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, tr("Enable dosageTab"))

    // Covariates
    if (!interpretation->getDrugResponseAnalysis()->getDrugModel()->getCovariates()->isEmpty()) {
        currentStatus->setEnable(StepType::Covariate, true);
//        EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, tr("Enable covariateTab"))
    } else {
        currentStatus->setEnable(StepType::Covariate, false);
//        EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, tr("Disable covariateTab"))
    }

    // Measures
    currentStatus->setEnable(StepType::Measure, drugTreatment->getDosages()->size() > 0);
//    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, tr("Enable measureTab"))

    // Target
    currentStatus->setEnable(StepType::Target, true);
//    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, tr("Enable targetTab"))

    // Adjustment
    bool isDosageValid = true; //(currentStatus->getStatus(StepType::Dosage) == DataStatusType::ValidData);
    bool isTargetValid = (currentStatus->getStatus(StepType::Target) != StatusType::Invalid);
    currentStatus->setEnable(StepType::Adjustment, isDosageValid && isTargetValid);

    // Validation
    bool isAdjustmentValid = (currentStatus->getStatus(StepType::Adjustment) != StatusType::Invalid);
    bool isAdjustmentDefined = !drugTreatment->getAdjustments()->isEmpty();
    currentStatus->setEnable(StepType::Validation, isAdjustmentValid && isAdjustmentDefined);

    // Report
    currentStatus->setEnable(StepType::Report, _isInterpretationValidated);
}

void FlowController::generateStatuses()
{
    if (interpretation == nullptr) {
        return;
    }

    ezechiel::GuiCore::DrugModel* drugModel = interpretation->getDrugResponseAnalysis()->getDrugModel();
    ezechiel::GuiCore::DrugTreatment* drugTreatment = interpretation->getDrugResponseAnalysis()->getTreatment();

    // Patient
    if (drugTreatment->getPatient()) {
        currentStatus->setDataStatus(StepType::Patient, DataStatusType::ValidData);
    }

    // Drug
    if (drugModel == nullptr) {
        propagateStatus(DataStatusType::InvalidData, StepType::Drug);
        propagateUnvalidate(StepType::Drug);
        return;
    }
    currentStatus->setDataStatus(StepType::Drug, DataStatusType::ValidData);

    // Dosages
    if (drugTreatment->getDosages() != nullptr) {
        bool isValid = drugTreatment->getDosages()->isValid();
        for(int i = 0; i < drugTreatment->getDosages()->size(); i++) {
            ezechiel::GuiCore::Dosage *dosage = drugTreatment->getDosages()->at(i);
            for(int j=0; j < dosage->getUncastedValues()->size(); j++) {
                ezechiel::GuiCore::UncastedValue *value = dosage->getUncastedValues()->at(j);
                if (value->getStatus() != ezechiel::GuiCore::UncastedStatus::Casted) {
                    if (!value->getValidated()) {
                        isValid = false;
                    }
                }
            }
        }

        currentStatus->setDataStatus(StepType::Dosage, isValid ? DataStatusType::ValidData : DataStatusType::InvalidData);
        if (!isValid) {
            propagateUnvalidate(StepType::Dosage);
        }
        else {
            if (!interpretationController->isFlowRequest()) {
                currentStatus->setValidationStatus(StepType::Dosage, ValidationStatus::Validated);
            }
        }
    }

    // Covariates
    if (drugModel->getCovariates()) {
        bool hasDrugCovariates = !drugModel->getCovariates()->isEmpty();
        bool areDrugCovariatesOk = (!hasDrugCovariates || drugModel->getCovariates()->isValid());
        bool arePatientCovariatesOk = (drugTreatment->getCovariates() != nullptr && drugTreatment->getCovariates()->isValid());
        bool isDosageOk = currentStatus->getValidationStatus(StepType::Dosage);
//        std::cout << "Dosage status : " << (isDosageOk ? "OK" : "NOK") << std::endl;
        bool isValid = (areDrugCovariatesOk && arePatientCovariatesOk && isDosageOk);
        currentStatus->setDataStatus(StepType::Covariate, isValid ? DataStatusType::ValidData : DataStatusType::InvalidData);
        isValid = false;
//        if (isValid) {
//            // Set validated as soon as we have at least one patient variable for each drug variable
//            QSet<QString> drugVariateIds;
//            for (ezechiel::GuiCore::DrugVariate *variate: drugModel->getCovariates()->getList()) {
//                if ((!variate->getAutomatic()) && (variate->getCovariateId() != "birthdate")) {
//                    drugVariateIds.insert(variate->getCovariateId());
//                }
//            }
//            bool isOk = true;
//            for (ezechiel::GuiCore::PatientVariate *variate: drugTreatment->getCovariates()->getList()) {
//                if (drugVariateIds.contains(variate->getCovariateId())) {
//                    drugVariateIds.remove(variate->getCovariateId());
//                }
//                else {
//                    // No real reason to invalidate in that case
//                    // isOk = false;
//                }
//            }
//            isOk &= drugVariateIds.size() == 0;
//            currentStatus->setValidationStatus(StepType::Covariate, isOk ? ValidationStatus::Validated : ValidationStatus::UnValidated);
//        }
    }

    // Measures
    if (drugTreatment->getMeasures() != nullptr) {
        bool isValid = !drugTreatment->getMeasures()->isEmpty();
        currentStatus->setDataStatus(StepType::Measure, isValid ? DataStatusType::ValidData : DataStatusType::InvalidData);
        if (isValid && !interpretationController->isFlowRequest()) {
            currentStatus->setValidationStatus(StepType::Measure, ValidationStatus::Validated);
        }
    }

    // Targets
    if (drugModel->getTargets() != nullptr) {
        bool isValid = !drugTreatment->getTargets()->isEmpty() && drugModel->getTargets()->isValid();
        currentStatus->setDataStatus(StepType::Target, isValid ? DataStatusType::ValidData : DataStatusType::InvalidData);
        if (isValid && !interpretationController->isFlowRequest()) {
            currentStatus->setValidationStatus(StepType::Target, ValidationStatus::Validated);
        }
    }

    // Adjustments
    ezechiel::GuiCore::DosageHistory *adjustments = drugTreatment->getAdjustments();
    ValidationStatusType adjValidated = ValidationStatus::UnValidated;
    if (adjustments != nullptr) {
        if (adjustments->isValid()) {
            propagateStatus(DataStatusType::ValidData, StepType::Adjustment);
            if (adjustments->size() > 0) {
                adjValidated = ValidationStatus::Validated;
            }
        }
        else {
        //    propagateStatus(DataStatusType::InvalidData, StepType::Adjustment);
        }
    }
    currentStatus->setValidationStatus(StepType::Adjustment, adjValidated);

    currentStatus->setValidationStatus(StepType::Validation, _isInterpretationValidated ? ValidationStatusType::Validated : ValidationStatusType::UnValidated);
    currentStatus->setValidationStatus(StepType::Report, _isInterpretationValidated ? ValidationStatusType::Validated : ValidationStatusType::UnValidated);

/*
    if (interpretation->getAnalysis()->getChartData()) {
        if (interpretation->getAnalysis()->getChartData()) {
            if (interpretation->getAnalysis()->getChartData()->getRevPred()) {
                if (interpretation->getAnalysis()->getChartData()->getRevPred()->getAdjustments()->isValid()) {
                    currentStatus->setDataStatus(StepType::Adjustment, DataStatusType::ValidData);
                    propagateEnable(true, StepType::Validation);
                    propagateStatus(DataStatusType::ValidData, StepType::Validation);
                } else {
                    currentStatus->setDataStatus(StepType::Adjustment, DataStatusType::InvalidData);
                    propagateEnable(false, StepType::Validation);
                    propagateStatus(DataStatusType::InvalidData, StepType::Validation);
                }
            }
        }
    }
*/
}

void FlowController::start()
{
    oldStatus->copyFrom(currentStatus);
/* YJ Not necessary as it is already done in copyFrom...
    for(int i = StepType::first; i <= StepType::last; i++) {
        oldStatus->setEnable(StepType::convert(i), currentStatus->getEnable(StepType::convert(i)));
    }
*/
}

void FlowController::end()
{
    if (!(currentStatus == oldStatus)) {
        CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(flowView,     "setStatus",
                                                     Q_ARG(QVariant, QVariant::fromValue(currentStatus))));
    }
}

void FlowController::evaluate()
{
    start();
    generateStatuses();
    generateEnables();
    end();
}

void FlowController::startNewInterpretation()
{
    invalidateInterpretation();
    propagateEnable(false, StepType::convert(StepType::first));
    currentStatus->setEnable(StepType::Patient, true);
    propagateStatus(DataStatusType::InvalidData, StepType::convert(StepType::first));
    propagateUnvalidate(StepType::convert(StepType::first));

    CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(flowView,     "setStatus",
                                                 Q_ARG(QVariant, QVariant::fromValue(currentStatus))));
}

void FlowController::loadInterpretation()
{
    currentStatus->setEnable(StepType::Patient, true);

    CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(flowView,     "setStatus",
                                                 Q_ARG(QVariant, QVariant::fromValue(currentStatus))));

    evaluate();
}

void FlowController::validateInterpretation()
{
    start();
    _isInterpretationValidated = true;
    evaluate();
    end();
}

void FlowController::invalidateInterpretation()
{
    _isInterpretationValidated = false;
}
