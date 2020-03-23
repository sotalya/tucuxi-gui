#include "predictionspeccopier.h"
#include "core/dal/predictionspec.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/covariate.h"
#include "core/dal/dosage.h"
#include "core/dal/drug/drug.h"
#include "core/dal/drug/adme.h"
#include "core/dal/drug/drugvariate.h"
#include "core/dal/drug/concentrations.h"
#include "core/dal/drug/doses.h"
#include "core/dal/corepatient.h"
#include "core/dal/coremeasure.h"
#include "core/dal/drug/errormodel.h"
#include "core/dal/drug/halflife.h"
#include "core/dal/drug/infusions.h"
#include "core/dal/drug/intervals.h"
#include "core/dal/drug/target.h"
#include "core/dal/drug/bsv.h"
#include "core/dal/drug/standardtreatment.h"

PredictionSpecCopier::PredictionSpecCopier()
{

}

ezechiel::core::PredictionSpec *PredictionSpecCopier::copy(ezechiel::core::PredictionSpec *spec, QObject *parent)
{
    ezechiel::core::PredictionSpec *newSpec;
    newSpec = ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionSpec>(ABSTRACTREPO,parent);
    newSpec->setCalculationType(spec->getCalculationType());
    newSpec->setAdjustmentDate(spec->getAdjustmentDate());
    newSpec->setAdjustmentWithLoadingDose(spec->getAdjustmentWithLoadingDose());
    newSpec->setAdjustmentWithRestPeriod(spec->getAdjustmentWithRestPeriod());
    newSpec->setAnalysis(copy(spec->getAnalysis(), newSpec));
    newSpec->setEndDate(spec->getEndDate());
    newSpec->setId(spec->id());
    newSpec->setNbPoints(spec->getNbPoints());
    newSpec->setParamsType(spec->getParamsType());
    newSpec->setPercentileList(spec->getPercentileList());
    newSpec->setStartDate(spec->getStartDate());
    newSpec->setSteadyState(spec->getSteadyState());

    return newSpec;
}


ezechiel::core::DrugResponseAnalysis *PredictionSpecCopier::copy(ezechiel::core::DrugResponseAnalysis *analysis, QObject *parent)
{
    ezechiel::core::DrugResponseAnalysis *newAnalysis;
    newAnalysis = ezechiel::core::CoreFactory::createEntity<ezechiel::core::DrugResponseAnalysis>(ABSTRACTREPO,parent);

    newAnalysis->setTreatment(copy(analysis->getTreatment(), newAnalysis));
    newAnalysis->setDrugModel(copy(analysis->getDrugModel(), newAnalysis));

    return newAnalysis;
}


ezechiel::core::IdentifiableAmount *PredictionSpecCopier::copy(ezechiel::core::IdentifiableAmount *amount, QObject *parent)
{
    ezechiel::core::IdentifiableAmount *newAmount;
    newAmount = ezechiel::core::CoreFactory::createEntity<ezechiel::core::IdentifiableAmount>(ABSTRACTREPO,parent);

    newAmount->setAmountId(amount->getAmountId());
    newAmount->setDbvalue(amount->getDbvalue());
    newAmount->setUnit(amount->unit());
    newAmount->setValue(amount->value());

    return newAmount;
}

ezechiel::core::OperableAmount *PredictionSpecCopier::copy(ezechiel::core::OperableAmount *amount, QObject *parent)
{
    ezechiel::core::OperableAmount *newAmount;
    newAmount = ezechiel::core::CoreFactory::createEntity<ezechiel::core::OperableAmount>(ABSTRACTREPO,parent);

    newAmount->setAmountId(amount->getAmountId());
    newAmount->setDbvalue(amount->getDbvalue());
    newAmount->setFixed(amount->getFixed());
    newAmount->setOperations(copy(amount->getOperations(), newAmount));
    newAmount->setUnit(amount->unit());
    newAmount->setValue(amount->value());
    // Something strange with OperableAmount. To be checked

    return newAmount;
}


ezechiel::core::PatientVariate* PredictionSpecCopier::copy(ezechiel::core::PatientVariate* variate, QObject *parent)
{
    ezechiel::core::PatientVariate *newVariate;
    newVariate = ezechiel::core::CoreFactory::createEntity<ezechiel::core::PatientVariate>(ABSTRACTREPO,parent);
    // To be completed
    newVariate->setDate(variate->getDate());
    newVariate->setForced(variate->getForced());
    newVariate->setCovariateId(variate->getCovariateId());
    newVariate->setQuantity(copy(variate->getQuantity(), newVariate));
    newVariate->setType(variate->getType());
    newVariate->setValueAsString(variate->getValueAsString());

    return newVariate;
}

