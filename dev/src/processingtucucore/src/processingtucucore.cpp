#include <iostream>

#include "processingtucucore.h"


#include "eztotucucoretranslator.h"
#include "tucucoretoeztranslator.h"

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
}

ProcessingTucucore::~ProcessingTucucore() {
}

ezechiel::ProcessingResult ProcessingTucucore::points(
        const ezechiel::core::DrugResponseAnalysis* analysis,
        const ezechiel::core::PointsTraits traits,
        ezechiel::core::PredictionResult& prediction
        )
{


    EzToTucucoreTranslator translator;

    Tucuxi::Core::DrugModel *drugModel = translator.buildDrugModel(analysis->getDrugModel());

    if (drugModel == nullptr) {
        return ezechiel::ProcessingResult::Failure;
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
        return ezechiel::ProcessingResult::Failure;
    }

    Tucuxi::Core::PredictionParameterType _type = translator.buildParameterType(traits.traits);

    Tucuxi::Core::ComputingOption options(_type,
                                          Tucuxi::Core::CompartmentsOption::MainCompartment,
                                          Tucuxi::Core::RetrieveStatisticsOption::RetrieveStatistics,
                                          Tucuxi::Core::RetrieveParametersOption::RetrieveParameters,
                                          Tucuxi::Core::RetrieveCovariatesOption::RetrieveCovariates);

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
        return ezechiel::ProcessingResult::Failure;
    }

    Tucuxi::Common::DateTime _startDate = translator.buildDateTime(traits.start);
    Tucuxi::Common::DateTime _endDate = translator.buildDateTime(traits.end);
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
                    _startDate,
                    _endDate,
                    nbPointsPerHour,
                    options);

        traits->addTrait(std::move(computingTrait));

        // The traits for the prediction at measure times
        std::unique_ptr<Tucuxi::Core::ComputingTrait> computingTraitAtMeasure = std::make_unique<Tucuxi::Core::ComputingTraitAtMeasures>(
                    std::to_string(m_requestID),
                    options);

        traits->addTrait(std::move(computingTraitAtMeasure));

        Tucuxi::Core::ComputingRequest request(std::to_string(m_requestID), *drugModel, *drugTreatment, std::move(traits));
        std::unique_ptr<Tucuxi::Core::ComputingResponse> response = std::make_unique<Tucuxi::Core::ComputingResponse>(std::to_string(m_requestID));

        Tucuxi::Core::ComputingResult res = iCore->compute(request, response);
        if (res == Tucuxi::Core::ComputingResult::Ok) {

            TucucoreToEzTranslator tuToEzTranslator;

            ezechiel::core::FancyPoints* fpts = ezechiel::core::CoreFactory::createEntity<ezechiel::core::FancyPoints>(ABSTRACTREPO, prediction.getPredictive()->getPredictionData());

            for (const std::unique_ptr<Tucuxi::Core::SingleComputingResponse>& resp: response->getResponses()) {
                Tucuxi::Core::SinglePointsResponse* pPredictionAtSampleTime = dynamic_cast<Tucuxi::Core::SinglePointsResponse*>(resp.get());
                if (pPredictionAtSampleTime != nullptr) {

                    ezechiel::core::FancyPoints *pointsAtMeasures = prediction.getPredictive()->getPointsAtMeasures();

                    for( size_t i = 0; i < pPredictionAtSampleTime->m_times.size(); i++) {

                        ezechiel::core::FancyPoint* fpt = ezechiel::core::CoreFactory::createEntity<ezechiel::core::FancyPoint>(ABSTRACTREPO, pointsAtMeasures);

                        fpt->setTime(tuToEzTranslator.buildDateTime(pPredictionAtSampleTime->m_times[i]).toMSecsSinceEpoch() / 1000.0);
                        fpt->setValue(pPredictionAtSampleTime->m_concentrations[0][i]);
                        pointsAtMeasures->append(fpt);
                    }
                }

                Tucuxi::Core::SinglePredictionResponse* pSinglePred = dynamic_cast<Tucuxi::Core::SinglePredictionResponse*>(resp.get());
                if (pSinglePred != nullptr) {

                    for (Tucuxi::Core::CycleData cycleData: pSinglePred->getData()) {
                        for (size_t i=0; i<cycleData.m_concentrations[0].size(); i++) {

                            // Create the FancyPoint
                            ezechiel::core::FancyPoint* fpt = ezechiel::core::CoreFactory::createEntity<ezechiel::core::FancyPoint>(ABSTRACTREPO, fpts);

                            // Get its parameters
                            ezechiel::core::ParameterSet* parameterSet = fpt->getPset();

                            // Populate the parameters list from the one of cycleData
                            for(size_t param = 0; param < cycleData.m_parameters.size(); param++) {
                                ezechiel::core::Parameter* p = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Parameter>(ABSTRACTREPO, parameterSet);
                                p->getQuantity()->setValue(cycleData.m_parameters[param].m_value);
                                p->setName(QString::fromStdString(cycleData.m_parameters[param].m_parameterId));
                                parameterSet->getParameters()->append(p);
                            }
                            fpt->setPset(parameterSet);

                            // This complex next translation is required because of the epoch being different for QDateTime and DateTime, one being local and the other no
                            // fpt time is in seconds since epoch
                            fpt->setTime(tuToEzTranslator.buildDateTime(cycleData.m_start+Tucuxi::Common::Duration(
                                                                      std::chrono::milliseconds(static_cast<int>(cycleData.m_times[0][i] * 3600000.0)))).toMSecsSinceEpoch() / 1000.0);
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
                                    ezechiel::core::Duration(0,0,0,
                                                             (cycleData.m_end - cycleData.m_start).toMilliseconds()).toHours(),
                                mean, auc, cumulativeAuc, residual, peak
                                );


                    }
                }
            }

            prediction.getPredictive()->getPredictionData()->setPoints(fpts);

            return ezechiel::ProcessingResult::Success;
        }
    }
    return ezechiel::ProcessingResult::Failure;
}

