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

Tucuxi::GuiCore::PredictionSpec *PredictionSpecCopier::copy(Tucuxi::GuiCore::PredictionSpec *spec, QObject *parent)
{
    Tucuxi::GuiCore::PredictionSpec *newSpec;
    newSpec = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::PredictionSpec>(ABSTRACTREPO,parent);
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


Tucuxi::GuiCore::DrugResponseAnalysis *PredictionSpecCopier::copy(Tucuxi::GuiCore::DrugResponseAnalysis *analysis, QObject *parent)
{
    Tucuxi::GuiCore::DrugResponseAnalysis *newAnalysis;
    newAnalysis = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::DrugResponseAnalysis>(ABSTRACTREPO,parent);

    newAnalysis->setTreatment(copy(analysis->getTreatment(), newAnalysis));
    newAnalysis->setDrugModel(copy(analysis->getDrugModel(), newAnalysis));

    return newAnalysis;
}


Tucuxi::GuiCore::IdentifiableAmount *PredictionSpecCopier::copy(Tucuxi::GuiCore::IdentifiableAmount *amount, QObject *parent)
{
    Tucuxi::GuiCore::IdentifiableAmount *newAmount;
    newAmount = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::IdentifiableAmount>(ABSTRACTREPO,parent);

    newAmount->setAmountId(amount->getAmountId());
    newAmount->setDbvalue(amount->getDbvalue());
    newAmount->setUnit(amount->unit());
    newAmount->setValue(amount->value());

    return newAmount;
}

Tucuxi::GuiCore::OperableAmount *PredictionSpecCopier::copy(Tucuxi::GuiCore::OperableAmount *amount, QObject *parent)
{
    Tucuxi::GuiCore::OperableAmount *newAmount;
    newAmount = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::OperableAmount>(ABSTRACTREPO,parent);

    newAmount->setAmountId(amount->getAmountId());
    newAmount->setDbvalue(amount->getDbvalue());
    newAmount->setFixed(amount->getFixed());
    newAmount->setOperations(copy(amount->getOperations(), newAmount));
    newAmount->setUnit(amount->unit());
    newAmount->setValue(amount->value());
    // Something strange with OperableAmount. To be checked

    return newAmount;
}


Tucuxi::GuiCore::PatientVariate* PredictionSpecCopier::copy(Tucuxi::GuiCore::PatientVariate* variate, QObject *parent)
{
    Tucuxi::GuiCore::PatientVariate *newVariate;
    newVariate = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::PatientVariate>(ABSTRACTREPO,parent);
    // To be completed
    newVariate->setDate(variate->getDate());
    newVariate->setForced(variate->getForced());
    newVariate->setCovariateId(variate->getCovariateId());
    newVariate->setQuantity(copy(variate->getQuantity(), newVariate));
    newVariate->setType(variate->getType());
    newVariate->setValueAsString(variate->getValueAsString());

    return newVariate;
}

Tucuxi::GuiCore::PatientVariateList* PredictionSpecCopier::copy(Tucuxi::GuiCore::PatientVariateList* list, QObject *parent)
{
    Tucuxi::GuiCore::PatientVariateList *newList;
    newList = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::PatientVariateList>(ABSTRACTREPO,parent);
    foreach(Tucuxi::GuiCore::PatientVariate *variate, list->getList())
        newList->append(copy(variate, newList));

    return newList;
}


Tucuxi::GuiCore::Admin* PredictionSpecCopier::copy(Tucuxi::GuiCore::Admin* admin, QObject *parent)
{
    Tucuxi::GuiCore::Admin *newAdmin;
    newAdmin = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::Admin>(ABSTRACTREPO,parent);
    newAdmin->setRoute(admin->getRoute());
    newAdmin->setValue(admin->getValue());
    newAdmin->setFormulationAndRoute(admin->getFormulationAndRoute());

    return newAdmin;
}

Tucuxi::GuiCore::Dosage* PredictionSpecCopier::copy(Tucuxi::GuiCore::Dosage* dosage, QObject *parent)
{
    Tucuxi::GuiCore::Dosage *newDosage;
    newDosage = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::Dosage>(ABSTRACTREPO,parent);
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

Tucuxi::GuiCore::DosageHistory* PredictionSpecCopier::copy(Tucuxi::GuiCore::DosageHistory* history, QObject *parent)
{
    Tucuxi::GuiCore::DosageHistory *newHistory;
    newHistory = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::DosageHistory>(ABSTRACTREPO,parent);
    foreach(Tucuxi::GuiCore::Dosage* dosage, history->getList())
        newHistory->append(copy(dosage, newHistory));

    return newHistory;
}


Tucuxi::GuiCore::AdminList* PredictionSpecCopier::copy(Tucuxi::GuiCore::AdminList* list, QObject *parent)
{
    Tucuxi::GuiCore::AdminList *newList;
    newList = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::AdminList>(ABSTRACTREPO,parent);
    foreach(Tucuxi::GuiCore::Admin* admin, list->getList())
        newList->append(copy(admin, newList));

    return newList;
}

Tucuxi::GuiCore::DrugVariateList* PredictionSpecCopier::copy(Tucuxi::GuiCore::DrugVariateList* list, QObject *parent)
{
    Tucuxi::GuiCore::DrugVariateList *newList;
    newList = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::DrugVariateList>(ABSTRACTREPO,parent);
    foreach(Tucuxi::GuiCore::DrugVariate* variate, list->getList())
        newList->append(copy(variate, newList));

    return newList;
}

Tucuxi::GuiCore::Concentrations* PredictionSpecCopier::copy(Tucuxi::GuiCore::Concentrations* concentrations, QObject *parent)
{
    Tucuxi::GuiCore::Concentrations *newConcentrations;
    newConcentrations = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::Concentrations>(ABSTRACTREPO,parent);
    newConcentrations->setQuantity(concentrations->getQuantity());

    return newConcentrations;
}

Tucuxi::GuiCore::DrugVariate* PredictionSpecCopier::copy(Tucuxi::GuiCore::DrugVariate* variate, QObject *parent)
{
    Tucuxi::GuiCore::DrugVariate *newVariate;
    newVariate = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::DrugVariate>(ABSTRACTREPO,parent);
    newVariate->setQuantity(copy(variate->getQuantity(), newVariate));
    newVariate->setCovariateId(variate->getCovariateId());
    newVariate->setType(variate->getType());
//    newVariate->setUnitString(variate->getUnitString());

    return newVariate;
}

Tucuxi::GuiCore::ADME* PredictionSpecCopier::copy(Tucuxi::GuiCore::ADME* adme, QObject *parent)
{
    Tucuxi::GuiCore::ADME *newAdme;
    newAdme = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::ADME>(ABSTRACTREPO,parent);
    newAdme->setDefaultIntake(copy(adme->getDefaultIntake(), newAdme));
    newAdme->setDistribution(adme->getDistribution());
    newAdme->setElimination(adme->getElimination());
    newAdme->setIntakes(copy(adme->getIntakes(), newAdme));

    return newAdme;
}

Tucuxi::GuiCore::DrugModel* PredictionSpecCopier::copy(Tucuxi::GuiCore::DrugModel *drug, QObject *parent)
{
    Tucuxi::GuiCore::DrugModel *newDrug;
    newDrug = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::DrugModel>(ABSTRACTREPO,parent);
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


Tucuxi::GuiCore::StandardTreatment* PredictionSpecCopier::copy(Tucuxi::GuiCore::StandardTreatment* treatment, QObject *parent)
{
    if (treatment == nullptr)
        return nullptr;

    Tucuxi::GuiCore::StandardTreatment *newTreatment;
    newTreatment = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::StandardTreatment>(ABSTRACTREPO,parent);
    newTreatment->setIsFixedDuration(treatment->getIsFixedDuration());
    newTreatment->setDuration(treatment->getDuration());
    return newTreatment;
}

Tucuxi::GuiCore::ActiveSubstance* PredictionSpecCopier::copy(Tucuxi::GuiCore::ActiveSubstance *drug, QObject *parent)
{
    if (drug == nullptr)
        return nullptr;

    Tucuxi::GuiCore::ActiveSubstance *newDrug;
    newDrug = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::ActiveSubstance>(ABSTRACTREPO,parent);
    newDrug->setAtc(drug->getAtc());
    newDrug->setBrands(drug->getBrands());
    newDrug->setSubstanceId(drug->getSubstanceId());
//    newDrug->setMolarMass(drug->getMolarMass());

    return newDrug;

}

Tucuxi::GuiCore::DrugTreatment* PredictionSpecCopier::copy(Tucuxi::GuiCore::DrugTreatment *treatment, QObject *parent)
{
    Tucuxi::GuiCore::DrugTreatment *newTreatment;
    newTreatment = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::DrugTreatment>(ABSTRACTREPO,parent);
    newTreatment->setCovariates(copy(treatment->getCovariates(), newTreatment));
    newTreatment->setDosages(copy(treatment->getDosages(), newTreatment));
    newTreatment->setAdjustments(copy(treatment->getAdjustments(), newTreatment));
    newTreatment->setActiveSubstanceId(treatment->getActiveSubstanceId());
    newTreatment->setMeasures(copy(treatment->getMeasures(), newTreatment));
    newTreatment->setPatient(copy(treatment->getPatient(), newTreatment));
    newTreatment->setTargets(copy(treatment->getTargets(), newTreatment));

    return newTreatment;
}



Tucuxi::GuiCore::ValidDoses* PredictionSpecCopier::copy(Tucuxi::GuiCore::ValidDoses* list, QObject *parent)
{
    Tucuxi::GuiCore::ValidDoses *newList;
    newList = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::ValidDoses>(ABSTRACTREPO,parent);
    newList->setAnyDose(list->getAnyDose());
    newList->setQuantity(copy(list->getQuantity(), newList));
    newList->setFromDose(copy(list->getFromDose(), newList));
    newList->setToDose(copy(list->getToDose(), newList));
    newList->setStepDose(copy(list->getStepDose(), newList));
    for(int i=0;i<list->size();i++)
        newList->append(copy(list->at(i), newList));

    return newList;
}


Tucuxi::GuiCore::ErrorModel* PredictionSpecCopier::copy(Tucuxi::GuiCore::ErrorModel *model, QObject *parent)
{
    Tucuxi::GuiCore::ErrorModel *newModel;
    newModel = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::ErrorModel>(ABSTRACTREPO,parent);
    newModel->setAdditive(model->getAdditive());
    newModel->setProportional(model->getProportional());

    return newModel;
}


Tucuxi::GuiCore::Halflife* PredictionSpecCopier::copy(Tucuxi::GuiCore::Halflife *halflife, QObject *parent)
{
    Tucuxi::GuiCore::Halflife *newHalflife;
    newHalflife = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::Halflife>(ABSTRACTREPO,parent);
    newHalflife->setMultiplier(halflife->getMultiplier());
    newHalflife->setUnit(halflife->getUnit());
    newHalflife->setValue(halflife->getValue());

    return newHalflife;
}


Tucuxi::GuiCore::ValidInfusion* PredictionSpecCopier::copy(Tucuxi::GuiCore::ValidInfusion *source, QObject *parent)
{
    Tucuxi::GuiCore::ValidInfusion *target;
    target = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::ValidInfusion>(ABSTRACTREPO,parent);
    target->setQuantity(copy(source->getQuantity(), target));

    return target;
}

Tucuxi::GuiCore::ValidInfusions* PredictionSpecCopier::copy(Tucuxi::GuiCore::ValidInfusions *source, QObject *parent)
{
    Tucuxi::GuiCore::ValidInfusions *target;
    target = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::ValidInfusions>(ABSTRACTREPO,parent);
    target->setAny(source->getAny());
    target->setQuantity(copy(source->getQuantity(), target));
    for(int i=0;i<source->size();i++)
        target->append(copy(source->at(i), target));

    return target;
}


Tucuxi::GuiCore::ValidInterval* PredictionSpecCopier::copy(Tucuxi::GuiCore::ValidInterval *source, QObject *parent)
{
    Tucuxi::GuiCore::ValidInterval *target;
    target = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::ValidInterval>(ABSTRACTREPO,parent);
    target->setQuantity(copy(source->getQuantity(), target));

    return target;
}

Tucuxi::GuiCore::ValidIntervals* PredictionSpecCopier::copy(Tucuxi::GuiCore::ValidIntervals *source, QObject *parent)
{
    Tucuxi::GuiCore::ValidIntervals *target;
    target = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::ValidIntervals>(ABSTRACTREPO,parent);
    target->setAny(source->getAny());
    target->setQuantity(copy(source->getQuantity(), target));
    for(int i=0;i<source->size();i++)
        target->append(copy(source->at(i), target));

    return target;
}



Tucuxi::GuiCore::Target* PredictionSpecCopier::copy(Tucuxi::GuiCore::Target* source, QObject *parent)
{
    Tucuxi::GuiCore::Target *target;
    target = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::Target>(ABSTRACTREPO,parent);
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


Tucuxi::GuiCore::TargetList* PredictionSpecCopier::copy(Tucuxi::GuiCore::TargetList* source, QObject *parent)
{
    Tucuxi::GuiCore::TargetList *target;
    target = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::TargetList>(ABSTRACTREPO,parent);
    foreach(Tucuxi::GuiCore::Target* t, source->getList())
        target->append(copy(t, target));

    return target;
}


Tucuxi::GuiCore::CorePatient* PredictionSpecCopier::copy(Tucuxi::GuiCore::CorePatient* source, QObject *parent)
{
    Tucuxi::GuiCore::CorePatient *target;
    target = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::CorePatient>(ABSTRACTREPO,parent);
    // No field in corepatient...

    return target;
}

Tucuxi::GuiCore::CoreMeasureList* PredictionSpecCopier::copy(Tucuxi::GuiCore::CoreMeasureList* source, QObject *parent)
{
    Tucuxi::GuiCore::CoreMeasureList *target;
    target = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::CoreMeasureList>(ABSTRACTREPO,parent);
    foreach(Tucuxi::GuiCore::CoreMeasure* t, source->getList())
        target->append(copy(t, target));

    return target;
}

Tucuxi::GuiCore::CoreMeasure* PredictionSpecCopier::copy(Tucuxi::GuiCore::CoreMeasure* source, QObject *parent)
{
    Tucuxi::GuiCore::CoreMeasure *target;
    target = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::CoreMeasure>(ABSTRACTREPO,parent);
    target->setConcentration(copy(source->getConcentration(), target));
    target->setSdrug(source->getSdrug());
    target->setMoment(source->getMoment());

    return target;

}


Tucuxi::GuiCore::ValidDose* PredictionSpecCopier::copy(Tucuxi::GuiCore::ValidDose* source, QObject *parent)
{
    Tucuxi::GuiCore::ValidDose *target;
    target = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::ValidDose>(ABSTRACTREPO,parent);
    target->setQuantity(copy(source->getQuantity(), target));
    target->setRoute(copy(source->getRoute(), target));

    return target;

}

Tucuxi::GuiCore::TargetMethod* PredictionSpecCopier::copy(Tucuxi::GuiCore::TargetMethod* source, QObject *parent)
{
    Tucuxi::GuiCore::TargetMethod *target;
    target = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::TargetMethod>(ABSTRACTREPO,parent);
    target->setTargetType(source->getTargetType());
    target->setValue(source->getValue());

    return target;

}


Tucuxi::GuiCore::OperationList *PredictionSpecCopier::copy(Tucuxi::GuiCore::OperationList *source, QObject *parent)
{
    Tucuxi::GuiCore::OperationList *target;
    target = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::OperationList>(ABSTRACTREPO,parent);
    foreach(Tucuxi::GuiCore::Operation* t, source->getList())
        target->append(copy(t, target));

    return target;
}


Tucuxi::GuiCore::Operation *PredictionSpecCopier::copy(Tucuxi::GuiCore::Operation *source, QObject *parent)
{
    Tucuxi::GuiCore::Operation *target;
    target = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::Operation>(ABSTRACTREPO,parent);
    target->setFormula(source->getFormula());
    target->setType(source->getType());

    return target;
}


Tucuxi::GuiCore::ParameterSet *PredictionSpecCopier::copy(Tucuxi::GuiCore::ParameterSet *source, QObject *parent)
{
    Tucuxi::GuiCore::ParameterSet *target;
    target = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::ParameterSet>(ABSTRACTREPO,parent);
    target->setTime(source->getTime());
    target->setType(source->type());
    for(int i=0;i<source->getParameters()->getList().size();i++)
        target->set(copy(source->getParameters()->getList().at(i), target));

    target->copyCorrelationsFrom(*source);

    // Attention à setType qui n'a pas de setType()

    return target;
}

Tucuxi::GuiCore::Parameters *PredictionSpecCopier::copy(Tucuxi::GuiCore::Parameters *source, QObject *parent)
{
    return source;
    Tucuxi::GuiCore::Parameters *target;
    target = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::Parameters>(ABSTRACTREPO,parent);
    foreach(Tucuxi::GuiCore::Parameter* t, source->getList())
        target->append(copy(t, target));

    return target;
}

Tucuxi::GuiCore::Parameter *PredictionSpecCopier::copy(Tucuxi::GuiCore::Parameter *source, QObject *parent)
{
    Tucuxi::GuiCore::Parameter *target;
    target = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::Parameter>(ABSTRACTREPO,parent);
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

Tucuxi::GuiCore::Bsv *PredictionSpecCopier::copy(Tucuxi::GuiCore::Bsv *source, QObject *parent)
{
    Tucuxi::GuiCore::Bsv *target;
    target = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::Bsv>(ABSTRACTREPO,parent);
    target->setProportional(source->getProportional());
    target->setStandard(source->getStandard());

    return target;
}
