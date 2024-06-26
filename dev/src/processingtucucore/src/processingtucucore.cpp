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


#include <iostream>

#include "processingtucucore.h"


#include "guitotucucoretranslator.h"
#include "tucucoretoguitranslator.h"
#include "tqflogger.h"

// #include "errors_processing.h"
#include "core/core.h"
#include "core/dal/predictionresult.h"
#include "core/utils/logging.h"
#include "core/utils/errorenums.h"
#include "core/corerepository.h"
#include "core/dal/drug/parameters.h"
#include "core/dal/drug/doses.h"
#include "core/dal/drug/intervals.h"
#include "core/dal/drug/infusions.h"
#include "core/dal/coremeasure.h"
#include "core/dal/drug/drug.h"
#include "core/dal/drug/target.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drug/drug.h"
#include "core/dal/drug/errormodel.h"
#include "core/dal/drug/translatablestring.h"
#include "guiutils/src/calculationcontroller.h"

//#include "tucucommon/loggerhelper.h"
#include "tucucommon/componentmanager.h"
#include "tucucore/computingcomponent.h"

#include "tucucore/computingservice/icomputingservice.h"
#include "tucucore/computingservice/computingtrait.h"
#include "tucucore/computingservice/computingrequest.h"
#include "tucucore/computingservice/computingresponse.h"
#include "tucucore/cyclestatisticscalculator.h"
#include "tucucore/overloadevaluator.h"
#include "tucucore/drugdomainconstraintsevaluator.h"
#include "tucucore/operation.h"
#include "tucucore/cachecomputing.h"

using namespace Tucuxi::Gui::Processing;

void checkCovariate(const Tucuxi::Common::DateTime _startDate, const Tucuxi::Core::DrugTreatment &drugTreatment, const Tucuxi::Core::DrugModel &drugModel, Tucuxi::Gui::Core::PredictionResult& prediction, const Tucuxi::Common::DateTime _endDate)
{
    // This check should be done when building processing data. We keep it here to be safe.
    if (_startDate > _endDate) {
        return;
    }
    Tucuxi::Core::DrugDomainConstraintsEvaluator drugDomainConstraintsEvaluator;
    std::vector<Tucuxi::Core::DrugDomainConstraintsEvaluator::EvaluationResult> evaluationResults;
    Tucuxi::Core::DrugDomainConstraintsEvaluator::Result drugDomainResult = drugDomainConstraintsEvaluator.evaluate(drugModel,
                                                                                                                    drugTreatment,
                                                                                                                    _startDate,
                                                                                                                    _endDate,
                                                                                                                    evaluationResults);
    if (drugDomainResult != Tucuxi::Core::DrugDomainConstraintsEvaluator::Result::Compatible){
        prediction.setIsValidDomain(false);
        std::string errorMessage;
        for (const auto &evalResult : evaluationResults){
            if (evalResult.m_result != Tucuxi::Core::DrugDomainConstraintsEvaluator::Result::Compatible) {
                errorMessage += evalResult.m_constraint->getErrorMessage().getString() + " ";
            }
        }
        prediction.setDomainMessage(QString::fromStdString(errorMessage));
    }
}

ProcessingTucucore::ProcessingTucucore() : m_requestID(1)
{
    static bool bInitialized = false;
    if (!bInitialized) {
        bInitialized = true;

//        Tucuxi::Common::LoggerHelper::init("logFileComputing.log");

        Tucuxi::Common::ComponentManager* cmpMgr = Tucuxi::Common::ComponentManager::getInstance();
        cmpMgr->registerComponentFactory("ComputingComponent", &Tucuxi::Core::ComputingComponent::createComponent);

        // Create the Core component
        Tucuxi::Core::IComputingService *iCore = cmpMgr->createComponent<Tucuxi::Core::IComputingService>("ComputingComponent");
        cmpMgr->registerComponent("ComputingService", iCore);

        // We define the maximum number of points that can be calculated by the computing component:
        // 200000 points for a prediction
        // 10000 points for percentiles
        Tucuxi::Core::SingleOverloadEvaluator::getInstance()->setValues(200000, 10000, 10000);
    }

    Tucuxi::Common::ComponentManager* cmpMgr = Tucuxi::Common::ComponentManager::getInstance();
    Tucuxi::Core::IComputingService* iCore = cmpMgr->getComponent<Tucuxi::Core::IComputingService>("ComputingService");
    for (int i = 0; i < 4; i++) {
        m_caches.emplace_back(std::make_unique<Tucuxi::Core::CacheComputing>(iCore));
    }
}

