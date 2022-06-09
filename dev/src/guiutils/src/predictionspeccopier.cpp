//@@license@@

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

ezechiel::GuiCore::PredictionSpec *PredictionSpecCopier::copy(ezechiel::GuiCore::PredictionSpec *spec, QObject *parent)
{
    ezechiel::GuiCore::PredictionSpec *newSpec;
    newSpec = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::PredictionSpec>(ABSTRACTREPO,parent);
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
    newSpec->setClearCache(spec->getClearCache());

    return newSpec;
}


ezechiel::GuiCore::DrugResponseAnalysis *PredictionSpecCopier::copy(ezechiel::GuiCore::DrugResponseAnalysis *analysis, QObject *parent)
{
    ezechiel::GuiCore::DrugResponseAnalysis *newAnalysis;
    newAnalysis = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::DrugResponseAnalysis>(ABSTRACTREPO,parent);

    newAnalysis->setTreatment(copy(analysis->getTreatment(), newAnalysis));
    newAnalysis->setDrugModel(copy(analysis->getDrugModel(), newAnalysis));

    return newAnalysis;
}


ezechiel::GuiCore::IdentifiableAmount *PredictionSpecCopier::copy(ezechiel::GuiCore::IdentifiableAmount *amount, QObject *parent)
{
    ezechiel::GuiCore::IdentifiableAmount *newAmount;
    newAmount = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::IdentifiableAmount>(ABSTRACTREPO,parent);

    newAmount->setAmountId(amount->getAmountId());
    newAmount->setDbvalue(amount->getDbvalue());
    newAmount->setUnit(amount->unit());
    newAmount->setValue(amount->value());

    return newAmount;
}

ezechiel::GuiCore::OperableAmount *PredictionSpecCopier::copy(ezechiel::GuiCore::OperableAmount *amount, QObject *parent)
{
    ezechiel::GuiCore::OperableAmount *newAmount;
    newAmount = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::OperableAmount>(ABSTRACTREPO,parent);

    newAmount->setAmountId(amount->getAmountId());
    newAmount->setDbvalue(amount->getDbvalue());
    newAmount->setFixed(amount->getFixed());
    newAmount->setOperations(copy(amount->getOperations(), newAmount));
    newAmount->setUnit(amount->unit());
    newAmount->setValue(amount->value());
    // Something strange with OperableAmount. To be checked

    return newAmount;
}


ezechiel::GuiCore::PatientVariate* PredictionSpecCopier::copy(ezechiel::GuiCore::PatientVariate* variate, QObject *parent)
{
    ezechiel::GuiCore::PatientVariate *newVariate;
    newVariate = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::PatientVariate>(ABSTRACTREPO,parent);
    // To be completed
    newVariate->setDate(variate->getDate());
    newVariate->setForced(variate->getForced());
    newVariate->setCovariateId(variate->getCovariateId());
    newVariate->setQuantity(copy(variate->getQuantity(), newVariate));
    newVariate->setType(variate->getType());
    newVariate->setValueAsString(variate->getValueAsString());

    return newVariate;
}

ezechiel::GuiCore::PatientVariateList* PredictionSpecCopier::copy(ezechiel::GuiCore::PatientVariateList* list, QObject *parent)
{
    ezechiel::GuiCore::PatientVariateList *newList;
    newList = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::PatientVariateList>(ABSTRACTREPO,parent);
    foreach(ezechiel::GuiCore::PatientVariate *variate, list->getList())
        newList->append(copy(variate, newList));

    return newList;
}


ezechiel::GuiCore::Admin* PredictionSpecCopier::copy(ezechiel::GuiCore::Admin* admin, QObject *parent)
{
    ezechiel::GuiCore::Admin *newAdmin;
    newAdmin = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Admin>(ABSTRACTREPO,parent);
    newAdmin->setRoute(admin->getRoute());
    newAdmin->setValue(admin->getValue());
    newAdmin->setFormulationAndRoute(admin->getFormulationAndRoute());

    return newAdmin;
}

