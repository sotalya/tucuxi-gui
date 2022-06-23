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

//using namespace Tucuxi::Gui::Admin;
using namespace Tucuxi::Gui::GuiUtils;

ChartDataController::ChartDataController(QObject *parent) : QObject(parent),
    _currentTab(Tucuxi::Gui::Admin::StepType::Patient),
    calculationController(nullptr)
{
}

void ChartDataController::setCalculationController(CalculationController *controller)
{
    calculationController = controller;

    CONNECT(calculationController, SIGNAL(popPredComputed(Tucuxi::Gui::Core::PredictionResult*)), this, SLOT(receiveResultsPopPred(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(popPercComputed(Tucuxi::Gui::Core::PredictionResult*)), this, SLOT(receiveResultsPopPerc(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(aprPredComputed(Tucuxi::Gui::Core::PredictionResult*)), this, SLOT(receiveResultsAprPred(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(aprPercComputed(Tucuxi::Gui::Core::PredictionResult*)), this, SLOT(receiveResultsAprPerc(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(apoPredComputed(Tucuxi::Gui::Core::PredictionResult*)), this, SLOT(receiveResultsApoPred(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(apoPercComputed(Tucuxi::Gui::Core::PredictionResult*)), this, SLOT(receiveResultsApoPerc(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(revPredComputed(Tucuxi::Gui::Core::PredictionResult*)), this, SLOT(receiveResultsRevPred(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(adjPredComputed(Tucuxi::Gui::Core::PredictionResult*)), this, SLOT(receiveResultsAdjPred(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(adjPercComputed(Tucuxi::Gui::Core::PredictionResult*)), this, SLOT(receiveResultsAdjPerc(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(calculationController, SIGNAL(popCalcFail()), this, SLOT(popCalcFailed()));
    CONNECT(calculationController, SIGNAL(aprCalcFail()), this, SLOT(aprCalcFailed()));
    CONNECT(calculationController, SIGNAL(apoCalcFail()), this, SLOT(apoCalcFailed()));
    CONNECT(calculationController, SIGNAL(adjCalcFail()), this, SLOT(adjCalcFailed()));
}

void ChartDataController::setCurrentTab(Tucuxi::Gui::Admin::StepType::Enum step)
{
    _currentTab = step;
}

void ChartDataController::initiateUpdatePopPred()
{
    if (!isPopulationValid()) {
        chartData->setPopPred(Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PredictionResult>(ABSTRACTREPO, chartData));
        return;
    }
    calculationController->computePopPred(predictionspec);
}

void ChartDataController::receiveResultsPopPred(Tucuxi::Gui::Core::PredictionResult* pred) {
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
        chartData->setPopPred(Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PredictionResult>(ABSTRACTREPO, chartData));
        return;
    }

    calculationController->computePopPerc(predictionspec);
}

void ChartDataController::receiveResultsPopPerc(Tucuxi::Gui::Core::PredictionResult* pred) {
    chartData->getPopPred()->getPredictive()->setPercentileDataList(pred->getPredictive()->getPercentileDataList());
    chartData->getPopPred()->getPredictive()->getPercentileDataList()->setParent(chartData->getPopPred()->getPredictive());
}

void ChartDataController::initiateUpdateApoPred()
{
    if (!isAPosterioriValid()) {
        chartData->setApoPred(Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PredictionResult>(ABSTRACTREPO, chartData));
        return;
    }
    calculationController->computeApoPred(predictionspec);
}

void ChartDataController::receiveResultsApoPred(Tucuxi::Gui::Core::PredictionResult* pred) {
    pred->setParent(chartData);
    chartData->setApoPred(pred);
}

void ChartDataController::initiateUpdateApoPerc() {
#ifdef DISABLEPERCS
    return;
#endif
    if (!isAPosterioriValid()) {
        chartData->setApoPred(Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PredictionResult>(ABSTRACTREPO, chartData));
        return;
    }
    calculationController->computeApoPerc(predictionspec);
}

void ChartDataController::receiveResultsApoPerc(Tucuxi::Gui::Core::PredictionResult* pred) {
//    chartData->getApoPred()->getPredictive()->getPercentileDataList()->deleteLater();
    pred->getPredictive()->getPercentileDataList()->setParent(chartData->getApoPred()->getPredictive());
    chartData->getApoPred()->getPredictive()->setPercentileDataList(pred->getPredictive()->getPercentileDataList());
}

void ChartDataController::initiateUpdateAprPred()
{
    if (!isAPrioriValid()) {
        chartData->setAprPred(Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PredictionResult>(ABSTRACTREPO, chartData));
        return;
    }
    calculationController->computeAprPred(predictionspec);
}

void ChartDataController::receiveResultsAprPred(Tucuxi::Gui::Core::PredictionResult* pred) {
    pred->setParent(chartData);
    chartData->setAprPred(pred);
}

void ChartDataController::initiateUpdateAprPerc() {
#ifdef DISABLEPERCS
    return;
#endif
    if (!isAPrioriValid()) {
        chartData->setAprPred(Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PredictionResult>(ABSTRACTREPO, chartData));
        return;
    }
    calculationController->computeAprPerc(predictionspec);
}

void ChartDataController::receiveResultsAprPerc(Tucuxi::Gui::Core::PredictionResult* pred) {
    pred->getPredictive()->getPercentileDataList()->setParent(chartData->getAprPred()->getPredictive());
    chartData->getAprPred()->getPredictive()->setPercentileDataList(pred->getPredictive()->getPercentileDataList());
}

void ChartDataController::initiateUpdateRevPred()
{
    if (!isReverseValid()) {
        chartData->setRevPred(Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PredictionResult>(ABSTRACTREPO, chartData));
        return;
    }
    calculationController->computeRevPred(predictionspec);
}

void ChartDataController::initiateUpdateAdjPred()
{
    if (!isAdjustedValid()) {
        chartData->setAdjPred(Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PredictionResult>(ABSTRACTREPO, chartData));
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

void ChartDataController::receiveResultsRevPred(Tucuxi::Gui::Core::PredictionResult* pred) {
    pred->setParent(chartData);
    chartData->setRevPred(pred);
}

#include "core/dal/predictiondata.h"

void ChartDataController::receiveResultsAdjPred(Tucuxi::Gui::Core::PredictionResult* pred) {
    if (!isAdjustedValid()) {
        return;
    }
    pred->setParent(chartData);
    chartData->setAdjPred(pred);

    // Getting the steady state only works if we really reached steady state.
    // It means it is not very right for drugs for neonates where the age in days can have an influence
    Tucuxi::Gui::Core::PredictionData *data = pred->getPredictive()->getPredictionData();
    int statIndex = data->getNbCycles() - 2;
    if (statIndex >= 0) {
        Tucuxi::Gui::Core::CycleStatistics stat = data->getCycleStatistics(statIndex);
        chartData->setInfo("steadyStateAUC24", QString::number(stat.auc24));
        chartData->setInfo("steadyStateMin", QString::number(stat.trough));
        chartData->setInfo("steadyStateMax", QString::number(stat.peak));
    }
}

void ChartDataController::receiveResultsAdjPerc(Tucuxi::Gui::Core::PredictionResult* pred) {
    if (!isAdjustedValid()) {
        return;
    }
    pred->getPredictive()->getPercentileDataList()->setParent(chartData->getAdjPred()->getPredictive());
    chartData->getAdjPred()->getPredictive()->setPercentileDataList(pred->getPredictive()->getPercentileDataList());
}

void ChartDataController::popCalcFailed() {
    EXLOG(QtDebugMsg, Tucuxi::guiutils::NOEZERROR, tr("Calculation timedout for Population Percentiles"))
    togglePercentiles(Tucuxi::Gui::Core::POPULATION);
}

void ChartDataController::aprCalcFailed() {
    EXLOG(QtDebugMsg, Tucuxi::guiutils::NOEZERROR, tr("Calculation timedout for Apriori Percentiles"))
    togglePercentiles(Tucuxi::Gui::Core::APRIORI);
}

void ChartDataController::apoCalcFailed() {
    EXLOG(QtDebugMsg, Tucuxi::guiutils::NOEZERROR, tr("Calculation timedout for Aposteriori Percentiles"))
    togglePercentiles(Tucuxi::Gui::Core::APOSTERIORI);
}

void ChartDataController::adjCalcFailed() {
    EXLOG(QtDebugMsg, Tucuxi::guiutils::NOEZERROR, tr("Calculation timedout for adjustment Percentiles"))
    //togglePercentiles(Tucuxi::Gui::Core::APOSTERIORI);
}

void ChartDataController::togglePercentiles(int ptype) {

    EXLOG(QtDebugMsg, Tucuxi::guiutils::NOEZERROR, tr("Toggling percentiles of type: %1").arg(ptype))
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
       // EXLOG(QtDebugMsg, Tucuxi::guiutils::NOEZERROR, "Reverse prediction not run because adjustment time not valid.");
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
        EXLOG(QtDebugMsg, Tucuxi::guiutils::NOEZERROR, "Adjustment prediction not run because adjustment time not valid.");
        return false;
    }

    if (predictionspec->getAnalysis()->getTreatment()->getAdjustments()->size() == 0) {
        EXLOG(QtDebugMsg, Tucuxi::guiutils::NOEZERROR, "Adjustment prediction not run because there is no defined adjustment.");
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