ProcessingTucucore::~ProcessingTucucore() {
}
Tucuxi::ProcessingResult ProcessingTucucore::points(
        const Tucuxi::Gui::Core::DrugResponseAnalysis* analysis,
        const Tucuxi::Gui::Core::PointsTraits traits,
        Tucuxi::Gui::Core::PredictionResult& prediction
        )
{

    GuiToTucucoreTranslator translator;

    Tucuxi::Core::DrugModel *drugModel = translator.buildDrugModel(analysis->getDrugModel());

    if (drugModel == nullptr) {
        return Tucuxi::ProcessingResult::Failure;
    }

    Tucuxi::Core::DrugTreatment *drugTreatment;
    QDateTime adjTime;
    if (traits.traits->getAdjustmentDate(adjTime)) {
        drugTreatment = translator.buildTreatment(analysis->getTreatment(), adjTime);
    }
    else {
        drugTreatment = translator.buildTreatment(analysis->getTreatment());
    }

    if (drugTreatment == nullptr) {
        return Tucuxi::ProcessingResult::Failure;
    }

    Tucuxi::Core::PredictionParameterType _type = translator.buildParameterType(traits.traits);

    Tucuxi::Core::ComputingOption options(_type,
                                          Tucuxi::Core::CompartmentsOption::MainCompartment,
                                          Tucuxi::Core::RetrieveStatisticsOption::RetrieveStatistics,
                                          Tucuxi::Core::RetrieveParametersOption::RetrieveParameters,
                                          Tucuxi::Core::RetrieveCovariatesOption::RetrieveCovariates,
                                          Tucuxi::Core::ForceUgPerLiterOption::Force);

    bool validDates = true;
    if (!traits.start.isValid()) {
        validDates = false;
        std::cout << "File " << __FILE__ << " . Line " << __LINE__ << ". Unvalid Starting date" << std::endl;
    }
    if (!traits.end.isValid()) {
        validDates = false;
        std::cout << "File " << __FILE__ << " . Line " << __LINE__ << ". Unvalid Ending date" << std::endl;
    }
    if (!validDates) {
        return Tucuxi::ProcessingResult::Failure;
    }

    Tucuxi::Common::DateTime startDate = translator.buildDateTime(traits.start);
    Tucuxi::Common::DateTime endDate = translator.buildDateTime(traits.end);
    double nbPointsPerHour = static_cast<double>(traits.nbPoints) / 24.0;


    // std::cout << "Start time" << std::endl;
    // std::cout << drugTreatment->getDosageHistory().getDosageTimeRanges().at(0)->getStartDate() << std::endl;

    Tucuxi::Common::ComponentManager* cmpMgr = Tucuxi::Common::ComponentManager::getInstance();
    Tucuxi::Core::IComputingService* iCore = cmpMgr->getComponent<Tucuxi::Core::IComputingService>("ComputingService");
    if (iCore != nullptr) {
        m_requestID++;

        // The list of traits
        std::unique_ptr<Tucuxi::Core::ComputingTraits> traits = std::make_unique<Tucuxi::Core::ComputingTraits>();

        // The traits for the curve prediction
        std::unique_ptr<Tucuxi::Core::ComputingTrait> computingTrait = std::make_unique<Tucuxi::Core::ComputingTraitConcentration>(
                    std::to_string(m_requestID),
                    startDate,
                    endDate,
                    nbPointsPerHour,
                    options);

        //traits->addTrait(std::move(computingTrait));

        // The traits for the prediction at measure times
        std::unique_ptr<Tucuxi::Core::ComputingTrait> computingTraitAtMeasure = std::make_unique<Tucuxi::Core::ComputingTraitAtMeasures>(
                    std::to_string(m_requestID),
                    options);

        //traits->addTrait(std::move(computingTraitAtMeasure));

        // TODO : Split that correctly
        Tucuxi::Core::ComputingRequest request(std::to_string(m_requestID), *drugModel, *drugTreatment, std::move(computingTrait));
        std::unique_ptr<Tucuxi::Core::ComputingResponse> response = std::make_unique<Tucuxi::Core::ComputingResponse>(std::to_string(m_requestID));

        Tucuxi::Core::ComputingRequest request1(std::to_string(m_requestID), *drugModel, *drugTreatment, std::move(computingTraitAtMeasure));
        std::unique_ptr<Tucuxi::Core::ComputingResponse> response1 = std::make_unique<Tucuxi::Core::ComputingResponse>(std::to_string(m_requestID));

        TqfLogger::getInstance()->log(request);
        Tucuxi::Core::ComputingStatus res = iCore->compute(request, response);

        checkCovariate(startDate, *drugTreatment, *drugModel, prediction, endDate);

        TqfLogger::getInstance()->log(request1);
        Tucuxi::Core::ComputingStatus res1 = iCore->compute(request1, response1);
        if ((res == Tucuxi::Core::ComputingStatus::Ok) && (res1 == Tucuxi::Core::ComputingStatus::Ok)) {

            TucucoreToGuiTranslator tuToEzTranslator;

            Tucuxi::Gui::Core::FancyPoints* fpts = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::FancyPoints>(ABSTRACTREPO, prediction.getPredictive()->getPredictionData());

            const Tucuxi::Core::SinglePointsData* pPredictionAtSampleTime = dynamic_cast<const Tucuxi::Core::SinglePointsData*>(response1->getData());
            if (pPredictionAtSampleTime != nullptr) {

                Tucuxi::Gui::Core::FancyPoints *pointsAtMeasures = prediction.getPredictive()->getPointsAtMeasures();

                for( size_t i = 0; i < pPredictionAtSampleTime->m_times.size(); i++) {

                    Tucuxi::Gui::Core::FancyPoint* fpt = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::FancyPoint>(ABSTRACTREPO, pointsAtMeasures);

                    fpt->setTime(tuToEzTranslator.buildDateTime(pPredictionAtSampleTime->m_times[i]).toMSecsSinceEpoch() / 1000.0);
                    fpt->setValue(pPredictionAtSampleTime->m_concentrations[0][i]);
                    pointsAtMeasures->append(fpt);
                }
            }

            const Tucuxi::Core::SinglePredictionData* pSinglePred = dynamic_cast<const Tucuxi::Core::SinglePredictionData*>(response->getData());
            if (pSinglePred != nullptr) {

                for (Tucuxi::Core::CycleData cycleData: pSinglePred->getData()) {
                    for (size_t i=0; i<cycleData.m_concentrations[0].size(); i++) {

                        // Create the FancyPoint
                        Tucuxi::Gui::Core::FancyPoint* fpt = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::FancyPoint>(ABSTRACTREPO, fpts);

                        // Get its parameters
                        Tucuxi::Gui::Core::ParameterSet* parameterSet = fpt->getPset();

                        // Populate the parameters list from the one of cycleData
                        for(size_t param = 0; param < cycleData.m_parameters.size(); param++) {
                            Tucuxi::Gui::Core::Parameter* p = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Parameter>(ABSTRACTREPO, parameterSet);
                            p->getQuantity()->setValue(cycleData.m_parameters[param].m_value);
                            p->setName(QString::fromStdString(cycleData.m_parameters[param].m_parameterId));
                            parameterSet->getParameters()->append(p);
                        }
                        fpt->setPset(parameterSet);

                        // This complex next translation is required because of the epoch being different for QDateTime and DateTime, one being local and the other no
                        // fpt time is in seconds since epoch
                        fpt->setTime(tuToEzTranslator.buildDateTime(cycleData.m_start+Tucuxi::Common::Duration(
                                                                        std::chrono::milliseconds(static_cast<int64_t>(cycleData.m_times[0][i] * 3600000.0)))).toMSecsSinceEpoch() / 1000.0);
                        // The following requires at least Qt 5.8
                        //fpt->setTime(tuToEzTranslator.buildDateTime(cycleData.m_start+Tucuxi::Common::Duration(
                        //                                          std::chrono::milliseconds(static_cast<int>(cycleData.m_times[0][i] * 3600000.0)))).toSecsSinceEpoch());
                        fpt->setValue(cycleData.m_concentrations[0][i]);
                        fpts->append(fpt);
                    }

                    // Extract statistics

                    double mean;
                    double auc;
                    double cumulativeAuc;
                    double residual;
                    double peak;
                    Tucuxi::Common::DateTime date;
                    bool ok = true;


                    ok &= cycleData.m_statistics.getStatistic(0, Tucuxi::Core::CycleStatisticType::Mean).getValue(date, mean);
                    ok &= cycleData.m_statistics.getStatistic(0, Tucuxi::Core::CycleStatisticType::AUC).getValue(date, auc);
                    ok &= cycleData.m_statistics.getStatistic(0, Tucuxi::Core::CycleStatisticType::CumulativeAuc).getValue(date, cumulativeAuc);
                    ok &= cycleData.m_statistics.getStatistic(0, Tucuxi::Core::CycleStatisticType::Residual).getValue(date, residual);
                    ok &= cycleData.m_statistics.getStatistic(0, Tucuxi::Core::CycleStatisticType::Peak).getValue(date, peak);

                    if (!ok) {
                        std::cout << "Something went wrong here" << std::endl;
                    }

                    prediction.getPredictive()->getPredictionData()->addStats(
                                tuToEzTranslator.buildDateTime(cycleData.m_start).toMSecsSinceEpoch() / 1000,
                                Tucuxi::Gui::Core::Duration(0,0,0,
                                                         (cycleData.m_end - cycleData.m_start).toMilliseconds()).toHours(),
                                mean, auc, cumulativeAuc, residual, peak
                                );


                }
            }

            prediction.getPredictive()->getPredictionData()->setPoints(fpts);

            return Tucuxi::ProcessingResult::Success;
        }
    }
    return Tucuxi::ProcessingResult::Failure;
}