ezechiel::core::PatientVariateList* PredictionSpecCopier::copy(ezechiel::core::PatientVariateList* list, QObject *parent)
{
    ezechiel::core::PatientVariateList *newList;
    newList = ezechiel::core::CoreFactory::createEntity<ezechiel::core::PatientVariateList>(ABSTRACTREPO,parent);
    foreach(ezechiel::core::PatientVariate *variate, list->getList())
        newList->append(copy(variate, newList));

    return newList;
}


ezechiel::core::Admin* PredictionSpecCopier::copy(ezechiel::core::Admin* admin, QObject *parent)
{
    ezechiel::core::Admin *newAdmin;
    newAdmin = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Admin>(ABSTRACTREPO,parent);
    newAdmin->setRoute(admin->getRoute());
    newAdmin->setValue(admin->getValue());

    return newAdmin;
}

ezechiel::core::Dosage* PredictionSpecCopier::copy(ezechiel::core::Dosage* dosage, QObject *parent)
{
    ezechiel::core::Dosage *newDosage;
    newDosage = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Dosage>(ABSTRACTREPO,parent);
    newDosage->setApplied(dosage->getApplied());
    newDosage->setDbinterval(dosage->getDbinterval());
    newDosage->setDbtinf(dosage->getDbtinf());
    newDosage->setEndTime(dosage->getEndTime());
    newDosage->setInterval(dosage->getInterval());
    newDosage->setQuantity(copy(dosage->getQuantity(), newDosage));
    newDosage->setRoute(copy(dosage->getRoute(), newDosage));
    newDosage->setTinf(dosage->getTinf());
    newDosage->setIsAtSteadyState(dosage->getIsAtSteadyState());
    newDosage->setSteadyStateLastDoseDate(dosage->getSteadyStateLastDoseDate());

    return newDosage;
}

ezechiel::core::DosageHistory* PredictionSpecCopier::copy(ezechiel::core::DosageHistory* history, QObject *parent)
{
    ezechiel::core::DosageHistory *newHistory;
    newHistory = ezechiel::core::CoreFactory::createEntity<ezechiel::core::DosageHistory>(ABSTRACTREPO,parent);
    foreach(ezechiel::core::Dosage* dosage, history->getList())
        newHistory->append(copy(dosage, newHistory));

    return newHistory;
}


ezechiel::core::AdminList* PredictionSpecCopier::copy(ezechiel::core::AdminList* list, QObject *parent)
{
    ezechiel::core::AdminList *newList;
    newList = ezechiel::core::CoreFactory::createEntity<ezechiel::core::AdminList>(ABSTRACTREPO,parent);
    foreach(ezechiel::core::Admin* admin, list->getList())
        newList->append(copy(admin, newList));

    return newList;
}

ezechiel::core::DrugVariateList* PredictionSpecCopier::copy(ezechiel::core::DrugVariateList* list, QObject *parent)
{
    ezechiel::core::DrugVariateList *newList;
    newList = ezechiel::core::CoreFactory::createEntity<ezechiel::core::DrugVariateList>(ABSTRACTREPO,parent);
    foreach(ezechiel::core::DrugVariate* variate, list->getList())
        newList->append(copy(variate, newList));

    return newList;
}

ezechiel::core::Concentrations* PredictionSpecCopier::copy(ezechiel::core::Concentrations* concentrations, QObject *parent)
{
    ezechiel::core::Concentrations *newConcentrations;
    newConcentrations = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Concentrations>(ABSTRACTREPO,parent);
    newConcentrations->setQuantity(concentrations->getQuantity());

    return newConcentrations;
}

ezechiel::core::DrugVariate* PredictionSpecCopier::copy(ezechiel::core::DrugVariate* variate, QObject *parent)
{
    ezechiel::core::DrugVariate *newVariate;
    newVariate = ezechiel::core::CoreFactory::createEntity<ezechiel::core::DrugVariate>(ABSTRACTREPO,parent);
    newVariate->setQuantity(copy(variate->getQuantity(), newVariate));
    newVariate->setCovariateId(variate->getCovariateId());
    newVariate->setType(variate->getType());
//    newVariate->setUnitString(variate->getUnitString());

    return newVariate;
}

