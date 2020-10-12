#include <iostream>

#include "chartdatacontroller.h"
#include "core/dal/chartdata.h"
#include "core/corefactory.h"
#include "core/dal/predictionresult.h"
#include "core/dal/predictionspec.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/drug/drug.h"
#include "core/dal/drug/drugvariate.h"
#include "core/dal/coremeasure.h"
#include "core/dal/covariate.h"
#include "calculationcontroller.h"
#include "core/utils/logging.h"
#include "errors_guiutils.h"
#include "core/utils/connect.h"
#include "core/dal/drug/parameters.h"

ChartDataController::ChartDataController(QObject *parent) : QObject(parent),
    _currentTab(StepType::Patient),
    calculationController(nullptr)
{
}

void ChartDataController::setCalculationController(CalculationController *controller)
{
    calculationController = controller;

    CONNECT(calculationController, SIGNAL(popPredComputed(ezechiel::core::PredictionResult*)), this, SLOT(receiveResultsPopPred(ezechiel::core::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(popPercComputed(ezechiel::core::PredictionResult*)), this, SLOT(receiveResultsPopPerc(ezechiel::core::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(aprPredComputed(ezechiel::core::PredictionResult*)), this, SLOT(receiveResultsAprPred(ezechiel::core::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(aprPercComputed(ezechiel::core::PredictionResult*)), this, SLOT(receiveResultsAprPerc(ezechiel::core::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(apoPredComputed(ezechiel::core::PredictionResult*)), this, SLOT(receiveResultsApoPred(ezechiel::core::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(apoPercComputed(ezechiel::core::PredictionResult*)), this, SLOT(receiveResultsApoPerc(ezechiel::core::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(revPredComputed(ezechiel::core::PredictionResult*)), this, SLOT(receiveResultsRevPred(ezechiel::core::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(adjPredComputed(ezechiel::core::PredictionResult*)), this, SLOT(receiveResultsAdjPred(ezechiel::core::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(adjPercComputed(ezechiel::core::PredictionResult*)), this, SLOT(receiveResultsAdjPerc(ezechiel::core::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(popCalcFail()), this, SLOT(popCalcFailed()));
    CONNECT(calculationController, SIGNAL(aprCalcFail()), this, SLOT(aprCalcFailed()));
    CONNECT(calculationController, SIGNAL(apoCalcFail()), this, SLOT(apoCalcFailed()));
    CONNECT(calculationController, SIGNAL(adjCalcFail()), this, SLOT(adjCalcFailed()));
}

void ChartDataController::setCurrentTab(StepType::Enum step)
{
    _currentTab = step;
}

void ChartDataController::initiateUpdatePopPred()
{
    if (!isPopulationValid()) {
        chartData->setPopPred(ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionResult>(ABSTRACTREPO, chartData));
        return;
    }
    calculationController->computePopPred(predictionspec);
}

void ChartDataController::receiveResultsPopPred(ezechiel::core::PredictionResult* pred) {
    pred->setParent(chartData);
    // Here we set the prediction result, and as a result, it erases the percentiles.
    // It is a side effect that could be handled differently. There is also a memory leak here.
    chartData->setPopPred(pred);
}

void ChartDataController::initiateUpdatePopPerc() {
#ifdef DISABLEPERCS
    return;
#endif
    if (!isPopulationValid()) {
        chartData->setPopPred(ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionResult>(ABSTRACTREPO, chartData));
        return;
    }

    calculationController->computePopPerc(predictionspec);
}

void ChartDataController::receiveResultsPopPerc(ezechiel::core::PredictionResult* pred) {
    chartData->getPopPred()->getPredictive()->setPercentileDataList(pred->getPredictive()->getPercentileDataList());
    chartData->getPopPred()->getPredictive()->getPercentileDataList()->setParent(chartData->getPopPred()->getPredictive());
}

void ChartDataController::initiateUpdateApoPred()
{
    if (!isAPosterioriValid()) {
        chartData->setApoPred(ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionResult>(ABSTRACTREPO, chartData));
        return;
    }
    calculationController->computeApoPred(predictionspec);
}

void ChartDataController::receiveResultsApoPred(ezechiel::core::PredictionResult* pred) {
    pred->setParent(chartData);
    chartData->setApoPred(pred);
}

void ChartDataController::initiateUpdateApoPerc() {
#ifdef DISABLEPERCS
    return;
#endif
    if (!isAPosterioriValid()) {
        chartData->setApoPred(ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionResult>(ABSTRACTREPO, chartData));
        return;
    }
    calculationController->computeApoPerc(predictionspec);
}

void ChartDataController::receiveResultsApoPerc(ezechiel::core::PredictionResult* pred) {
//    chartData->getApoPred()->getPredictive()->getPercentileDataList()->deleteLater();
    pred->getPredictive()->getPercentileDataList()->setParent(chartData->getApoPred()->getPredictive());
    chartData->getApoPred()->getPredictive()->setPercentileDataList(pred->getPredictive()->getPercentileDataList());
}

void ChartDataController::initiateUpdateAprPred()
{
    if (!isAPrioriValid()) {
        chartData->setAprPred(ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionResult>(ABSTRACTREPO, chartData));
        return;
    }
    calculationController->computeAprPred(predictionspec);
}

void ChartDataController::receiveResultsAprPred(ezechiel::core::PredictionResult* pred) {
    pred->setParent(chartData);
    chartData->setAprPred(pred);
}

void ChartDataController::initiateUpdateAprPerc() {
#ifdef DISABLEPERCS
    return;
#endif
    if (!isAPrioriValid()) {
        chartData->setAprPred(ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionResult>(ABSTRACTREPO, chartData));
        return;
    }
    calculationController->computeAprPerc(predictionspec);
}

void ChartDataController::receiveResultsAprPerc(ezechiel::core::PredictionResult* pred) {
    pred->getPredictive()->getPercentileDataList()->setParent(chartData->getAprPred()->getPredictive());
    chartData->getAprPred()->getPredictive()->setPercentileDataList(pred->getPredictive()->getPercentileDataList());
}

void ChartDataController::initiateUpdateRevPred()
{
    if (!isReverseValid()) {
        chartData->setRevPred(ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionResult>(ABSTRACTREPO, chartData));
        return;
    }
    calculationController->computeRevPred(predictionspec);
}

void ChartDataController::initiateUpdateAdjPred()
{
    if (!isAdjustedValid()) {
        chartData->setAdjPred(ezechiel::core::CoreFactory::createEntity<ezechiel::core::PredictionResult>(ABSTRACTREPO, chartData));
        return;
    }
    calculationController->computeAdjPred(predictionspec);
}

void ChartDataController::initiateUpdateAdjPerc()
{
    if (!isAdjustedValid()) {
        return;
    }
    calculationController->computeAdjPerc(predictionspec);
}

void ChartDataController::receiveResultsRevPred(ezechiel::core::PredictionResult* pred) {
    pred->setParent(chartData);
    chartData->setRevPred(pred);
}

#include "core/dal/predictiondata.h"

void ChartDataController::receiveResultsAdjPred(ezechiel::core::PredictionResult* pred) {
    if (!isAdjustedValid()) {
        return;
    }
    pred->setParent(chartData);
    chartData->setAdjPred(pred);

    ezechiel::core::PredictionData *data = pred->getPredictive()->getPredictionData();
    int statIndex = data->getNbCycles() - 2;
    if (statIndex >= 0) {
        ezechiel::core::CycleStatistics stat = data->getCycleStatistics(statIndex);
        chartData->setInfo("steadyStateAUC24", QString::number(stat.auc24));
        chartData->setInfo("steadyStateMin", QString::number(stat.trough));
        chartData->setInfo("steadyStateMax", QString::number(stat.peak));
    }
}

void ChartDataController::receiveResultsAdjPerc(ezechiel::core::PredictionResult* pred) {
    if (!isAdjustedValid()) {
        return;
    }
    pred->getPredictive()->getPercentileDataList()->setParent(chartData->getAdjPred()->getPredictive());
    chartData->getAdjPred()->getPredictive()->setPercentileDataList(pred->getPredictive()->getPercentileDataList());
}

void ChartDataController::popCalcFailed() {
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, tr("Calculation timedout for Population Percentiles"))
    togglePercentiles(ezechiel::core::POPULATION);
}

void ChartDataController::aprCalcFailed() {
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, tr("Calculation timedout for Apriori Percentiles"))
    togglePercentiles(ezechiel::core::APRIORI);
}

void ChartDataController::apoCalcFailed() {
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, tr("Calculation timedout for Aposteriori Percentiles"))
    togglePercentiles(ezechiel::core::APOSTERIORI);
}

void ChartDataController::adjCalcFailed() {
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, tr("Calculation timedout for adjustment Percentiles"))
    //togglePercentiles(ezechiel::core::APOSTERIORI);
}

void ChartDataController::togglePercentiles(int ptype) {

    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, tr("Toggling percentiles of type: %1").arg(ptype))
    //toggle the showing of parameters for failed calc or for chosen
}


bool ChartDataController::isPopulationValid() const
{
    if (predictionspec->getAnalysis()->getTreatment()->getDosages()->isEmpty())
        return false;
    if (!predictionspec->getAnalysis()->getDrugModel())
        return false;
    if (predictionspec->getNbPoints() <= 0)
        return false;
    if (!predictionspec->getStartDate().isValid())
        return false;
    if (!predictionspec->getEndDate().isValid())
        return false;

    return true;
}

bool ChartDataController::isAPrioriValid() const
{
    if (!isPopulationValid())
        return false;
    if (predictionspec->getAnalysis()->getDrugModel()->getCovariates()->isEmpty())
        return false;
    if (!chartData->getAprPred()->isValid()) {
        return true;
    }
    if (predictionspec->getAnalysis()->getTreatment()->getCovariates()->isEmpty())
        return false;

    return true;
}

bool ChartDataController::isAPosterioriValid() const
{
    if (!isPopulationValid())
        return false;
    if (predictionspec->getAnalysis()->getTreatment()->getMeasures()->isEmpty())
        return false;

    return true;
}

bool ChartDataController::isReverseValid() const
{
    if (!predictionspec->getAdjustmentDate().isValid()) {
       // EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Reverse prediction not run because adjustment time not valid.");
        return false;
    }

    return true;
}

bool ChartDataController::isAdjustedValid() const
{
    if (!isReverseValid()) {
        return false;
    }
    if (!predictionspec->getAdjustmentDate().isValid()) {
        EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Adjustment prediction not run because adjustment time not valid.");
        return false;
    }

    if (predictionspec->getAnalysis()->getTreatment()->getAdjustments()->size() == 0) {
        EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Adjustment prediction not run because there is no defined adjustment.");
       return false;
    }

    return true;
}


void ChartDataController::start()
{
    initiateUpdatePopPred();
    initiateUpdateAprPred();
    initiateUpdateApoPred();
    initiateUpdateRevPred();
    initiateUpdateAdjPred();

    initiateUpdatePopPerc();
    initiateUpdateAprPerc();
    initiateUpdateApoPerc();

    initiateUpdateAdjPerc();

}

void ChartDataController::adjustmentSettingsUpdated()
{
    initiateUpdateRevPred();
}

void ChartDataController::adjustmentUpdated()
{
    initiateUpdateAdjPred();
    initiateUpdateAdjPerc();
}


void ChartDataController::viewRangeUpdated(bool bShowPerc, bool computeRev)
{
    // Only for that particular case we keep the cache
    predictionspec->setClearCache(false);
    initiateUpdatePopPred();
    initiateUpdateAprPred();
    initiateUpdateApoPred();
//    initiateUpdateAdjPred();
    if (computeRev)
        initiateUpdateRevPred();
    if (bShowPerc) {
        initiateUpdatePopPerc();
        initiateUpdateAprPerc();
        initiateUpdateApoPerc();
//        initiateUpdateAdjPerc();
    }
    // Back to the clear cache mode
    predictionspec->setClearCache(true);
}

void ChartDataController::dosageUpdated(bool bShowPerc, bool computeRev)
{    
    initiateUpdatePopPred();
    initiateUpdateAprPred();
    initiateUpdateApoPred();
//    initiateUpdateAdjPred();
    if (computeRev)
        initiateUpdateRevPred();
    if (bShowPerc) {
        initiateUpdatePopPerc();
        initiateUpdateAprPerc();
        initiateUpdateApoPerc();
//        initiateUpdateAdjPerc();
    }
}

void ChartDataController::measureUpdated(bool bShowPerc)
{
    initiateUpdateApoPred();
    initiateUpdateRevPred();
    if (bShowPerc) {
        initiateUpdateApoPerc();
    }
}

void ChartDataController::targetUpdated()
{
    initiateUpdateRevPred();
}

void ChartDataController::covariateUpdated(bool bShowPerc)
{
    initiateUpdateAprPred();
    initiateUpdateApoPred();
    initiateUpdateRevPred();
    if (bShowPerc) {
        initiateUpdateAprPerc();
        initiateUpdateApoPerc();
    }
}