Tucuxi::ProcessingResult ProcessingTucucore::point(
        const Tucuxi::Gui::Core::DrugResponseAnalysis* analysis,
        const Tucuxi::Gui::Core::PointTraits traits,
        Tucuxi::Gui::Core::Amount &amount,
        Tucuxi::Gui::Core::PredictionResult& prediction)
{
    // TODO : To be implemented
    return Tucuxi::ProcessingResult::Success;
}

Tucuxi::ProcessingResult ProcessingTucucore::percentiles(
        const Tucuxi::Gui::Core::DrugResponseAnalysis* analysis,
        const Tucuxi::Gui::Core::PercentilesTraits traits,
        Tucuxi::Gui::Core::PredictionResult& prediction,
        Tucuxi::math::ProcessingAborter *aborter)
{
    return generalCalculatePercentiles(analysis, traits, prediction, aborter);
}


Tucuxi::ProcessingResult ProcessingTucucore::calculateAposterioriPercentiles(
        const Tucuxi::Gui::Core::DrugResponseAnalysis* analysis,
        const Tucuxi::Gui::Core::PercentilesTraits traits,
        Tucuxi::Gui::Core::PredictionResult& prediction,
        Tucuxi::math::ProcessingAborter *aborter)
{
    return generalCalculatePercentiles(analysis, traits, prediction, aborter);
}