ezechiel::core::ADME* PredictionSpecCopier::copy(ezechiel::core::ADME* adme, QObject *parent)
{
    ezechiel::core::ADME *newAdme;
    newAdme = ezechiel::core::CoreFactory::createEntity<ezechiel::core::ADME>(ABSTRACTREPO,parent);
    newAdme->setDefaultIntake(copy(adme->getDefaultIntake(), newAdme));
    newAdme->setDistribution(adme->getDistribution());
    newAdme->setElimination(adme->getElimination());
    newAdme->setIntakes(copy(adme->getIntakes(), newAdme));

    return newAdme;
}

ezechiel::core::DrugModel* PredictionSpecCopier::copy(ezechiel::core::DrugModel *drug, QObject *parent)
{
    ezechiel::core::DrugModel *newDrug;
    newDrug = ezechiel::core::CoreFactory::createEntity<ezechiel::core::DrugModel>(ABSTRACTREPO,parent);
    newDrug->setAdme(copy(drug->getAdme(), newDrug));
    newDrug->setActiveSubstance(copy(drug->getActiveSubstance(), newDrug));
    newDrug->setConcentrations(copy(drug->getConcentrations(), newDrug));
    newDrug->setCovariates(copy(drug->getCovariates(), newDrug));
    newDrug->setStandardTreatment(copy(drug->getStandardTreatment(), newDrug));
    newDrug->setDoses(copy(drug->getDoses(), newDrug));
    newDrug->setDrugModelId(drug->getDrugModelId());
    newDrug->setErrorModel(copy(drug->getErrorModel(), newDrug));
    newDrug->setHalflife(copy(drug->getHalflife(), newDrug));
    newDrug->setInfusions(copy(drug->getInfusions(), newDrug));
    newDrug->setIntervals(copy(drug->getIntervals(), newDrug));
    newDrug->setMolarMass(drug->getMolarMass());
    newDrug->setPkModelId(drug->getPkModelId());
    newDrug->setTargets(copy(drug->getTargets(), newDrug));
    newDrug->setParameters(copy(drug->getParameters(), newDrug));

    return newDrug;

}


ezechiel::core::StandardTreatment* PredictionSpecCopier::copy(ezechiel::core::StandardTreatment* treatment, QObject *parent)
{
    if (treatment == nullptr)
        return nullptr;

    ezechiel::core::StandardTreatment *newTreatment;
    newTreatment = ezechiel::core::CoreFactory::createEntity<ezechiel::core::StandardTreatment>(ABSTRACTREPO,parent);
    newTreatment->setIsFixedDuration(treatment->getIsFixedDuration());
    newTreatment->setDuration(treatment->getDuration());
    return newTreatment;
}

ezechiel::core::ActiveSubstance* PredictionSpecCopier::copy(ezechiel::core::ActiveSubstance *drug, QObject *parent)
{
    if (drug == nullptr)
        return nullptr;

    ezechiel::core::ActiveSubstance *newDrug;
    newDrug = ezechiel::core::CoreFactory::createEntity<ezechiel::core::ActiveSubstance>(ABSTRACTREPO,parent);
    newDrug->setAtc(drug->getAtc());
    newDrug->setBrands(drug->getBrands());
    newDrug->setSubstanceId(drug->getSubstanceId());
//    newDrug->setMolarMass(drug->getMolarMass());

    return newDrug;

}

ezechiel::core::DrugTreatment* PredictionSpecCopier::copy(ezechiel::core::DrugTreatment *treatment, QObject *parent)
{
    ezechiel::core::DrugTreatment *newTreatment;
    newTreatment = ezechiel::core::CoreFactory::createEntity<ezechiel::core::DrugTreatment>(ABSTRACTREPO,parent);
    newTreatment->setCovariates(copy(treatment->getCovariates(), newTreatment));
    newTreatment->setDosages(copy(treatment->getDosages(), newTreatment));
    newTreatment->setAdjustments(copy(treatment->getAdjustments(), newTreatment));
    newTreatment->setActiveSubstanceId(treatment->getActiveSubstanceId());
    newTreatment->setMeasures(copy(treatment->getMeasures(), newTreatment));
    newTreatment->setPatient(copy(treatment->getPatient(), newTreatment));
    newTreatment->setTargets(copy(treatment->getTargets(), newTreatment));

    return newTreatment;
}



ezechiel::core::ValidDoses* PredictionSpecCopier::copy(ezechiel::core::ValidDoses* list, QObject *parent)
{
    ezechiel::core::ValidDoses *newList;
    newList = ezechiel::core::CoreFactory::createEntity<ezechiel::core::ValidDoses>(ABSTRACTREPO,parent);
    newList->setAnyDose(list->getAnyDose());
    newList->setQuantity(copy(list->getQuantity(), newList));
    newList->setFromDose(copy(list->getFromDose(), newList));
    newList->setToDose(copy(list->getToDose(), newList));
    newList->setStepDose(copy(list->getStepDose(), newList));
    for(int i=0;i<list->size();i++)
        newList->append(copy(list->at(i), newList));

    return newList;
}


