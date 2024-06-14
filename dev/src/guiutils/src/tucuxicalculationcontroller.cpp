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


#include <memory>
#include <iostream>

#include "errors_guiutils.h"

#include "core/utils/logging.h"
#include "calculationcontroller.h"
#include "core/dal/predictionspec.h"
#include "core/dal/predictive.h"
#include "core/dal/predictiondata.h"
#include "core/dal/predictionresult.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/dosage.h"
#include "core/dal/covariate.h"
#include "core/dal/coremeasure.h"
#include "core/dal/drug/target.h"
#include "tucuxicalculationcontroller.h"

#include "tucucommon/componentmanager.h"
#include "tucucore/computingservice/icomputingservice.h"
#include "tucucore/computingservice/computingtrait.h"
#include "tucucore/computingservice/computingrequest.h"
#include "tucucore/computingservice/computingresponse.h"
#include "tucucore/drugmodel/drugmodel.h"
#include "tucucore/drugtreatment/drugtreatment.h"
#include "tucucore/corecomponent.h"

#include "tucucore/drugmodels/buildimatinib.h"

using namespace Tucuxi::Common;
using namespace Tucuxi::Core;

DateTime buildDateTime(const QDateTime &qDate)
{
    return DateTime(std::chrono::milliseconds(qDate.toMSecsSinceEpoch()));
}


Unit buildUnit(const QString &_strUnit)
{
    return Unit(_strUnit.toLatin1().data());
}


void buildTreatment(DrugTreatment &_treatment, const Tucuxi::Gui::Core::DrugTreatment *_ezTreatment)
{
    QList<Tucuxi::Gui::Core::Dosage*> dosageList = _ezTreatment->getDosages()->getList();
    QList<Tucuxi::Gui::Core::Dosage*>::iterator itDosages = dosageList.begin();
    while (itDosages != dosageList.end()) {
        Tucuxi::Gui::Core::Dosage *dosage = *itDosages++;

        AbsorptionModel route;
        switch (dosage->getRoute()->getRoute()) {
            case Tucuxi::Gui::Core::Admin::Route::BOLUS: route = AbsorptionModel::EXTRAVASCULAR; break;
            case Tucuxi::Gui::Core::Admin::Route::INFUSION: route = AbsorptionModel::INTRAVASCULAR; break;
            case Tucuxi::Gui::Core::Admin::Route::EXTRA: route = AbsorptionModel::EXTRAVASCULAR; break;
            default: route = AbsorptionModel::EXTRAVASCULAR; break;
        }
        LastingDose lastingDose(dosage->getQuantity()->getDbvalue(),
                    route,
                    std::chrono::seconds(static_cast<int>(dosage->getDbtinf()*60.0)),
                    std::chrono::seconds(static_cast<int>(dosage->getDbinterval()*3600.0)));
        DosageLoop loop(lastingDose);
        _treatment.addDosageTimeRange(std::make_unique<DosageTimeRange>(
            buildDateTime(dosage->getApplied()),    // startDate,
            buildDateTime(dosage->getEndTime()),    // endDate,
            loop));                                 // dosage
    }

    const std::string analyteId = "imatinib";

    QList<Tucuxi::Gui::Core::PatientVariate*> covariateList = _ezTreatment->getCovariates()->getList();
    QList<Tucuxi::Gui::Core::PatientVariate*>::iterator itCovariates = covariateList.begin();
    while (itCovariates != covariateList.end()) {
        Tucuxi::Gui::Core::PatientVariate *covariate = *itCovariates++;
        _treatment.addCovariate(std::make_unique<PatientCovariate>(
            covariate->getName().toLatin1().data(),                    // _id,
            std::to_string(covariate->getQuantity()->getDbvalue()),    // _value,
            DataType::Double,                                          // _dataType,
            buildUnit(covariate->getQuantity()->getUnitstring()),      // _unit,
            buildDateTime(covariate->getDate())));                     // _date
    }

    QList<Tucuxi::Gui::Core::CoreMeasure*> sampleList = _ezTreatment->getMeasures()->getList();
    QList<Tucuxi::Gui::Core::CoreMeasure*>::iterator itSamples = sampleList.begin();
    while (itSamples != sampleList.end()) {
        Tucuxi::Gui::Core::CoreMeasure *sample = *itSamples++;
        _treatment.addSample(std::make_unique<Sample>(
            buildDateTime(sample->getMoment()),                     // date,
            analyteId,                                                   // analyteId,
            sample->getConcentration()->getDbvalue(),               // value,
            buildUnit(sample->getConcentration()->getUnitstring())  // unit
        ));
    }

    QList<Tucuxi::Gui::Core::Target*> targetList = _ezTreatment->getTargets()->getList();
    QList<Tucuxi::Gui::Core::Target*>::iterator itTargets = targetList.begin();
    while (itTargets != targetList.end()) {
        Tucuxi::Gui::Core::Target *target = *itTargets++;
        TargetType targetType;
        switch (target->getType()->getTargetType()) {
            case Tucuxi::Gui::Core::TargetMethod::TargetType::ResidualTarget: targetType = TargetType::Residual; break;
            case Tucuxi::Gui::Core::TargetMethod::TargetType::PeakTarget:     targetType = TargetType::Peak;     break;
            case Tucuxi::Gui::Core::TargetMethod::TargetType::MeanTarget:     targetType = TargetType::Mean;     break;
            case Tucuxi::Gui::Core::TargetMethod::TargetType::AUCTarget:      targetType = TargetType::Auc;      break;
            default:                                                       targetType = TargetType::Residual; break;
        }
        _treatment.addTarget(std::make_unique<Target>(
            analyteId, //_analyteId
            targetType,
            target->getCmin()->getDbvalue(),
            target->getCbest()->getDbvalue(),
            target->getCmax()->getDbvalue(),
            std::chrono::milliseconds(static_cast<int>(1000*target->getTmin()->getDbvalue())),
            std::chrono::milliseconds(static_cast<int>(1000*target->getTbest()->getDbvalue())),
            std::chrono::milliseconds(static_cast<int>(1000*target->getTmax()->getDbvalue()))));
    }
}