ezechiel::GuiCore::Dosage* PredictionSpecCopier::copy(ezechiel::GuiCore::Dosage* dosage, QObject *parent)
{
    ezechiel::GuiCore::Dosage *newDosage;
    newDosage = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Dosage>(ABSTRACTREPO,parent);
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

ezechiel::GuiCore::DosageHistory* PredictionSpecCopier::copy(ezechiel::GuiCore::DosageHistory* history, QObject *parent)
{
    ezechiel::GuiCore::DosageHistory *newHistory;
    newHistory = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::DosageHistory>(ABSTRACTREPO,parent);
    foreach(ezechiel::GuiCore::Dosage* dosage, history->getList())
        newHistory->append(copy(dosage, newHistory));

    return newHistory;
}


ezechiel::GuiCore::AdminList* PredictionSpecCopier::copy(ezechiel::GuiCore::AdminList* list, QObject *parent)
{
    ezechiel::GuiCore::AdminList *newList;
    newList = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::AdminList>(ABSTRACTREPO,parent);
    foreach(ezechiel::GuiCore::Admin* admin, list->getList())
        newList->append(copy(admin, newList));

    return newList;
}

ezechiel::GuiCore::DrugVariateList* PredictionSpecCopier::copy(ezechiel::GuiCore::DrugVariateList* list, QObject *parent)
{
    ezechiel::GuiCore::DrugVariateList *newList;
    newList = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::DrugVariateList>(ABSTRACTREPO,parent);
    foreach(ezechiel::GuiCore::DrugVariate* variate, list->getList())
        newList->append(copy(variate, newList));

    return newList;
}

ezechiel::GuiCore::Concentrations* PredictionSpecCopier::copy(ezechiel::GuiCore::Concentrations* concentrations, QObject *parent)
{
    ezechiel::GuiCore::Concentrations *newConcentrations;
    newConcentrations = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Concentrations>(ABSTRACTREPO,parent);
    newConcentrations->setQuantity(concentrations->getQuantity());

    return newConcentrations;
}

ezechiel::GuiCore::DrugVariate* PredictionSpecCopier::copy(ezechiel::GuiCore::DrugVariate* variate, QObject *parent)
{
    ezechiel::GuiCore::DrugVariate *newVariate;
    newVariate = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::DrugVariate>(ABSTRACTREPO,parent);
    newVariate->setQuantity(copy(variate->getQuantity(), newVariate));
    newVariate->setCovariateId(variate->getCovariateId());
    newVariate->setType(variate->getType());
//    newVariate->setUnitString(variate->getUnitString());

    return newVariate;
}

ezechiel::GuiCore::ADME* PredictionSpecCopier::copy(ezechiel::GuiCore::ADME* adme, QObject *parent)
{
    ezechiel::GuiCore::ADME *newAdme;
    newAdme = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ADME>(ABSTRACTREPO,parent);
    newAdme->setDefaultIntake(copy(adme->getDefaultIntake(), newAdme));
    newAdme->setDistribution(adme->getDistribution());
    newAdme->setElimination(adme->getElimination());
    newAdme->setIntakes(copy(adme->getIntakes(), newAdme));

    return newAdme;
}

ezechiel::GuiCore::DrugModel* PredictionSpecCopier::copy(ezechiel::GuiCore::DrugModel *drug, QObject *parent)
{
    ezechiel::GuiCore::DrugModel *newDrug;
    newDrug = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::DrugModel>(ABSTRACTREPO,parent);
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


ezechiel::GuiCore::StandardTreatment* PredictionSpecCopier::copy(ezechiel::GuiCore::StandardTreatment* treatment, QObject *parent)
{
    if (treatment == nullptr)
        return nullptr;

    ezechiel::GuiCore::StandardTreatment *newTreatment;
    newTreatment = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::StandardTreatment>(ABSTRACTREPO,parent);
    newTreatment->setIsFixedDuration(treatment->getIsFixedDuration());
    newTreatment->setDuration(treatment->getDuration());
    return newTreatment;
}

ezechiel::GuiCore::ActiveSubstance* PredictionSpecCopier::copy(ezechiel::GuiCore::ActiveSubstance *drug, QObject *parent)
{
    if (drug == nullptr)
        return nullptr;

    ezechiel::GuiCore::ActiveSubstance *newDrug;
    newDrug = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ActiveSubstance>(ABSTRACTREPO,parent);
    newDrug->setAtc(drug->getAtc());
    newDrug->setBrands(drug->getBrands());
    newDrug->setSubstanceId(drug->getSubstanceId());
//    newDrug->setMolarMass(drug->getMolarMass());

    return newDrug;

}

ezechiel::GuiCore::DrugTreatment* PredictionSpecCopier::copy(ezechiel::GuiCore::DrugTreatment *treatment, QObject *parent)
{
    ezechiel::GuiCore::DrugTreatment *newTreatment;
    newTreatment = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::DrugTreatment>(ABSTRACTREPO,parent);
    newTreatment->setCovariates(copy(treatment->getCovariates(), newTreatment));
    newTreatment->setDosages(copy(treatment->getDosages(), newTreatment));
    newTreatment->setAdjustments(copy(treatment->getAdjustments(), newTreatment));
    newTreatment->setActiveSubstanceId(treatment->getActiveSubstanceId());
    newTreatment->setMeasures(copy(treatment->getMeasures(), newTreatment));
    newTreatment->setPatient(copy(treatment->getPatient(), newTreatment));
    newTreatment->setTargets(copy(treatment->getTargets(), newTreatment));

    return newTreatment;
}



ezechiel::GuiCore::ValidDoses* PredictionSpecCopier::copy(ezechiel::GuiCore::ValidDoses* list, QObject *parent)
{
    ezechiel::GuiCore::ValidDoses *newList;
    newList = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ValidDoses>(ABSTRACTREPO,parent);
    newList->setAnyDose(list->getAnyDose());
    newList->setQuantity(copy(list->getQuantity(), newList));
    newList->setFromDose(copy(list->getFromDose(), newList));
    newList->setToDose(copy(list->getToDose(), newList));
    newList->setStepDose(copy(list->getStepDose(), newList));
    for(int i=0;i<list->size();i++)
        newList->append(copy(list->at(i), newList));

    return newList;
}


ezechiel::GuiCore::ErrorModel* PredictionSpecCopier::copy(ezechiel::GuiCore::ErrorModel *model, QObject *parent)
{
    ezechiel::GuiCore::ErrorModel *newModel;
    newModel = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ErrorModel>(ABSTRACTREPO,parent);
    newModel->setAdditive(model->getAdditive());
    newModel->setProportional(model->getProportional());

    return newModel;
}


ezechiel::GuiCore::Halflife* PredictionSpecCopier::copy(ezechiel::GuiCore::Halflife *halflife, QObject *parent)
{
    ezechiel::GuiCore::Halflife *newHalflife;
    newHalflife = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Halflife>(ABSTRACTREPO,parent);
    newHalflife->setMultiplier(halflife->getMultiplier());
    newHalflife->setUnit(halflife->getUnit());
    newHalflife->setValue(halflife->getValue());

    return newHalflife;
}


ezechiel::GuiCore::ValidInfusion* PredictionSpecCopier::copy(ezechiel::GuiCore::ValidInfusion *source, QObject *parent)
{
    ezechiel::GuiCore::ValidInfusion *target;
    target = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ValidInfusion>(ABSTRACTREPO,parent);
    target->setQuantity(copy(source->getQuantity(), target));

    return target;
}

ezechiel::GuiCore::ValidInfusions* PredictionSpecCopier::copy(ezechiel::GuiCore::ValidInfusions *source, QObject *parent)
{
    ezechiel::GuiCore::ValidInfusions *target;
    target = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ValidInfusions>(ABSTRACTREPO,parent);
    target->setAny(source->getAny());
    target->setQuantity(copy(source->getQuantity(), target));
    for(int i=0;i<source->size();i++)
        target->append(copy(source->at(i), target));

    return target;
}


ezechiel::GuiCore::ValidInterval* PredictionSpecCopier::copy(ezechiel::GuiCore::ValidInterval *source, QObject *parent)
{
    ezechiel::GuiCore::ValidInterval *target;
    target = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ValidInterval>(ABSTRACTREPO,parent);
    target->setQuantity(copy(source->getQuantity(), target));

    return target;
}

ezechiel::GuiCore::ValidIntervals* PredictionSpecCopier::copy(ezechiel::GuiCore::ValidIntervals *source, QObject *parent)
{
    ezechiel::GuiCore::ValidIntervals *target;
    target = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ValidIntervals>(ABSTRACTREPO,parent);
    target->setAny(source->getAny());
    target->setQuantity(copy(source->getQuantity(), target));
    for(int i=0;i<source->size();i++)
        target->append(copy(source->at(i), target));

    return target;
}



ezechiel::GuiCore::Target* PredictionSpecCopier::copy(ezechiel::GuiCore::Target* source, QObject *parent)
{
    ezechiel::GuiCore::Target *target;
    target = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Target>(ABSTRACTREPO,parent);
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


ezechiel::GuiCore::TargetList* PredictionSpecCopier::copy(ezechiel::GuiCore::TargetList* source, QObject *parent)
{
    ezechiel::GuiCore::TargetList *target;
    target = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::TargetList>(ABSTRACTREPO,parent);
    foreach(ezechiel::GuiCore::Target* t, source->getList())
        target->append(copy(t, target));

    return target;
}


ezechiel::GuiCore::CorePatient* PredictionSpecCopier::copy(ezechiel::GuiCore::CorePatient* source, QObject *parent)
{
    ezechiel::GuiCore::CorePatient *target;
    target = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::CorePatient>(ABSTRACTREPO,parent);
    // No field in corepatient...

    return target;
}

ezechiel::GuiCore::CoreMeasureList* PredictionSpecCopier::copy(ezechiel::GuiCore::CoreMeasureList* source, QObject *parent)
{
    ezechiel::GuiCore::CoreMeasureList *target;
    target = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::CoreMeasureList>(ABSTRACTREPO,parent);
    foreach(ezechiel::GuiCore::CoreMeasure* t, source->getList())
        target->append(copy(t, target));

    return target;
}

ezechiel::GuiCore::CoreMeasure* PredictionSpecCopier::copy(ezechiel::GuiCore::CoreMeasure* source, QObject *parent)
{
    ezechiel::GuiCore::CoreMeasure *target;
    target = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::CoreMeasure>(ABSTRACTREPO,parent);
    target->setConcentration(copy(source->getConcentration(), target));
    target->setSdrug(source->getSdrug());
    target->setMoment(source->getMoment());

    return target;

}


ezechiel::GuiCore::ValidDose* PredictionSpecCopier::copy(ezechiel::GuiCore::ValidDose* source, QObject *parent)
{
    ezechiel::GuiCore::ValidDose *target;
    target = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ValidDose>(ABSTRACTREPO,parent);
    target->setQuantity(copy(source->getQuantity(), target));
    target->setRoute(copy(source->getRoute(), target));

    return target;

}

ezechiel::GuiCore::TargetMethod* PredictionSpecCopier::copy(ezechiel::GuiCore::TargetMethod* source, QObject *parent)
{
    ezechiel::GuiCore::TargetMethod *target;
    target = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::TargetMethod>(ABSTRACTREPO,parent);
    target->setTargetType(source->getTargetType());
    target->setValue(source->getValue());

    return target;

}


ezechiel::GuiCore::OperationList *PredictionSpecCopier::copy(ezechiel::GuiCore::OperationList *source, QObject *parent)
{
    ezechiel::GuiCore::OperationList *target;
    target = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::OperationList>(ABSTRACTREPO,parent);
    foreach(ezechiel::GuiCore::Operation* t, source->getList())
        target->append(copy(t, target));

    return target;
}


ezechiel::GuiCore::Operation *PredictionSpecCopier::copy(ezechiel::GuiCore::Operation *source, QObject *parent)
{
    ezechiel::GuiCore::Operation *target;
    target = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Operation>(ABSTRACTREPO,parent);
    target->setFormula(source->getFormula());
    target->setType(source->getType());

    return target;
}


ezechiel::GuiCore::ParameterSet *PredictionSpecCopier::copy(ezechiel::GuiCore::ParameterSet *source, QObject *parent)
{
    ezechiel::GuiCore::ParameterSet *target;
    target = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::ParameterSet>(ABSTRACTREPO,parent);
    target->setTime(source->getTime());
    target->setType(source->type());
    for(int i=0;i<source->getParameters()->getList().size();i++)
        target->set(copy(source->getParameters()->getList().at(i), target));

    target->copyCorrelationsFrom(*source);

    // Attention à setType qui n'a pas de setType()

    return target;
}

ezechiel::GuiCore::Parameters *PredictionSpecCopier::copy(ezechiel::GuiCore::Parameters *source, QObject *parent)
{
    return source;
    ezechiel::GuiCore::Parameters *target;
    target = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Parameters>(ABSTRACTREPO,parent);
    foreach(ezechiel::GuiCore::Parameter* t, source->getList())
        target->append(copy(t, target));

    return target;
}

ezechiel::GuiCore::Parameter *PredictionSpecCopier::copy(ezechiel::GuiCore::Parameter *source, QObject *parent)
{
    ezechiel::GuiCore::Parameter *target;
    target = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Parameter>(ABSTRACTREPO,parent);
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

ezechiel::GuiCore::Bsv *PredictionSpecCopier::copy(ezechiel::GuiCore::Bsv *source, QObject *parent)
{
    ezechiel::GuiCore::Bsv *target;
    target = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::Bsv>(ABSTRACTREPO,parent);
    target->setProportional(source->getProportional());
    target->setStandard(source->getStandard());

    return target;
}