ezechiel::core::ErrorModel* PredictionSpecCopier::copy(ezechiel::core::ErrorModel *model, QObject *parent)
{
    ezechiel::core::ErrorModel *newModel;
    newModel = ezechiel::core::CoreFactory::createEntity<ezechiel::core::ErrorModel>(ABSTRACTREPO,parent);
    newModel->setAdditive(model->getAdditive());
    newModel->setProportional(model->getProportional());

    return newModel;
}


ezechiel::core::Halflife* PredictionSpecCopier::copy(ezechiel::core::Halflife *halflife, QObject *parent)
{
    ezechiel::core::Halflife *newHalflife;
    newHalflife = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Halflife>(ABSTRACTREPO,parent);
    newHalflife->setMultiplier(halflife->getMultiplier());
    newHalflife->setUnit(halflife->getUnit());
    newHalflife->setValue(halflife->getValue());

    return newHalflife;
}


ezechiel::core::ValidInfusion* PredictionSpecCopier::copy(ezechiel::core::ValidInfusion *source, QObject *parent)
{
    ezechiel::core::ValidInfusion *target;
    target = ezechiel::core::CoreFactory::createEntity<ezechiel::core::ValidInfusion>(ABSTRACTREPO,parent);
    target->setQuantity(copy(source->getQuantity(), target));

    return target;
}

ezechiel::core::ValidInfusions* PredictionSpecCopier::copy(ezechiel::core::ValidInfusions *source, QObject *parent)
{
    ezechiel::core::ValidInfusions *target;
    target = ezechiel::core::CoreFactory::createEntity<ezechiel::core::ValidInfusions>(ABSTRACTREPO,parent);
    target->setAny(source->getAny());
    target->setQuantity(copy(source->getQuantity(), target));
    for(int i=0;i<source->size();i++)
        target->append(copy(source->at(i), target));

    return target;
}


ezechiel::core::ValidInterval* PredictionSpecCopier::copy(ezechiel::core::ValidInterval *source, QObject *parent)
{
    ezechiel::core::ValidInterval *target;
    target = ezechiel::core::CoreFactory::createEntity<ezechiel::core::ValidInterval>(ABSTRACTREPO,parent);
    target->setQuantity(copy(source->getQuantity(), target));

    return target;
}

ezechiel::core::ValidIntervals* PredictionSpecCopier::copy(ezechiel::core::ValidIntervals *source, QObject *parent)
{
    ezechiel::core::ValidIntervals *target;
    target = ezechiel::core::CoreFactory::createEntity<ezechiel::core::ValidIntervals>(ABSTRACTREPO,parent);
    target->setAny(source->getAny());
    target->setQuantity(copy(source->getQuantity(), target));
    for(int i=0;i<source->size();i++)
        target->append(copy(source->at(i), target));

    return target;
}



ezechiel::core::Target* PredictionSpecCopier::copy(ezechiel::core::Target* source, QObject *parent)
{
    ezechiel::core::Target *target;
    target = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Target>(ABSTRACTREPO,parent);
    target->setCbest(copy(source->getCbest(), target));
    target->setCmax(copy(source->getCmax(), target));
    target->setCmin(copy(source->getCmin(), target));
    target->setTbest(copy(source->getTbest(), target));
    target->setTmax(copy(source->getTmax(), target));
    target->setTmin(copy(source->getTmin(), target));
    target->setMic(copy(source->getMic(), target));
    target->setType(copy(source->getType(), target));

    return target;
}


ezechiel::core::TargetList* PredictionSpecCopier::copy(ezechiel::core::TargetList* source, QObject *parent)
{
    ezechiel::core::TargetList *target;
    target = ezechiel::core::CoreFactory::createEntity<ezechiel::core::TargetList>(ABSTRACTREPO,parent);
    foreach(ezechiel::core::Target* t, source->getList())
        target->append(copy(t, target));

    return target;
}


ezechiel::core::CorePatient* PredictionSpecCopier::copy(ezechiel::core::CorePatient* source, QObject *parent)
{
    ezechiel::core::CorePatient *target;
    target = ezechiel::core::CoreFactory::createEntity<ezechiel::core::CorePatient>(ABSTRACTREPO,parent);
    // No field in corepatient...

    return target;
}