TucuxiCalculationController::TucuxiCalculationController(QObject *parent)
    : m_requestID(1)
{
    static bool bInitialized = false;
    if (!bInitialized) {
        bInitialized = true;

        ComponentManager* cmpMgr = ComponentManager::getInstance();
        cmpMgr->registerComponentFactory("CoreComponent", &CoreComponent::createComponent);

        // Create the Core component
        IComputingService *iCore = cmpMgr->createComponent<IComputingService>("CoreComponent");
        cmpMgr->registerComponent("TucuxiCore", iCore);
    }
}


Tucuxi::Gui::Core::PredictionResult* TucuxiCalculationController::computePrediction(
    const DrugModel& _model,
    const DrugTreatment &_treatment,
    PredictionParameterType _type,
    const DateTime& _startDate,
    const DateTime& _endDate,
    int nbPoints)
{
    ComponentManager* cmpMgr = ComponentManager::getInstance();
    IComputingService* iCore = cmpMgr->getComponent<IComputingService>("TucuxiCore");
    if (iCore != nullptr) {
        m_requestID++;

        std::unique_ptr<ComputingTrait> computingTrait = std::make_unique<ComputingTraitConcentration>(
            m_requestID,
            _type,
            _startDate,
            _endDate,
            nbPoints,
            ComputingOption::MainCompartment);

        ComputingTraits traits;
        traits.addTrait(std::move(computingTrait));
        ComputingRequest request(m_requestID, _model, _treatment, traits);
        std::unique_ptr<ComputingResponse> response = std::make_unique<ComputingResponse>(m_requestID);

        ComputingResult res = iCore->compute(request, response);
        if (res == ComputingResult::Success) {
            Tucuxi::Gui::Core::PredictionResult* pred = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PredictionResult>(ABSTRACTREPO);
            Tucuxi::Gui::Core::Predictive* predictive = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Predictive>(ABSTRACTREPO, pred);
            pred->setPredictive(predictive);
            Tucuxi::Gui::Core::PredictionData* pdata = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PredictionData>(ABSTRACTREPO, predictive);
            predictive->setPredictionData(pdata);
            Tucuxi::Gui::Core::FancyPoints* fpts = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::FancyPoints>(ABSTRACTREPO, pdata);
            pdata->setPoints(fpts);

            for (const std::unique_ptr<SingleComputingResponse>& resp: response->getResponses()) {
                SinglePredictionResponse* pSinglePred = dynamic_cast<SinglePredictionResponse*>(resp.get());
                if (pSinglePred != nullptr) {
                    for (CycleData cycleData: pSinglePred->getData()) {
                        for (size_t i=0; i<cycleData.m_concentrations[0].size(); i++) {
                            Tucuxi::Gui::Core::FancyPoint* fpt = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::FancyPoint>(ABSTRACTREPO, fpts);
                            fpt->setTime(cycleData.m_start.toSeconds()+cycleData.m_times[0][i]*3600.0);
                            fpt->setValue(cycleData.m_concentrations[0][i]);
                            fpts->append(fpt);
                        }
                    }
                }
            }
            pred->moveToThread(qApp->thread());
            return pred;
        }
    }
    return nullptr;
}


