/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
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

using namespace Tucuxi::Gui::GuiUtils;

PredictionSpecCopier::PredictionSpecCopier()
{

}

Tucuxi::Gui::Core::PredictionSpec *PredictionSpecCopier::copy(Tucuxi::Gui::Core::PredictionSpec *spec, QObject *parent)
{
    Tucuxi::Gui::Core::PredictionSpec *newSpec;
    newSpec = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PredictionSpec>(ABSTRACTREPO,parent);
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


Tucuxi::Gui::Core::DrugResponseAnalysis *PredictionSpecCopier::copy(Tucuxi::Gui::Core::DrugResponseAnalysis *analysis, QObject *parent)
{
    Tucuxi::Gui::Core::DrugResponseAnalysis *newAnalysis;
    newAnalysis = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::DrugResponseAnalysis>(ABSTRACTREPO,parent);

    newAnalysis->setTreatment(copy(analysis->getTreatment(), newAnalysis));
    newAnalysis->setDrugModel(copy(analysis->getDrugModel(), newAnalysis));

    return newAnalysis;
}


Tucuxi::Gui::Core::IdentifiableAmount *PredictionSpecCopier::copy(Tucuxi::Gui::Core::IdentifiableAmount *amount, QObject *parent)
{
    Tucuxi::Gui::Core::IdentifiableAmount *newAmount;
    newAmount = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::IdentifiableAmount>(ABSTRACTREPO,parent);

    newAmount->setAmountId(amount->getAmountId());
    newAmount->setDbvalue(amount->getDbvalue());
    newAmount->setUnit(amount->unit());
    newAmount->setValue(amount->value());

    return newAmount;
}

Tucuxi::Gui::Core::OperableAmount *PredictionSpecCopier::copy(Tucuxi::Gui::Core::OperableAmount *amount, QObject *parent)
{
    Tucuxi::Gui::Core::OperableAmount *newAmount;
    newAmount = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::OperableAmount>(ABSTRACTREPO,parent);

    newAmount->setAmountId(amount->getAmountId());
    newAmount->setDbvalue(amount->getDbvalue());
    newAmount->setFixed(amount->getFixed());
    newAmount->setOperations(copy(amount->getOperations(), newAmount));
    newAmount->setUnit(amount->unit());
    newAmount->setValue(amount->value());
    // Something strange with OperableAmount. To be checked

    return newAmount;
}


Tucuxi::Gui::Core::PatientVariate* PredictionSpecCopier::copy(Tucuxi::Gui::Core::PatientVariate* variate, QObject *parent)
{
    Tucuxi::Gui::Core::PatientVariate *newVariate;
    newVariate = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PatientVariate>(ABSTRACTREPO,parent);
    // To be completed
    newVariate->setDate(variate->getDate());
    newVariate->setForced(variate->getForced());
    newVariate->setCovariateId(variate->getCovariateId());
    newVariate->setQuantity(copy(variate->getQuantity(), newVariate));
    newVariate->setType(variate->getType());
    newVariate->setValueAsString(variate->getValueAsString());

    return newVariate;
}

Tucuxi::Gui::Core::PatientVariateList* PredictionSpecCopier::copy(Tucuxi::Gui::Core::PatientVariateList* list, QObject *parent)
{
    Tucuxi::Gui::Core::PatientVariateList *newList;
    newList = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PatientVariateList>(ABSTRACTREPO,parent);
    foreach(Tucuxi::Gui::Core::PatientVariate *variate, list->getList())
        newList->append(copy(variate, newList));

    return newList;
}


Tucuxi::Gui::Core::Admin* PredictionSpecCopier::copy(Tucuxi::Gui::Core::Admin* admin, QObject *parent)
{
    Tucuxi::Gui::Core::Admin *newAdmin;
    newAdmin = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Admin>(ABSTRACTREPO,parent);
    newAdmin->setRoute(admin->getRoute());
    newAdmin->setValue(admin->getValue());
    newAdmin->setFormulationAndRoute(admin->getFormulationAndRoute());

    return newAdmin;
}

Tucuxi::Gui::Core::Dosage* PredictionSpecCopier::copy(Tucuxi::Gui::Core::Dosage* dosage, QObject *parent)
{
    Tucuxi::Gui::Core::Dosage *newDosage;
    newDosage = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Dosage>(ABSTRACTREPO,parent);
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

Tucuxi::Gui::Core::DosageHistory* PredictionSpecCopier::copy(Tucuxi::Gui::Core::DosageHistory* history, QObject *parent)
{
    Tucuxi::Gui::Core::DosageHistory *newHistory;
    newHistory = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::DosageHistory>(ABSTRACTREPO,parent);
    foreach(Tucuxi::Gui::Core::Dosage* dosage, history->getList())
        newHistory->append(copy(dosage, newHistory));

    return newHistory;
}


Tucuxi::Gui::Core::AdminList* PredictionSpecCopier::copy(Tucuxi::Gui::Core::AdminList* list, QObject *parent)
{
    Tucuxi::Gui::Core::AdminList *newList;
    newList = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::AdminList>(ABSTRACTREPO,parent);
    foreach(Tucuxi::Gui::Core::Admin* admin, list->getList())
        newList->append(copy(admin, newList));

    return newList;
}

Tucuxi::Gui::Core::DrugVariateList* PredictionSpecCopier::copy(Tucuxi::Gui::Core::DrugVariateList* list, QObject *parent)
{
    Tucuxi::Gui::Core::DrugVariateList *newList;
    newList = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::DrugVariateList>(ABSTRACTREPO,parent);
    foreach(Tucuxi::Gui::Core::DrugVariate* variate, list->getList())
        newList->append(copy(variate, newList));

    return newList;
}

Tucuxi::Gui::Core::Concentrations* PredictionSpecCopier::copy(Tucuxi::Gui::Core::Concentrations* concentrations, QObject *parent)
{
    Tucuxi::Gui::Core::Concentrations *newConcentrations;
    newConcentrations = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Concentrations>(ABSTRACTREPO,parent);
    newConcentrations->setQuantity(concentrations->getQuantity());

    return newConcentrations;
}

Tucuxi::Gui::Core::DrugVariate* PredictionSpecCopier::copy(Tucuxi::Gui::Core::DrugVariate* variate, QObject *parent)
{
    Tucuxi::Gui::Core::DrugVariate *newVariate;
    newVariate = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::DrugVariate>(ABSTRACTREPO,parent);
    newVariate->setQuantity(copy(variate->getQuantity(), newVariate));
    newVariate->setCovariateId(variate->getCovariateId());
    newVariate->setType(variate->getType());
//    newVariate->setUnitString(variate->getUnitString());

    return newVariate;
}

Tucuxi::Gui::Core::ADME* PredictionSpecCopier::copy(Tucuxi::Gui::Core::ADME* adme, QObject *parent)
{
    Tucuxi::Gui::Core::ADME *newAdme;
    newAdme = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::ADME>(ABSTRACTREPO,parent);
    newAdme->setDefaultIntake(copy(adme->getDefaultIntake(), newAdme));
    newAdme->setDistribution(adme->getDistribution());
    newAdme->setElimination(adme->getElimination());
    newAdme->setIntakes(copy(adme->getIntakes(), newAdme));

    return newAdme;
}

Tucuxi::Gui::Core::DrugModel* PredictionSpecCopier::copy(Tucuxi::Gui::Core::DrugModel *drug, QObject *parent)
{
    Tucuxi::Gui::Core::DrugModel *newDrug;
    newDrug = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::DrugModel>(ABSTRACTREPO,parent);
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


Tucuxi::Gui::Core::StandardTreatment* PredictionSpecCopier::copy(Tucuxi::Gui::Core::StandardTreatment* treatment, QObject *parent)
{
    if (treatment == nullptr)
        return nullptr;

    Tucuxi::Gui::Core::StandardTreatment *newTreatment;
    newTreatment = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::StandardTreatment>(ABSTRACTREPO,parent);
    newTreatment->setIsFixedDuration(treatment->getIsFixedDuration());
    newTreatment->setDuration(treatment->getDuration());
    return newTreatment;
}

Tucuxi::Gui::Core::ActiveSubstance* PredictionSpecCopier::copy(Tucuxi::Gui::Core::ActiveSubstance *drug, QObject *parent)
{
    if (drug == nullptr)
        return nullptr;

    Tucuxi::Gui::Core::ActiveSubstance *newDrug;
    newDrug = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::ActiveSubstance>(ABSTRACTREPO,parent);
    newDrug->setAtc(drug->getAtc());
    newDrug->setBrands(drug->getBrands());
    newDrug->setSubstanceId(drug->getSubstanceId());
//    newDrug->setMolarMass(drug->getMolarMass());

    return newDrug;

}

Tucuxi::Gui::Core::DrugTreatment* PredictionSpecCopier::copy(Tucuxi::Gui::Core::DrugTreatment *treatment, QObject *parent)
{
    Tucuxi::Gui::Core::DrugTreatment *newTreatment;
    newTreatment = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::DrugTreatment>(ABSTRACTREPO,parent);
    newTreatment->setCovariates(copy(treatment->getCovariates(), newTreatment));
    newTreatment->setDosages(copy(treatment->getDosages(), newTreatment));
    newTreatment->setAdjustments(copy(treatment->getAdjustments(), newTreatment));
    newTreatment->setActiveSubstanceId(treatment->getActiveSubstanceId());
    newTreatment->setMeasures(copy(treatment->getMeasures(), newTreatment));
    newTreatment->setPatient(copy(treatment->getPatient(), newTreatment));
    newTreatment->setTargets(copy(treatment->getTargets(), newTreatment));

    return newTreatment;
}



Tucuxi::Gui::Core::ValidDoses* PredictionSpecCopier::copy(Tucuxi::Gui::Core::ValidDoses* list, QObject *parent)
{
    Tucuxi::Gui::Core::ValidDoses *newList;
    newList = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::ValidDoses>(ABSTRACTREPO,parent);
    newList->setAnyDose(list->getAnyDose());
    newList->setQuantity(copy(list->getQuantity(), newList));
    newList->setFromDose(copy(list->getFromDose(), newList));
    newList->setToDose(copy(list->getToDose(), newList));
    newList->setStepDose(copy(list->getStepDose(), newList));
    for(int i=0;i<list->size();i++)
        newList->append(copy(list->at(i), newList));

    return newList;
}


Tucuxi::Gui::Core::ErrorModel* PredictionSpecCopier::copy(Tucuxi::Gui::Core::ErrorModel *model, QObject *parent)
{
    Tucuxi::Gui::Core::ErrorModel *newModel;
    newModel = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::ErrorModel>(ABSTRACTREPO,parent);
    newModel->setAdditive(model->getAdditive());
    newModel->setProportional(model->getProportional());

    return newModel;
}


Tucuxi::Gui::Core::Halflife* PredictionSpecCopier::copy(Tucuxi::Gui::Core::Halflife *halflife, QObject *parent)
{
    Tucuxi::Gui::Core::Halflife *newHalflife;
    newHalflife = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Halflife>(ABSTRACTREPO,parent);
    newHalflife->setMultiplier(halflife->getMultiplier());
    newHalflife->setUnit(halflife->getUnit());
    newHalflife->setValue(halflife->getValue());

    return newHalflife;
}


Tucuxi::Gui::Core::ValidInfusion* PredictionSpecCopier::copy(Tucuxi::Gui::Core::ValidInfusion *source, QObject *parent)
{
    Tucuxi::Gui::Core::ValidInfusion *target;
    target = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::ValidInfusion>(ABSTRACTREPO,parent);
    target->setQuantity(copy(source->getQuantity(), target));

    return target;
}

Tucuxi::Gui::Core::ValidInfusions* PredictionSpecCopier::copy(Tucuxi::Gui::Core::ValidInfusions *source, QObject *parent)
{
    Tucuxi::Gui::Core::ValidInfusions *target;
    target = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::ValidInfusions>(ABSTRACTREPO,parent);
    target->setAny(source->getAny());
    target->setQuantity(copy(source->getQuantity(), target));
    for(int i=0;i<source->size();i++)
        target->append(copy(source->at(i), target));

    return target;
}


Tucuxi::Gui::Core::ValidInterval* PredictionSpecCopier::copy(Tucuxi::Gui::Core::ValidInterval *source, QObject *parent)
{
    Tucuxi::Gui::Core::ValidInterval *target;
    target = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::ValidInterval>(ABSTRACTREPO,parent);
    target->setQuantity(copy(source->getQuantity(), target));

    return target;
}

Tucuxi::Gui::Core::ValidIntervals* PredictionSpecCopier::copy(Tucuxi::Gui::Core::ValidIntervals *source, QObject *parent)
{
    Tucuxi::Gui::Core::ValidIntervals *target;
    target = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::ValidIntervals>(ABSTRACTREPO,parent);
    target->setAny(source->getAny());
    target->setQuantity(copy(source->getQuantity(), target));
    for(int i=0;i<source->size();i++)
        target->append(copy(source->at(i), target));

    return target;
}



Tucuxi::Gui::Core::Target* PredictionSpecCopier::copy(Tucuxi::Gui::Core::Target* source, QObject *parent)
{
    Tucuxi::Gui::Core::Target *target;
    target = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Target>(ABSTRACTREPO,parent);
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


Tucuxi::Gui::Core::TargetList* PredictionSpecCopier::copy(Tucuxi::Gui::Core::TargetList* source, QObject *parent)
{
    Tucuxi::Gui::Core::TargetList *target;
    target = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::TargetList>(ABSTRACTREPO,parent);
    foreach(Tucuxi::Gui::Core::Target* t, source->getList())
        target->append(copy(t, target));

    return target;
}


Tucuxi::Gui::Core::CorePatient* PredictionSpecCopier::copy(Tucuxi::Gui::Core::CorePatient* source, QObject *parent)
{
    Tucuxi::Gui::Core::CorePatient *target;
    target = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::CorePatient>(ABSTRACTREPO,parent);
    // No field in corepatient...

    return target;
}

Tucuxi::Gui::Core::CoreMeasureList* PredictionSpecCopier::copy(Tucuxi::Gui::Core::CoreMeasureList* source, QObject *parent)
{
    Tucuxi::Gui::Core::CoreMeasureList *target;
    target = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::CoreMeasureList>(ABSTRACTREPO,parent);
    foreach(Tucuxi::Gui::Core::CoreMeasure* t, source->getList())
        if(t->getEnable())
            target->append(copy(t, target));

    return target;
}

Tucuxi::Gui::Core::CoreMeasure* PredictionSpecCopier::copy(Tucuxi::Gui::Core::CoreMeasure* source, QObject *parent)
{
    Tucuxi::Gui::Core::CoreMeasure *target;
    target = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::CoreMeasure>(ABSTRACTREPO,parent);
    target->setConcentration(copy(source->getConcentration(), target));
    target->setSdrug(source->getSdrug());
    target->setMoment(source->getMoment());

    return target;

}


Tucuxi::Gui::Core::ValidDose* PredictionSpecCopier::copy(Tucuxi::Gui::Core::ValidDose* source, QObject *parent)
{
    Tucuxi::Gui::Core::ValidDose *target;
    target = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::ValidDose>(ABSTRACTREPO,parent);
    target->setQuantity(copy(source->getQuantity(), target));
    target->setRoute(copy(source->getRoute(), target));

    return target;

}

Tucuxi::Gui::Core::TargetMethod* PredictionSpecCopier::copy(Tucuxi::Gui::Core::TargetMethod* source, QObject *parent)
{
    Tucuxi::Gui::Core::TargetMethod *target;
    target = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::TargetMethod>(ABSTRACTREPO,parent);
    target->setTargetType(source->getTargetType());
    target->setValue(source->getValue());

    return target;

}


Tucuxi::Gui::Core::OperationList *PredictionSpecCopier::copy(Tucuxi::Gui::Core::OperationList *source, QObject *parent)
{
    Tucuxi::Gui::Core::OperationList *target;
    target = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::OperationList>(ABSTRACTREPO,parent);
    foreach(Tucuxi::Gui::Core::Operation* t, source->getList())
        target->append(copy(t, target));

    return target;
}


Tucuxi::Gui::Core::Operation *PredictionSpecCopier::copy(Tucuxi::Gui::Core::Operation *source, QObject *parent)
{
    Tucuxi::Gui::Core::Operation *target;
    target = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Operation>(ABSTRACTREPO,parent);
    target->setFormula(source->getFormula());
    target->setType(source->getType());

    return target;
}


Tucuxi::Gui::Core::ParameterSet *PredictionSpecCopier::copy(Tucuxi::Gui::Core::ParameterSet *source, QObject *parent)
{
    Tucuxi::Gui::Core::ParameterSet *target;
    target = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::ParameterSet>(ABSTRACTREPO,parent);
    target->setTime(source->getTime());
    target->setType(source->type());
    for(int i=0;i<source->getParameters()->getList().size();i++)
        target->set(copy(source->getParameters()->getList().at(i), target));

    target->copyCorrelationsFrom(*source);

    // Attention à setType qui n'a pas de setType()

    return target;
}

Tucuxi::Gui::Core::Parameters *PredictionSpecCopier::copy(Tucuxi::Gui::Core::Parameters *source, QObject *parent)
{
    return source;
    Tucuxi::Gui::Core::Parameters *target;
    target = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Parameters>(ABSTRACTREPO,parent);
    foreach(Tucuxi::Gui::Core::Parameter* t, source->getList())
        target->append(copy(t, target));

    return target;
}

Tucuxi::Gui::Core::Parameter *PredictionSpecCopier::copy(Tucuxi::Gui::Core::Parameter *source, QObject *parent)
{
    Tucuxi::Gui::Core::Parameter *target;
    target = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Parameter>(ABSTRACTREPO,parent);
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

Tucuxi::Gui::Core::Bsv *PredictionSpecCopier::copy(Tucuxi::Gui::Core::Bsv *source, QObject *parent)
{
    Tucuxi::Gui::Core::Bsv *target;
    target = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Bsv>(ABSTRACTREPO,parent);
    target->setProportional(source->getProportional());
    target->setStandard(source->getStandard());

    return target;
}