ezechiel::ProcessingResult ProcessingTucucore::point(
        const ezechiel::core::DrugResponseAnalysis* analysis,
        const ezechiel::core::PointTraits traits,
        ezechiel::core::Amount &amount,
        ezechiel::core::PredictionResult& prediction)
{
    // TODO : To be implemented
    return ezechiel::ProcessingResult::Success;
}

ezechiel::ProcessingResult ProcessingTucucore::percentiles(
        const ezechiel::core::DrugResponseAnalysis* analysis,
        const ezechiel::core::PercentilesTraits traits,
        ezechiel::core::PredictionResult& prediction,
        ezechiel::math::ProcessingAborter *aborter)
{
    return generalCalculatePercentiles(analysis, traits, prediction, aborter);
}


ezechiel::ProcessingResult ProcessingTucucore::calculateAposterioriPercentiles(
        const ezechiel::core::DrugResponseAnalysis* analysis,
        const ezechiel::core::PercentilesTraits traits,
        ezechiel::core::PredictionResult& prediction,
        ezechiel::math::ProcessingAborter *aborter)
{
    return generalCalculatePercentiles(analysis, traits, prediction, aborter);
}



ezechiel::ProcessingResult ProcessingTucucore::generalCalculatePercentiles(
        const ezechiel::core::DrugResponseAnalysis* analysis,
        const ezechiel::core::PercentilesTraits traits,
        ezechiel::core::PredictionResult &prediction,
        ezechiel::math::ProcessingAborter *aborter)
{


    EzToTucucoreTranslator translator;

    Tucuxi::Core::DrugModel *drugModel = translator.buildDrugModel(analysis->getDrugModel());

    if (drugModel == nullptr) {
        return ezechiel::ProcessingResult::Failure;
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
        return ezechiel::ProcessingResult::Failure;
    }


    Tucuxi::Common::ComponentManager* cmpMgr = Tucuxi::Common::ComponentManager::getInstance();
    Tucuxi::Core::IComputingService* iCore = cmpMgr->getComponent<Tucuxi::Core::IComputingService>("ComputingService");
    if (iCore != nullptr) {
        m_requestID++;

        Tucuxi::Core::PercentileRanks ranks(traits.percs.size());
        for(unsigned int i = 0; i < traits.percs.size(); i++)
            ranks[i] = traits.percs[i];


        Tucuxi::Core::PredictionParameterType _type = translator.buildParameterType(traits.traits);

        Tucuxi::Core::ComputingOption options(_type, Tucuxi::Core::CompartmentsOption::MainCompartment);

        Tucuxi::Common::DateTime _startDate = translator.buildDateTime(traits.start);
        Tucuxi::Common::DateTime _endDate = translator.buildDateTime(traits.end);
        int nbPointsPerHour = traits.nbPoints / 24.0;

        auto duration = _endDate - _startDate;
        double durationInHours = static_cast<double>(duration.toSeconds()) / 3600.0;
        if ( durationInHours * static_cast<double>(nbPointsPerHour) > 5000.0) {

            nbPointsPerHour = 4;
//            nbPointsPerHour = static_cast<int>(5000.0 / durationInHours);
        }


        std::unique_ptr<Tucuxi::Core::ComputingTrait> computingTrait = std::make_unique<Tucuxi::Core::ComputingTraitPercentiles>(
                    std::to_string(m_requestID),
                    _startDate,
                    _endDate,
                    ranks,
                    nbPointsPerHour,
                    options,
                    static_cast<MixAborter*>(aborter)->getTucuxiAborter());

        std::unique_ptr<Tucuxi::Core::ComputingTraits> traits = std::make_unique<Tucuxi::Core::ComputingTraits>();
        traits->addTrait(std::move(computingTrait));
        Tucuxi::Core::ComputingRequest request(std::to_string(m_requestID), *drugModel, *drugTreatment, std::move(traits));
        std::unique_ptr<Tucuxi::Core::ComputingResponse> response = std::make_unique<Tucuxi::Core::ComputingResponse>(std::to_string(m_requestID));

        Tucuxi::Core::ComputingResult res = iCore->compute(request, response);
        if (res == Tucuxi::Core::ComputingResult::Ok) {

            TucucoreToEzTranslator tuToEzTranslator;

            ezechiel::core::PercentileDataList* percpairs = ezechiel::core::CoreFactory::createEntity<ezechiel::core::PercentileDataList>(ABSTRACTREPO, prediction.getPredictive());
            prediction.getPredictive()->setPercentileDataList(percpairs);


            for (const std::unique_ptr<Tucuxi::Core::SingleComputingResponse>& resp: response->getResponses()) {
                Tucuxi::Core::PercentilesResponse* pPercentiles = dynamic_cast<Tucuxi::Core::PercentilesResponse*>(resp.get());
                if (pPercentiles != nullptr) {
                    for (size_t percIndex = 0; percIndex < pPercentiles->getNbRanks(); ++percIndex) {
                        ezechiel::core::PercentileData* percpair = ezechiel::core::CoreFactory::createEntity<ezechiel::core::PercentileData>(ABSTRACTREPO, prediction.getPredictive());
                        percpairs->append(percpair);
                        ezechiel::core::PredictionData* pdata = ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionData>(ABSTRACTREPO, percpair);
                        ezechiel::core::FancyPoints* fpts = ezechiel::core::CoreFactory::createEntity<ezechiel::core::FancyPoints>(ABSTRACTREPO);
                        fpts->setParent(pdata);
                        pdata->setPoints(fpts);
                        percpair->setPredictionData(pdata);
                        percpair->setPercentile(pPercentiles->getRank(percIndex));
                        const std::vector<Tucuxi::Core::CycleData>& cycleDatas = pPercentiles->getPercentileData(percIndex);
                        for (size_t cycleIndex=0; cycleIndex < cycleDatas.size(); cycleIndex++) {
                            const Tucuxi::Core::CycleData& cycleData = cycleDatas.at(cycleIndex);
                            for (size_t pointIndex=0; pointIndex<cycleData.m_concentrations[0].size(); pointIndex++) {
                                ezechiel::core::FancyPoint* fpt = ezechiel::core::CoreFactory::createEntity<ezechiel::core::FancyPoint>(ABSTRACTREPO, fpts);
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
            }

            return ezechiel::ProcessingResult::Success;
        }
    }
    return ezechiel::ProcessingResult::Failure;
}


#include <iostream>

using namespace std::chrono_literals;
using namespace date;






ezechiel::ProcessingResult ProcessingTucucore::computeSuggestedAdjustments(
        const ezechiel::core::DrugResponseAnalysis* analysis,
        const ezechiel::core::ReverseTraits traits,
        ezechiel::core::PredictionResult& prediction)
{

    EzToTucucoreTranslator translator;

    Tucuxi::Core::DrugModel *drugModel = translator.buildDrugModel(analysis->getDrugModel());

    if (drugModel == nullptr) {
        return ezechiel::ProcessingResult::Failure;
    }

    Tucuxi::Core::DrugTreatment *drugTreatment;
    drugTreatment = translator.buildTreatment(analysis->getTreatment());

    if (drugTreatment == nullptr) {
        return ezechiel::ProcessingResult::Failure;
    }


    Tucuxi::Common::ComponentManager* cmpMgr = Tucuxi::Common::ComponentManager::getInstance();
    Tucuxi::Core::IComputingService* iCore = cmpMgr->getComponent<Tucuxi::Core::IComputingService>("ComputingService");
    if (iCore == nullptr) {
        return ezechiel::ProcessingResult::Failure;
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
                                                  Tucuxi::Core::RetrieveCovariatesOption::RetrieveCovariates);
    if (analysis->getTreatment()->getMeasures()->size() != 0) {
        computingOption = Tucuxi::Core::ComputingOption(Tucuxi::Core::PredictionParameterType::Aposteriori,
                                                         Tucuxi::Core::CompartmentsOption::MainCompartment);
    }
    else if (analysis->getTreatment()->getCovariates()->size() != 0) {
        computingOption = Tucuxi::Core::ComputingOption(Tucuxi::Core::PredictionParameterType::Apriori,
                                                         Tucuxi::Core::CompartmentsOption::MainCompartment);
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
                Tucuxi::Core::TargetExtractionOption::DefinitionIfNoIndividualTarget,
                Tucuxi::Core::FormulationAndRouteSelectionOption::LastFormulationAndRoute);
    // std::cout << "Adjustment time" << std::endl;
    // std::cout << adjustmentTime << std::endl;
    // std::cout << start << std::endl;
    // std::cout << end << std::endl;

    std::unique_ptr<Tucuxi::Core::ComputingTraits> computingTraits = std::make_unique<Tucuxi::Core::ComputingTraits>();
    computingTraits->addTrait(std::move(adjustmentsTraits));

    Tucuxi::Core::ComputingRequest request(requestResponseId, *drugModel, *drugTreatment, std::move(computingTraits));

    std::unique_ptr<Tucuxi::Core::ComputingResponse> response = std::make_unique<Tucuxi::Core::ComputingResponse>(requestResponseId);

    Tucuxi::Core::ComputingResult result;
    result = iCore->compute(request, response);

    if (result != Tucuxi::Core::ComputingResult::Ok) {
        return ezechiel::ProcessingResult::Failure;
    }

    const std::vector<std::unique_ptr<Tucuxi::Core::SingleComputingResponse> > &responses = response.get()->getResponses();
    for(std::size_t i = 0; i < responses.size(); i++) {
        const Tucuxi::Core::AdjustmentResponse *resp = dynamic_cast<Tucuxi::Core::AdjustmentResponse*>(responses[i].get());
        if (resp == nullptr) {
            std::cout << "Received an unexpected response instead of Adjustment response" << std::endl;
        }


        TucucoreToEzTranslator tuToEzTranslator;

        // Process the adjustment
        for (const auto & fullDosage : resp->getAdjustments()) {

            TucucoreToEzTranslator translator;

            ezechiel::core::Adjustment *adjustment = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Adjustment>(ABSTRACTREPO, prediction.getAdjustments());

            ezechiel::core::DosageHistory *dosageHistory = ezechiel::core::CoreFactory::createEntity<ezechiel::core::DosageHistory>(ABSTRACTREPO, adjustment);

            if (!translator.buildDosageHistory(fullDosage.m_history, dosageHistory)) {
                std::cout << "Could not build a dosage history from the computed one" << std::endl;
            }

            adjustment->setDosageHistory(dosageHistory);
            adjustment->setScore(fullDosage.getGlobalScore());
            for (const auto & targetEvaluations : fullDosage.m_targetsEvaluation) {
                //targetEvaluations
                ezechiel::core::TargetEvaluationResult *targetResult = ezechiel::core::CoreFactory::createEntity<ezechiel::core::TargetEvaluationResult>(ABSTRACTREPO, adjustment);

                translator.buildTargetEvaluation(&targetEvaluations, targetResult);
                adjustment->getTargetEvaluationResults()->append(targetResult);

            }

            // data:
            ezechiel::core::FancyPoints* fpts = ezechiel::core::CoreFactory::createEntity<ezechiel::core::FancyPoints>(ABSTRACTREPO,adjustment->getPredictionData());
            adjustment->getPredictionData()->setPoints(fpts);

            for (Tucuxi::Core::CycleData cycleData: fullDosage.m_data) {
                for (size_t i=0; i<cycleData.m_concentrations[0].size(); i++) {
                    ezechiel::core::FancyPoint* fpt = ezechiel::core::CoreFactory::createEntity<ezechiel::core::FancyPoint>(ABSTRACTREPO, fpts);
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

            prediction.getAdjustments()->append(adjustment);

        }



    }

    return ezechiel::ProcessingResult::Success;
}