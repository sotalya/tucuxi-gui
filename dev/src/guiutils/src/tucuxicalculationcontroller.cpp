//@@license@@

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


void buildTreatment(DrugTreatment &_treatment, const ezechiel::core::DrugTreatment *_ezTreatment)
{
    QList<ezechiel::core::Dosage*> dosageList = _ezTreatment->getDosages()->getList();
    QList<ezechiel::core::Dosage*>::iterator itDosages = dosageList.begin();
    while (itDosages != dosageList.end()) {
        ezechiel::core::Dosage *dosage = *itDosages++;

        AbsorptionModel route;
        switch (dosage->getRoute()->getRoute()) {
            case ezechiel::core::Admin::Route::BOLUS: route = AbsorptionModel::EXTRAVASCULAR; break;
            case ezechiel::core::Admin::Route::INFUSION: route = AbsorptionModel::INTRAVASCULAR; break;
            case ezechiel::core::Admin::Route::EXTRA: route = AbsorptionModel::EXTRAVASCULAR; break;
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

    QList<ezechiel::core::PatientVariate*> covariateList = _ezTreatment->getCovariates()->getList();
    QList<ezechiel::core::PatientVariate*>::iterator itCovariates = covariateList.begin();
    while (itCovariates != covariateList.end()) {
        ezechiel::core::PatientVariate *covariate = *itCovariates++;
        _treatment.addCovariate(std::make_unique<PatientCovariate>(
            covariate->getName().toLatin1().data(),                    // _id,
            std::to_string(covariate->getQuantity()->getDbvalue()),    // _value,
            DataType::Double,                                          // _dataType,
            buildUnit(covariate->getQuantity()->getUnitstring()),      // _unit,
            buildDateTime(covariate->getDate())));                     // _date
    }

    QList<ezechiel::core::CoreMeasure*> sampleList = _ezTreatment->getMeasures()->getList();
    QList<ezechiel::core::CoreMeasure*>::iterator itSamples = sampleList.begin();
    while (itSamples != sampleList.end()) {
        ezechiel::core::CoreMeasure *sample = *itSamples++;
        _treatment.addSample(std::make_unique<Sample>(
            buildDateTime(sample->getMoment()),                     // date,
            analyteId,                                                   // analyteId,
            sample->getConcentration()->getDbvalue(),               // value,
            buildUnit(sample->getConcentration()->getUnitstring())  // unit
        ));
    }

    QList<ezechiel::core::Target*> targetList = _ezTreatment->getTargets()->getList();
    QList<ezechiel::core::Target*>::iterator itTargets = targetList.begin();
    while (itTargets != targetList.end()) {
        ezechiel::core::Target *target = *itTargets++;
        TargetType targetType;
        switch (target->getType()->getTargetType()) {
            case ezechiel::core::TargetMethod::TargetType::ResidualTarget: targetType = TargetType::Residual; break;
            case ezechiel::core::TargetMethod::TargetType::PeakTarget:     targetType = TargetType::Peak;     break;
            case ezechiel::core::TargetMethod::TargetType::MeanTarget:     targetType = TargetType::Mean;     break;
            case ezechiel::core::TargetMethod::TargetType::AUCTarget:      targetType = TargetType::Auc;      break;
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


ezechiel::core::PredictionResult* TucuxiCalculationController::computePrediction(
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
            ezechiel::core::PredictionResult* pred = ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionResult>(ABSTRACTREPO);
            ezechiel::core::Predictive* predictive = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Predictive>(ABSTRACTREPO, pred);
            pred->setPredictive(predictive);
            ezechiel::core::PredictionData* pdata = ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionData>(ABSTRACTREPO, predictive);
            predictive->setPredictionData(pdata);
            ezechiel::core::FancyPoints* fpts = ezechiel::core::CoreFactory::createEntity<ezechiel::core::FancyPoints>(ABSTRACTREPO, pdata);
            pdata->setPoints(fpts);

            for (const std::unique_ptr<SingleComputingResponse>& resp: response->getResponses()) {
                SinglePredictionResponse* pSinglePred = dynamic_cast<SinglePredictionResponse*>(resp.get());
                if (pSinglePred != nullptr) {
                    for (CycleData cycleData: pSinglePred->getData()) {
                        for (size_t i=0; i<cycleData.m_concentrations[0].size(); i++) {
                            ezechiel::core::FancyPoint* fpt = ezechiel::core::CoreFactory::createEntity<ezechiel::core::FancyPoint>(ABSTRACTREPO, fpts);
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


ezechiel::core::PredictionResult* TucuxiCalculationController::computePercentiles(
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
            ezechiel::core::PredictionResult* pred = ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionResult>(ABSTRACTREPO);
            ezechiel::core::Predictive* predictive = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Predictive>(ABSTRACTREPO, pred);
            pred->setPredictive(predictive);
            ezechiel::core::PercentileDataList* percpairs = ezechiel::core::CoreFactory::createEntity<ezechiel::core::PercentileDataList>(ABSTRACTREPO, predictive);
            predictive->setPercentileDataList(percpairs);


            for (const std::unique_ptr<SingleComputingResponse>& resp: response->getResponses()) {
                PercentilesResponse* pPercentiles = dynamic_cast<PercentilesResponse*>(resp.get());
                if (pPercentiles != nullptr) {
                    for (size_t i = 0; i < pPercentiles->getNbRanks(); ++i) {
                        ezechiel::core::PercentileData* percpair = ezechiel::core::CoreFactory::createEntity<ezechiel::core::PercentileData>(ABSTRACTREPO, predictive);
                        percpairs->append(percpair);
                        ezechiel::core::PredictionData* pdata = ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionData>(ABSTRACTREPO, percpair);
                        ezechiel::core::FancyPoints* fpts = ezechiel::core::CoreFactory::createEntity<ezechiel::core::FancyPoints>(ABSTRACTREPO);
                        fpts->setParent(pdata);
                        pdata->setPoints(fpts);
                        percpair->setPredictionData(pdata);
                        percpair->setPercentile(pPercentiles->getRank(i));
                        const CycleData& cycleData = pPercentiles->getData(i);
                        for (size_t j=0; i<cycleData.m_concentrations[0].size(); j++) {
                            ezechiel::core::FancyPoint* fpt = ezechiel::core::CoreFactory::createEntity<ezechiel::core::FancyPoint>(ABSTRACTREPO, fpts);
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


void TucuxiCalculationController::computePopPred(ezechiel::core::PredictionSpec *_prediction)
{
    emit disengage();
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Running population prediction.");

    BuildImatinib builder;
    DrugModel *pDrugModel = builder.buildDrugModel();

    DrugTreatment drugTreatment;
    buildTreatment(drugTreatment, _prediction->getAnalysis()->getTreatment());

    ezechiel::core::PredictionResult *pPred = computePrediction(*pDrugModel,
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


void TucuxiCalculationController::computePopPerc(ezechiel::core::PredictionSpec *_prediction)
{
    emit disengage();
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Running population percentiles.");

    BuildImatinib builder;
    DrugModel *pDrugModel = builder.buildDrugModel();

    DrugTreatment drugTreatment;
    buildTreatment(drugTreatment, _prediction->getAnalysis()->getTreatment());

    ezechiel::core::PredictionResult *pPred = computePercentiles(*pDrugModel,
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


void TucuxiCalculationController::computeAprPred(ezechiel::core::PredictionSpec *_prediction)
{
    emit disengage();
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Running apriori prediction.");

    BuildImatinib builder;
    DrugModel *pDrugModel = builder.buildDrugModel();

    DrugTreatment drugTreatment;
    buildTreatment(drugTreatment, _prediction->getAnalysis()->getTreatment());

    ezechiel::core::PredictionResult *pPred = computePrediction(*pDrugModel,
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

void TucuxiCalculationController::computeAprPerc(ezechiel::core::PredictionSpec *_prediction)
{
    emit disengage();
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Running population percentiles.");

    BuildImatinib builder;
    DrugModel *pDrugModel = builder.buildDrugModel();

    DrugTreatment drugTreatment;
    buildTreatment(drugTreatment, _prediction->getAnalysis()->getTreatment());

    ezechiel::core::PredictionResult *pPred = computePercentiles(*pDrugModel,
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

void TucuxiCalculationController::computeApoPred(ezechiel::core::PredictionSpec *_prediction)
{
    emit disengage();
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Running aposteriori prediction.");

    BuildImatinib builder;
    DrugModel *pDrugModel = builder.buildDrugModel();

    DrugTreatment drugTreatment;
    buildTreatment(drugTreatment, _prediction->getAnalysis()->getTreatment());

    ezechiel::core::PredictionResult *pPred = computePrediction(*pDrugModel,
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

void TucuxiCalculationController::computeApoPerc(ezechiel::core::PredictionSpec *_prediction)
{
    emit disengage();
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Running aposterior percentiles.");

    BuildImatinib builder;
    DrugModel *pDrugModel = builder.buildDrugModel();

    DrugTreatment drugTreatment;
    buildTreatment(drugTreatment, _prediction->getAnalysis()->getTreatment());

    ezechiel::core::PredictionResult *pPred = computePercentiles(*pDrugModel,
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

void TucuxiCalculationController::computeRevPred(ezechiel::core::PredictionSpec *_prediction)
{
}

void TucuxiCalculationController::computeAdjPred(ezechiel::core::PredictionSpec *_prediction)
{
}

void TucuxiCalculationController::abortAll()
{
}