Tucuxi::Gui::Core::PredictionResult* TucuxiCalculationController::computePercentiles(
    const DrugModel& _model,
    const DrugTreatment &_treatment,
    PredictionParameterType _type,
    const DateTime& _startDate,
    const DateTime& _endDate,
    int nbPoints)
{
    ComponentManager* cmpMgr = ComponentManager::getInstance();
    IComputingService* iCore = cmpMgr->getComponent<IComputingService>("TucuxiCore");
    if (iCore != nullptr) {
        m_requestID++;

        PercentileRanks ranks = {10, 25, 75, 90};

        std::unique_ptr<ComputingTrait> computingTrait = std::make_unique<ComputingTraitPercentiles>(
            m_requestID,
            PredictionParameterType::Population,
            _startDate,
            _endDate,
            ranks,
            nbPoints,
            ComputingOption::MainCompartment);

        ComputingTraits traits;
        traits.addTrait(std::move(computingTrait));
        ComputingRequest request(m_requestID, _model, _treatment, traits);
        std::unique_ptr<ComputingResponse> response = std::make_unique<ComputingResponse>(m_requestID);

        ComputingResult res = iCore->compute(request, response);
        if (res == ComputingResult::Success) {
            Tucuxi::Gui::Core::PredictionResult* pred = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PredictionResult>(ABSTRACTREPO);
            Tucuxi::Gui::Core::Predictive* predictive = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Predictive>(ABSTRACTREPO, pred);
            pred->setPredictive(predictive);
            Tucuxi::Gui::Core::PercentileDataList* percpairs = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PercentileDataList>(ABSTRACTREPO, predictive);
            predictive->setPercentileDataList(percpairs);


            for (const std::unique_ptr<SingleComputingResponse>& resp: response->getResponses()) {
                PercentilesResponse* pPercentiles = dynamic_cast<PercentilesResponse*>(resp.get());
                if (pPercentiles != nullptr) {
                    for (size_t i = 0; i < pPercentiles->getNbRanks(); ++i) {
                        Tucuxi::Gui::Core::PercentileData* percpair = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PercentileData>(ABSTRACTREPO, predictive);
                        percpairs->append(percpair);
                        Tucuxi::Gui::Core::PredictionData* pdata = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PredictionData>(ABSTRACTREPO, percpair);
                        Tucuxi::Gui::Core::FancyPoints* fpts = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::FancyPoints>(ABSTRACTREPO);
                        fpts->setParent(pdata);
                        pdata->setPoints(fpts);
                        percpair->setPredictionData(pdata);
                        percpair->setPercentile(pPercentiles->getRank(i));
                        const CycleData& cycleData = pPercentiles->getData(i);
                        for (size_t j=0; i<cycleData.m_concentrations[0].size(); j++) {
                            Tucuxi::Gui::Core::FancyPoint* fpt = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::FancyPoint>(ABSTRACTREPO, fpts);
                            fpt->setTime(cycleData.m_start.toSeconds()+cycleData.m_times[0][i]*3600.0);
                            fpt->setValue(cycleData.m_concentrations[0][i]);
                            fpts->append(fpt);
                        }
                    }
                }
            }
            pred->moveToThread(qApp->thread());

            return pred;
        }
    }
    return nullptr;
}


void TucuxiCalculationController::computePopPred(Tucuxi::Gui::Core::PredictionSpec *_prediction)
{
    emit disengage();
    EXLOG(QtDebugMsg, Tucuxi::guiutils::NOEZERROR, "Running population prediction.");

    BuildImatinib builder;
    DrugModel *pDrugModel = builder.buildDrugModel();

    DrugTreatment drugTreatment;
    buildTreatment(drugTreatment, _prediction->getAnalysis()->getTreatment());

    Tucuxi::Gui::Core::PredictionResult *pPred = computePrediction(*pDrugModel,
                                                                drugTreatment,
                                                                PredictionParameterType::Population,
                                                                buildDateTime(_prediction->getStartDate()),
                                                                buildDateTime(_prediction->getEndDate()),
                                                                _prediction->getNbPoints());
    if (pPred == nullptr) {
        emit popCalcFail();
    } else {
        emit popPredComputed(pPred);
    }
    emit engage();
}


void TucuxiCalculationController::computePopPerc(Tucuxi::Gui::Core::PredictionSpec *_prediction)
{
    emit disengage();
    EXLOG(QtDebugMsg, Tucuxi::guiutils::NOEZERROR, "Running population percentiles.");

    BuildImatinib builder;
    DrugModel *pDrugModel = builder.buildDrugModel();

    DrugTreatment drugTreatment;
    buildTreatment(drugTreatment, _prediction->getAnalysis()->getTreatment());

    Tucuxi::Gui::Core::PredictionResult *pPred = computePercentiles(*pDrugModel,
                                                                 drugTreatment,
                                                                 PredictionParameterType::Population,
                                                                 buildDateTime(_prediction->getStartDate()),
                                                                 buildDateTime(_prediction->getEndDate()),
                                                                 _prediction->getNbPoints());
    if (pPred == nullptr) {
        emit popCalcFail();
    } else {
        emit popPercComputed(pPred);
    }
    emit engage();
}