Tucuxi::ProcessingResult ProcessingTucucore::generalCalculatePercentiles(
        const Tucuxi::Gui::Core::DrugResponseAnalysis* analysis,
        const Tucuxi::Gui::Core::PercentilesTraits traits,
        Tucuxi::Gui::Core::PredictionResult &prediction,
        Tucuxi::math::ProcessingAborter *aborter)
{


    GuiToTucucoreTranslator translator;

    Tucuxi::Core::DrugModel *drugModel = translator.buildDrugModel(analysis->getDrugModel());

    if (drugModel == nullptr) {
        return Tucuxi::ProcessingResult::Failure;
    }


    Tucuxi::Core::DrugTreatment *drugTreatment;
    QDateTime adjTime;
    if (traits.traits->getAdjustmentDate(adjTime)) {
        drugTreatment = translator.buildTreatment(analysis->getTreatment(), adjTime);
    }
    else {
        drugTreatment = translator.buildTreatment(analysis->getTreatment());
    }

//    Tucuxi::Core::DrugTreatment *drugTreatment;
//    drugTreatment = translator.buildTreatment(analysis->getTreatment());


    if (drugTreatment == nullptr) {
        return Tucuxi::ProcessingResult::Failure;
    }


    Tucuxi::Common::ComponentManager* cmpMgr = Tucuxi::Common::ComponentManager::getInstance();
    Tucuxi::Core::IComputingService* iCore = cmpMgr->getComponent<Tucuxi::Core::IComputingService>("ComputingService");
    if (iCore != nullptr) {

        m_requestID++;

        Tucuxi::Core::PercentileRanks ranks(traits.percs.size());
        for(unsigned int i = 0; i < traits.percs.size(); i++)
            ranks[i] = traits.percs[i];


        Tucuxi::Core::PredictionParameterType type = translator.buildParameterType(traits.traits);

        Tucuxi::Core::ComputingOption options(type, Tucuxi::Core::CompartmentsOption::MainCompartment,
                                              Tucuxi::Core::RetrieveStatisticsOption::DoNotRetrieveStatistics,
                                              Tucuxi::Core::RetrieveParametersOption::DoNotRetrieveParameters,
                                              Tucuxi::Core::RetrieveCovariatesOption::DoNotRetrieveCovariates,
                                              Tucuxi::Core::ForceUgPerLiterOption::Force);

        Tucuxi::Common::DateTime startDate = translator.buildDateTime(traits.start);
        Tucuxi::Common::DateTime endDate = translator.buildDateTime(traits.end);
        int nbPointsPerHour = traits.nbPoints / 24.0;

        auto duration = endDate - startDate;
        double durationInHours = static_cast<double>(duration.toSeconds()) / 3600.0;
        if ( durationInHours * static_cast<double>(nbPointsPerHour) > 5000.0) {

            nbPointsPerHour = 4;
//            nbPointsPerHour = static_cast<int>(5000.0 / durationInHours);
        }

        std::unique_ptr<Tucuxi::Core::ComputingTrait> computingTrait = std::make_unique<Tucuxi::Core::ComputingTraitPercentiles>(
                    std::to_string(m_requestID),
                    startDate,
                    endDate,
                    ranks,
                    nbPointsPerHour,
                    options,
                    static_cast<Tucuxi::Gui::GuiUtils::MixAborter*>(aborter)->getTucuxiAborter());

        Tucuxi::Core::ComputingRequest request(std::to_string(m_requestID), *drugModel, *drugTreatment, std::move(computingTrait));
        std::unique_ptr<Tucuxi::Core::ComputingResponse> response = std::make_unique<Tucuxi::Core::ComputingResponse>(std::to_string(m_requestID));

        Tucuxi::Core::CacheComputing *cache = nullptr;
        cache = m_caches[traits.cacheId].get();

        if (traits.clearCache) {
            cache->clear();
        }
        TqfLogger::getInstance()->log(request);
        Tucuxi::Core::ComputingStatus res = cache->compute(request, response);
//        Tucuxi::Core::ComputingStatus res = iCore->compute(request, response);

        checkCovariate(startDate, *drugTreatment, *drugModel, prediction, endDate);

        if (res == Tucuxi::Core::ComputingStatus::Ok) {

            TucucoreToGuiTranslator tuToEzTranslator;

            Tucuxi::Gui::Core::PercentileDataList* percpairs = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PercentileDataList>(ABSTRACTREPO, prediction.getPredictive());
            prediction.getPredictive()->setPercentileDataList(percpairs);

            const Tucuxi::Core::PercentilesData* pPercentiles = dynamic_cast<const Tucuxi::Core::PercentilesData*>(response->getData());

            if (pPercentiles != nullptr) {
                for (size_t percIndex = 0; percIndex < pPercentiles->getNbRanks(); ++percIndex) {
                    Tucuxi::Gui::Core::PercentileData* percpair = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PercentileData>(ABSTRACTREPO, prediction.getPredictive());
                    percpairs->append(percpair);
                    Tucuxi::Gui::Core::PredictionData* pdata = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PredictionData>(ABSTRACTREPO, percpair);
                    Tucuxi::Gui::Core::FancyPoints* fpts = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::FancyPoints>(ABSTRACTREPO);
                    fpts->setParent(pdata);
                    pdata->setPoints(fpts);
                    percpair->setPredictionData(pdata);
                    percpair->setPercentile(pPercentiles->getRank(percIndex));
                    const std::vector<Tucuxi::Core::CycleData>& cycleDatas = pPercentiles->getPercentileData(percIndex);
                    for (size_t cycleIndex=0; cycleIndex < cycleDatas.size(); cycleIndex++) {
                        const Tucuxi::Core::CycleData& cycleData = cycleDatas.at(cycleIndex);
                        for (size_t pointIndex=0; pointIndex<cycleData.m_concentrations[0].size(); pointIndex++) {
                            Tucuxi::Gui::Core::FancyPoint* fpt = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::FancyPoint>(ABSTRACTREPO, fpts);
                            fpt->setTime(tuToEzTranslator.buildDateTime(cycleData.m_start+Tucuxi::Common::Duration(
                                                                            std::chrono::milliseconds(static_cast<int64>(cycleData.m_times[0][pointIndex] * 3600000.0)))).toMSecsSinceEpoch() / 1000.0);
                            // The following requires at least Qt 5.8
                            //fpt->setTime(tuToEzTranslator.buildDateTime(cycleData.m_start+Tucuxi::Common::Duration(
                            //                                          std::chrono::milliseconds(static_cast<int>(cycleData.m_times[0][pointIndex] * 3600000.0)))).toSecsSinceEpoch());

                            //fpt->setTime(cycleData.m_start.toSeconds()+cycleData.m_times[0][pointIndex]*3600.0);
                            fpt->setValue(cycleData.m_concentrations[0][pointIndex]);
                            fpts->append(fpt);
                        }
                    }
                }
            }

            return Tucuxi::ProcessingResult::Success;
        }
    }
    return Tucuxi::ProcessingResult::Failure;
}


