//@@license@@

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

    CONNECT(calculationController, SIGNAL(popPredComputed(ezechiel::GuiCore::PredictionResult*)), this, SLOT(receiveResultsPopPred(ezechiel::GuiCore::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(popPercComputed(ezechiel::GuiCore::PredictionResult*)), this, SLOT(receiveResultsPopPerc(ezechiel::GuiCore::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(aprPredComputed(ezechiel::GuiCore::PredictionResult*)), this, SLOT(receiveResultsAprPred(ezechiel::GuiCore::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(aprPercComputed(ezechiel::GuiCore::PredictionResult*)), this, SLOT(receiveResultsAprPerc(ezechiel::GuiCore::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(apoPredComputed(ezechiel::GuiCore::PredictionResult*)), this, SLOT(receiveResultsApoPred(ezechiel::GuiCore::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(apoPercComputed(ezechiel::GuiCore::PredictionResult*)), this, SLOT(receiveResultsApoPerc(ezechiel::GuiCore::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(revPredComputed(ezechiel::GuiCore::PredictionResult*)), this, SLOT(receiveResultsRevPred(ezechiel::GuiCore::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(adjPredComputed(ezechiel::GuiCore::PredictionResult*)), this, SLOT(receiveResultsAdjPred(ezechiel::GuiCore::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(adjPercComputed(ezechiel::GuiCore::PredictionResult*)), this, SLOT(receiveResultsAdjPerc(ezechiel::GuiCore::PredictionResult*)));
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
        chartData->setPopPred(ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::PredictionResult>(ABSTRACTREPO, chartData));
        return;
    }
    calculationController->computePopPred(predictionspec);
}

void ChartDataController::receiveResultsPopPred(ezechiel::GuiCore::PredictionResult* pred) {
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
        chartData->setPopPred(ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::PredictionResult>(ABSTRACTREPO, chartData));
        return;
    }

    calculationController->computePopPerc(predictionspec);
}

void ChartDataController::receiveResultsPopPerc(ezechiel::GuiCore::PredictionResult* pred) {
    chartData->getPopPred()->getPredictive()->setPercentileDataList(pred->getPredictive()->getPercentileDataList());
    chartData->getPopPred()->getPredictive()->getPercentileDataList()->setParent(chartData->getPopPred()->getPredictive());
}

void ChartDataController::initiateUpdateApoPred()
{
    if (!isAPosterioriValid()) {
        chartData->setApoPred(ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::PredictionResult>(ABSTRACTREPO, chartData));
        return;
    }
    calculationController->computeApoPred(predictionspec);
}

void ChartDataController::receiveResultsApoPred(ezechiel::GuiCore::PredictionResult* pred) {
    pred->setParent(chartData);
    chartData->setApoPred(pred);
}

void ChartDataController::initiateUpdateApoPerc() {
#ifdef DISABLEPERCS
    return;
#endif
    if (!isAPosterioriValid()) {
        chartData->setApoPred(ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::PredictionResult>(ABSTRACTREPO, chartData));
        return;
    }
    calculationController->computeApoPerc(predictionspec);
}

void ChartDataController::receiveResultsApoPerc(ezechiel::GuiCore::PredictionResult* pred) {
//    chartData->getApoPred()->getPredictive()->getPercentileDataList()->deleteLater();
    pred->getPredictive()->getPercentileDataList()->setParent(chartData->getApoPred()->getPredictive());
    chartData->getApoPred()->getPredictive()->setPercentileDataList(pred->getPredictive()->getPercentileDataList());
}

void ChartDataController::initiateUpdateAprPred()
{
    if (!isAPrioriValid()) {
        chartData->setAprPred(ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::PredictionResult>(ABSTRACTREPO, chartData));
        return;
    }
    calculationController->computeAprPred(predictionspec);
}

void ChartDataController::receiveResultsAprPred(ezechiel::GuiCore::PredictionResult* pred) {
    pred->setParent(chartData);
    chartData->setAprPred(pred);
}

void ChartDataController::initiateUpdateAprPerc() {
#ifdef DISABLEPERCS
    return;
#endif
    if (!isAPrioriValid()) {
        chartData->setAprPred(ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::PredictionResult>(ABSTRACTREPO, chartData));
        return;
    }
    calculationController->computeAprPerc(predictionspec);
}

void ChartDataController::receiveResultsAprPerc(ezechiel::GuiCore::PredictionResult* pred) {
    pred->getPredictive()->getPercentileDataList()->setParent(chartData->getAprPred()->getPredictive());
    chartData->getAprPred()->getPredictive()->setPercentileDataList(pred->getPredictive()->getPercentileDataList());
}

void ChartDataController::initiateUpdateRevPred()
{
    if (!isReverseValid()) {
        chartData->setRevPred(ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::PredictionResult>(ABSTRACTREPO, chartData));
        return;
    }
    calculationController->computeRevPred(predictionspec);
}

void ChartDataController::initiateUpdateAdjPred()
{
    if (!isAdjustedValid()) {
        chartData->setAdjPred(ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::PredictionResult>(ABSTRACTREPO, chartData));
        return;
    }
    // Here we add 2 months to get the steady state values in ChartDataController::receiveResultsAdjPred()
    QDateTime oldEndDate = predictionspec->getEndDate();
    predictionspec->setEndDate(oldEndDate.addMonths(2));
    calculationController->computeAdjPred(predictionspec);
    // And back to the initial end date
    predictionspec->setEndDate(oldEndDate);
}

void ChartDataController::initiateUpdateAdjPerc()
{
    if (!isAdjustedValid()) {
        return;
    }
    calculationController->computeAdjPerc(predictionspec);
}

void ChartDataController::receiveResultsRevPred(ezechiel::GuiCore::PredictionResult* pred) {
    pred->setParent(chartData);
    chartData->setRevPred(pred);
}

#include "core/dal/predictiondata.h"

void ChartDataController::receiveResultsAdjPred(ezechiel::GuiCore::PredictionResult* pred) {
    if (!isAdjustedValid()) {
        return;
    }
    pred->setParent(chartData);
    chartData->setAdjPred(pred);

    // Getting the steady state only works if we really reached steady state.
    // It means it is not very right for drugs for neonates where the age in days can have an influence
    ezechiel::GuiCore::PredictionData *data = pred->getPredictive()->getPredictionData();
    int statIndex = data->getNbCycles() - 2;
    if (statIndex >= 0) {
        ezechiel::GuiCore::CycleStatistics stat = data->getCycleStatistics(statIndex);
        chartData->setInfo("steadyStateAUC24", QString::number(stat.auc24));
        chartData->setInfo("steadyStateMin", QString::number(stat.trough));
        chartData->setInfo("steadyStateMax", QString::number(stat.peak));
    }
}

void ChartDataController::receiveResultsAdjPerc(ezechiel::GuiCore::PredictionResult* pred) {
    if (!isAdjustedValid()) {
        return;
    }
    pred->getPredictive()->getPercentileDataList()->setParent(chartData->getAdjPred()->getPredictive());
    chartData->getAdjPred()->getPredictive()->setPercentileDataList(pred->getPredictive()->getPercentileDataList());
}

void ChartDataController::popCalcFailed() {
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, tr("Calculation timedout for Population Percentiles"))
    togglePercentiles(ezechiel::GuiCore::POPULATION);
}

void ChartDataController::aprCalcFailed() {
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, tr("Calculation timedout for Apriori Percentiles"))
    togglePercentiles(ezechiel::GuiCore::APRIORI);
}

void ChartDataController::apoCalcFailed() {
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, tr("Calculation timedout for Aposteriori Percentiles"))
    togglePercentiles(ezechiel::GuiCore::APOSTERIORI);
}

void ChartDataController::adjCalcFailed() {
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, tr("Calculation timedout for adjustment Percentiles"))
    //togglePercentiles(ezechiel::GuiCore::APOSTERIORI);
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

    if (!predictionspec->getStartDate().isValid()) {
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
    initiateUpdateAdjPred();
    if (computeRev)
        initiateUpdateRevPred();
    if (bShowPerc) {
        initiateUpdatePopPerc();
        initiateUpdateAprPerc();
        initiateUpdateApoPerc();
        initiateUpdateAdjPerc();
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