void TucuxiCalculationController::computeAprPred(Tucuxi::Gui::Core::PredictionSpec *_prediction)
{
    emit disengage();
    EXLOG(QtDebugMsg, Tucuxi::guiutils::NOEZERROR, "Running apriori prediction.");

    BuildImatinib builder;
    DrugModel *pDrugModel = builder.buildDrugModel();

    DrugTreatment drugTreatment;
    buildTreatment(drugTreatment, _prediction->getAnalysis()->getTreatment());

    Tucuxi::Gui::Core::PredictionResult *pPred = computePrediction(*pDrugModel,
                                                                drugTreatment,
                                                                PredictionParameterType::Apriori,
                                                                buildDateTime(_prediction->getStartDate()),
                                                                buildDateTime(_prediction->getEndDate()),
                                                                _prediction->getNbPoints());
    if (pPred == nullptr) {
        emit aprCalcFail();
    } else {
        emit aprPredComputed(pPred);
    }
    emit engage();
}

void TucuxiCalculationController::computeAprPerc(Tucuxi::Gui::Core::PredictionSpec *_prediction)
{
    emit disengage();
    EXLOG(QtDebugMsg, Tucuxi::guiutils::NOEZERROR, "Running population percentiles.");

    BuildImatinib builder;
    DrugModel *pDrugModel = builder.buildDrugModel();

    DrugTreatment drugTreatment;
    buildTreatment(drugTreatment, _prediction->getAnalysis()->getTreatment());

    Tucuxi::Gui::Core::PredictionResult *pPred = computePercentiles(*pDrugModel,
                                                                 drugTreatment,
                                                                 PredictionParameterType::Apriori,
                                                                 buildDateTime(_prediction->getStartDate()),
                                                                 buildDateTime(_prediction->getEndDate()),
                                                                 _prediction->getNbPoints());
    if (pPred == nullptr) {
        emit aprCalcFail();
    } else {
        emit aprPercComputed(pPred);
    }
    emit engage();
}

void TucuxiCalculationController::computeApoPred(Tucuxi::Gui::Core::PredictionSpec *_prediction)
{
    emit disengage();
    EXLOG(QtDebugMsg, Tucuxi::guiutils::NOEZERROR, "Running aposteriori prediction.");

    BuildImatinib builder;
    DrugModel *pDrugModel = builder.buildDrugModel();

    DrugTreatment drugTreatment;
    buildTreatment(drugTreatment, _prediction->getAnalysis()->getTreatment());

    Tucuxi::Gui::Core::PredictionResult *pPred = computePrediction(*pDrugModel,
                                                                drugTreatment,
                                                                PredictionParameterType::Aposteriori,
                                                                buildDateTime(_prediction->getStartDate()),
                                                                buildDateTime(_prediction->getEndDate()),
                                                                _prediction->getNbPoints());
    if (pPred == nullptr) {
        emit apoCalcFail();
    } else {
        emit apoPredComputed(pPred);
    }
    emit engage();
}

void TucuxiCalculationController::computeApoPerc(Tucuxi::Gui::Core::PredictionSpec *_prediction)
{
    emit disengage();
    EXLOG(QtDebugMsg, Tucuxi::guiutils::NOEZERROR, "Running aposterior percentiles.");

    BuildImatinib builder;
    DrugModel *pDrugModel = builder.buildDrugModel();

    DrugTreatment drugTreatment;
    buildTreatment(drugTreatment, _prediction->getAnalysis()->getTreatment());

    Tucuxi::Gui::Core::PredictionResult *pPred = computePercentiles(*pDrugModel,
                                                                 drugTreatment,
                                                                 PredictionParameterType::Aposteriori,
                                                                 buildDateTime(_prediction->getStartDate()),
                                                                 buildDateTime(_prediction->getEndDate()),
                                                                 _prediction->getNbPoints());
    if (pPred == nullptr) {
        emit apoCalcFail();
    } else {
        emit apoPercComputed(pPred);
    }
    emit engage();
}

void TucuxiCalculationController::computeRevPred(Tucuxi::Gui::Core::PredictionSpec *_prediction)
{
}

void TucuxiCalculationController::computeAdjPred(Tucuxi::Gui::Core::PredictionSpec *_prediction)
{
}

void TucuxiCalculationController::abortAll()
{
}