#include <iostream>

using namespace std::chrono_literals;
using namespace date;






Tucuxi::ProcessingResult ProcessingTucucore::computeSuggestedAdjustments(
        const Tucuxi::Gui::Core::DrugResponseAnalysis* analysis,
        const Tucuxi::Gui::Core::ReverseTraits traits,
        Tucuxi::Gui::Core::PredictionResult& prediction)
{

    GuiToTucucoreTranslator translator;

    Tucuxi::Core::DrugModel *drugModel = translator.buildDrugModel(analysis->getDrugModel());

    if (drugModel == nullptr) {
        return Tucuxi::ProcessingResult::Failure;
    }

    Tucuxi::Core::DrugTreatment *drugTreatment;
    drugTreatment = translator.buildTreatment(analysis->getTreatment());

    if (drugTreatment == nullptr) {
        return Tucuxi::ProcessingResult::Failure;
    }


    Tucuxi::Common::ComponentManager* cmpMgr = Tucuxi::Common::ComponentManager::getInstance();
    Tucuxi::Core::IComputingService* iCore = cmpMgr->getComponent<Tucuxi::Core::IComputingService>("ComputingService");
    if (iCore == nullptr) {
        return Tucuxi::ProcessingResult::Failure;
    }
    m_requestID++;

    // TODO : Construct the adjustment traits object
    Tucuxi::Core::RequestResponseId requestResponseId = std::to_string(m_requestID);
    Tucuxi::Common::DateTime start = translator.buildDateTime(traits.start);
    Tucuxi::Common::DateTime end = translator.buildDateTime(traits.end);

    // OK, this is a bit strange. The proto should also use a density instead of a cycleSize
    double nbPointsPerHour = traits.nbPoints / 24.0;

    Tucuxi::Core::ComputingOption computingOption(Tucuxi::Core::PredictionParameterType::Population,
                                                  Tucuxi::Core::CompartmentsOption::MainCompartment,
                                                  Tucuxi::Core::RetrieveStatisticsOption::RetrieveStatistics,
                                                  Tucuxi::Core::RetrieveParametersOption::RetrieveParameters,
                                                  Tucuxi::Core::RetrieveCovariatesOption::RetrieveCovariates,
                                                  Tucuxi::Core::ForceUgPerLiterOption::Force);
    if (analysis->getTreatment()->getMeasures()->size() != 0) {
        computingOption = Tucuxi::Core::ComputingOption(Tucuxi::Core::PredictionParameterType::Aposteriori,
                                                         Tucuxi::Core::CompartmentsOption::MainCompartment,
                                                        Tucuxi::Core::RetrieveStatisticsOption::DoNotRetrieveStatistics,
                                                        Tucuxi::Core::RetrieveParametersOption::DoNotRetrieveParameters,
                                                        Tucuxi::Core::RetrieveCovariatesOption::DoNotRetrieveCovariates,
                                                        Tucuxi::Core::ForceUgPerLiterOption::Force);
    }
    else if (analysis->getTreatment()->getCovariates()->size() != 0) {
        computingOption = Tucuxi::Core::ComputingOption(Tucuxi::Core::PredictionParameterType::Apriori,
                                                         Tucuxi::Core::CompartmentsOption::MainCompartment,
                                                        Tucuxi::Core::RetrieveStatisticsOption::DoNotRetrieveStatistics,
                                                        Tucuxi::Core::RetrieveParametersOption::DoNotRetrieveParameters,
                                                        Tucuxi::Core::RetrieveCovariatesOption::DoNotRetrieveCovariates,
                                                        Tucuxi::Core::ForceUgPerLiterOption::Force);
    }

    Tucuxi::Core::SteadyStateTargetOption steadyStateOption = Tucuxi::Core::SteadyStateTargetOption::AtSteadyState;
    if (drugModel->getFormulationAndRoutes().getDefault()->getStandardTreatment() != nullptr) {
        if (drugModel->getFormulationAndRoutes().getDefault()->getStandardTreatment()->getIsFixedDuration()) {
            steadyStateOption = Tucuxi::Core::SteadyStateTargetOption::WithinTreatmentTimeRange;
        }
    }

    Tucuxi::Core::LoadingOption loadingDoseOption =
            traits.adjWithLoadingDose ? Tucuxi::Core::LoadingOption::LoadingDoseAllowed :
                                        Tucuxi::Core::LoadingOption::NoLoadingDose;

    Tucuxi::Core::RestPeriodOption restPeriodOption =
            traits.adjWithRestPeriod ? Tucuxi::Core::RestPeriodOption::RestPeriodAllowed :
                                       Tucuxi::Core::RestPeriodOption::NoRestPeriod;

    Tucuxi::Common::DateTime adjustmentTime = translator.buildDateTime(traits.time);
    Tucuxi::Core::BestCandidatesOption adjustmentOption = Tucuxi::Core::BestCandidatesOption::BestDosagePerInterval;
    std::unique_ptr<Tucuxi::Core::ComputingTraitAdjustment> adjustmentsTraits =
            std::make_unique<Tucuxi::Core::ComputingTraitAdjustment>(
                requestResponseId, start, end, nbPointsPerHour, computingOption, adjustmentTime,
                adjustmentOption,
                loadingDoseOption,
                restPeriodOption,
                steadyStateOption,
                Tucuxi::Core::TargetExtractionOption::IndividualTargets,
//                Tucuxi::Core::TargetExtractionOption::DefinitionIfNoIndividualTarget,
                Tucuxi::Core::FormulationAndRouteSelectionOption::LastFormulationAndRoute);
    // std::cout << "Adjustment time" << std::endl;
    // std::cout << adjustmentTime << std::endl;
    // std::cout << start << std::endl;
    // std::cout << end << std::endl;

    Tucuxi::Core::ComputingRequest request(requestResponseId, *drugModel, *drugTreatment, std::move(adjustmentsTraits));

    std::unique_ptr<Tucuxi::Core::ComputingResponse> response = std::make_unique<Tucuxi::Core::ComputingResponse>(requestResponseId);

    Tucuxi::Core::ComputingStatus result;
    TqfLogger::getInstance()->log(request);
    result = iCore->compute(request, response);

    checkCovariate(start, *drugTreatment, *drugModel, prediction, end);

    if (result != Tucuxi::Core::ComputingStatus::Ok) {
        return Tucuxi::ProcessingResult::Failure;
    }

    const Tucuxi::Core::AdjustmentData *resp = dynamic_cast<const Tucuxi::Core::AdjustmentData*>(response->getData());
    if (resp == nullptr) {
        std::cout << "Received an unexpected response instead of Adjustment response" << std::endl;
    }


    TucucoreToGuiTranslator tuToEzTranslator;

    // Process the adjustment
    for (const auto & fullDosage : resp->getAdjustments()) {

        TucucoreToGuiTranslator translator;

        Tucuxi::Gui::Core::Adjustment *adjustment = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::Adjustment>(ABSTRACTREPO, prediction.getAdjustments());

        Tucuxi::Gui::Core::DosageHistory *dosageHistory = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::DosageHistory>(ABSTRACTREPO, adjustment);

        if (!translator.buildDosageHistory(fullDosage.m_history, dosageHistory)) {
            std::cout << "Could not build a dosage history from the computed one" << std::endl;
        }

        adjustment->setDosageHistory(dosageHistory);
        adjustment->setScore(fullDosage.getGlobalScore());
        for (const auto & targetEvaluations : fullDosage.m_targetsEvaluation) {
            //targetEvaluations
            Tucuxi::Gui::Core::TargetEvaluationResult *targetResult = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::TargetEvaluationResult>(ABSTRACTREPO, adjustment);

            translator.buildTargetEvaluation(&targetEvaluations, targetResult);
            adjustment->getTargetEvaluationResults()->append(targetResult);

        }

        // data:
        Tucuxi::Gui::Core::FancyPoints* fpts = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::FancyPoints>(ABSTRACTREPO, adjustment->getPredictionData());

        for (Tucuxi::Core::CycleData cycleData: fullDosage.getData()) {
            for (size_t i=0; i<cycleData.m_concentrations[0].size(); i++) {
                Tucuxi::Gui::Core::FancyPoint* fpt = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::FancyPoint>(ABSTRACTREPO, fpts);
                //fpt->setTime(cycleData.m_start.toSeconds()+cycleData.m_times[0][i]*3600.0);// This complex next translation is required because of the epoch being different for QDateTime and DateTime, one being local and the other no
                // fpt time is in seconds since epoch
                fpt->setTime(tuToEzTranslator.buildDateTime(cycleData.m_start+Tucuxi::Common::Duration(
                                                                std::chrono::milliseconds(static_cast<int>(cycleData.m_times[0][i] * 3600000.0)))).toMSecsSinceEpoch() / 1000.0);
                // The following requires Qt 5.8
                //fpt->setTime(tuToEzTranslator.buildDateTime(cycleData.m_start+Tucuxi::Common::Duration(
                //                                                std::chrono::milliseconds(static_cast<int>(cycleData.m_times[0][i] * 3600000.0)))).toSecsSinceEpoch());

                fpt->setValue(cycleData.m_concentrations[0][i]);
                fpts->append(fpt);
            }
        }

        adjustment->getPredictionData()->setPoints(fpts);

        prediction.getAdjustments()->append(adjustment);
    }

    return Tucuxi::ProcessingResult::Success;
}