ezechiel::core::CoreMeasureList* PredictionSpecCopier::copy(ezechiel::core::CoreMeasureList* source, QObject *parent)
{
    ezechiel::core::CoreMeasureList *target;
    target = ezechiel::core::CoreFactory::createEntity<ezechiel::core::CoreMeasureList>(ABSTRACTREPO,parent);
    foreach(ezechiel::core::CoreMeasure* t, source->getList())
        target->append(copy(t, target));

    return target;
}

ezechiel::core::CoreMeasure* PredictionSpecCopier::copy(ezechiel::core::CoreMeasure* source, QObject *parent)
{
    ezechiel::core::CoreMeasure *target;
    target = ezechiel::core::CoreFactory::createEntity<ezechiel::core::CoreMeasure>(ABSTRACTREPO,parent);
    target->setConcentration(copy(source->getConcentration(), target));
    target->setSdrug(source->getSdrug());
    target->setMoment(source->getMoment());

    return target;

}


ezechiel::core::ValidDose* PredictionSpecCopier::copy(ezechiel::core::ValidDose* source, QObject *parent)
{
    ezechiel::core::ValidDose *target;
    target = ezechiel::core::CoreFactory::createEntity<ezechiel::core::ValidDose>(ABSTRACTREPO,parent);
    target->setQuantity(copy(source->getQuantity(), target));
    target->setRoute(copy(source->getRoute(), target));

    return target;

}

ezechiel::core::TargetMethod* PredictionSpecCopier::copy(ezechiel::core::TargetMethod* source, QObject *parent)
{
    ezechiel::core::TargetMethod *target;
    target = ezechiel::core::CoreFactory::createEntity<ezechiel::core::TargetMethod>(ABSTRACTREPO,parent);
    target->setTargetType(source->getTargetType());
    target->setValue(source->getValue());

    return target;

}


ezechiel::core::OperationList *PredictionSpecCopier::copy(ezechiel::core::OperationList *source, QObject *parent)
{
    ezechiel::core::OperationList *target;
    target = ezechiel::core::CoreFactory::createEntity<ezechiel::core::OperationList>(ABSTRACTREPO,parent);
    foreach(ezechiel::core::Operation* t, source->getList())
        target->append(copy(t, target));

    return target;
}


ezechiel::core::Operation *PredictionSpecCopier::copy(ezechiel::core::Operation *source, QObject *parent)
{
    ezechiel::core::Operation *target;
    target = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Operation>(ABSTRACTREPO,parent);
    target->setFormula(source->getFormula());
    target->setType(source->getType());

    return target;
}


ezechiel::core::ParameterSet *PredictionSpecCopier::copy(ezechiel::core::ParameterSet *source, QObject *parent)
{
    ezechiel::core::ParameterSet *target;
    target = ezechiel::core::CoreFactory::createEntity<ezechiel::core::ParameterSet>(ABSTRACTREPO,parent);
    target->setTime(source->getTime());
    target->setType(source->type());
    for(int i=0;i<source->getParameters()->getList().size();i++)
        target->set(copy(source->getParameters()->getList().at(i), target));

    target->copyCorrelationsFrom(*source);

    // Attention à setType qui n'a pas de setType()

    return target;
}

ezechiel::core::Parameters *PredictionSpecCopier::copy(ezechiel::core::Parameters *source, QObject *parent)
{
    return source;
    ezechiel::core::Parameters *target;
    target = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Parameters>(ABSTRACTREPO,parent);
    foreach(ezechiel::core::Parameter* t, source->getList())
        target->append(copy(t, target));

    return target;
}

ezechiel::core::Parameter *PredictionSpecCopier::copy(ezechiel::core::Parameter *source, QObject *parent)
{
    ezechiel::core::Parameter *target;
    target = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Parameter>(ABSTRACTREPO,parent);
    target->setBsv(copy(source->getBsv(), target));
    target->setMax(source->getMax());
    target->setMin(source->getMin());
    target->setName(source->getName());
    if (source->getOperations() != nullptr) {
        target->setOperations(copy(source->getOperations(), target));
    }
    target->setQuantity(copy(source->getQuantity(), target));
    target->setStep(source->getStep());

    return target;
}

ezechiel::core::Bsv *PredictionSpecCopier::copy(ezechiel::core::Bsv *source, QObject *parent)
{
    ezechiel::core::Bsv *target;
    target = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Bsv>(ABSTRACTREPO,parent);
    target->setProportional(source->getProportional());
    target->setStandard(source->getStandard());

    return target;
}
