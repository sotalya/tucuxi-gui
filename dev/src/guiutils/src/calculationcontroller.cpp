/* 
 * Tucuxi - Tucuxi-gui software. 
 * This software is able to perform prediction of drug concentration in blood 
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


#include <QVariant>
#include <QDateTime>


#include "calculationcontroller.h"

#include "core/core.h"
#include "core/corefactory.h"
#include "core/corerepository.h"
#include "core/interfaces/processinginterface.h"
#include "core/interfaces/processingtraits.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/coremeasure.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/prediction.h"
#include "core/dal/predictionresult.h"
#include "core/dal/predictionspec.h"
#include "core/dal/dosage.h"
#include "core/dal/drug/drug.h"
#include "core/utils/logging.h"
#include "core/utils/connect.h"
#include "errors_guiutils.h"
#include "apputils/src/calculationrequestlogger.h"
#include "guiutils/src/predictionspeccopier.h"
#include "core/dal/drug/standardtreatment.h"


constexpr int APRTIMEOUT = 20000;
constexpr int APOTIMEOUT = 35000;
constexpr int POPTIMEOUT = 20000;
constexpr int ADJTIMEOUT = 35000;

using namespace Tucuxi;
using namespace Gui::Core;
using namespace Tucuxi::Gui::GuiUtils;

CalculationController::CalculationController(QObject *parent)
    : QObject(parent)
{

}


LocalCalculationController::LocalCalculationController(QObject *parent)
    : CalculationController(parent),
      popPercAborter(nullptr),
      aprPercAborter(nullptr),
      apoPercAborter(nullptr),
      adjPercAborter(nullptr)
{
}

void LocalCalculationController::setAborters(PopPercAborter *pop,
                                             AprPercAborter *apr,
                                             ApoPercAborter *apo,
                                             AdjPercAborter *adj)
{
    popPercAborter = pop;
    aprPercAborter = apr;
    apoPercAborter = apo;
    adjPercAborter = adj;
}

void LocalCalculationController::computePopPred(PredictionSpec* prediction)
{

    emit disengage();
    EXLOG(QtDebugMsg, Tucuxi::Gui::GuiUtils::NOEZERROR, "Running typical patient prediction.");

    DataSet dataSet;
    PopulationTraits parametersTraits;
    PointsTraits pointsTraits(prediction->getStartDate(),
                              prediction->getEndDate(),
                              prediction->getNbPoints(),
                              &parametersTraits);

    auto* pred = CoreFactory::createEntity<PredictionResult>(ABSTRACTREPO);

    CORE->getProcessingInterface()->points(prediction->getAnalysis(),
                                           pointsTraits,
                                           *pred);

    EXLOG(QtDebugMsg, Tucuxi::Gui::GuiUtils::NOEZERROR, "Ending typical patient prediction.");
    pred->moveToThread(qApp->thread());
    emit popPredComputed(pred);
    emit engage();
}

void LocalCalculationController::preparePredResults(QVector<int> time, QVector<double> data, PredictionResult* pred) {
    auto* predictive = CoreFactory::createEntity<Predictive>(ABSTRACTREPO, pred);
    pred->setPredictive(predictive);
    auto* pdata = CoreFactory::createEntity<PredictionData>(ABSTRACTREPO, predictive);
    predictive->setPredictionData(pdata);
    auto* fpts = CoreFactory::createEntity<FancyPoints>(ABSTRACTREPO, pdata);
    pdata->setPoints(fpts);
    for (int i = 0; i < time.size(); ++i) {
        auto* fpt = CoreFactory::createEntity<FancyPoint>(ABSTRACTREPO, fpts);
        fpt->setTime(time[i]);
        fpt->setValue(data[i]);
        fpts->append(fpt);
    }
    EXLOG(QtDebugMsg, Tucuxi::Gui::GuiUtils::NOEZERROR, "Ending typical patient prediction.");
    pred->moveToThread(qApp->thread());
}

void LocalCalculationController::computePopPerc(PredictionSpec* prediction)
{
    emit disengage();
    EXLOG(QtDebugMsg, Tucuxi::Gui::GuiUtils::NOEZERROR, "Running typical patient percentiles.");

    PopulationTraits parametersTraits;
    PercentilesTraits percentilesTraits(prediction->getNbPoints(),
                                        prediction->getPercentileList(),
                                        prediction->getStartDate(),
                                        prediction->getEndDate(),
                                        &parametersTraits);

    percentilesTraits.clearCache = prediction->getClearCache();
    percentilesTraits.cacheId = 0;

    percentilesTraits.options[QString("percentile_method")] = QVariant::fromValue(2);

    auto* pred = CoreFactory::createEntity<PredictionResult>(ABSTRACTREPO);

    ProcessingResult result = CORE->getProcessingInterface()->percentiles(prediction->getAnalysis(),
                                                percentilesTraits,
                                                *pred,
                                                popPercAborter);

    if (result == ProcessingResult::Aborted) {
        emit popCalcFail();
        emit engage();
        return;
    }
    EXLOG(QtDebugMsg, Tucuxi::Gui::GuiUtils::NOEZERROR, "Ending typical patient percentiles.");
    pred->moveToThread(qApp->thread());
    emit popPercComputed(pred);
    emit engage();
}

void LocalCalculationController::computeAprPred(PredictionSpec* prediction)
{
    emit disengage();
    EXLOG(QtDebugMsg, Tucuxi::Gui::GuiUtils::NOEZERROR, "Running a priori prediction.");

    AprioriTraits parametersTraits;
    PointsTraits pointsTraits(prediction->getStartDate(),
                              prediction->getEndDate(),
                              prediction->getNbPoints(),
                              &parametersTraits);

    auto* pred = CoreFactory::createEntity<PredictionResult>(ABSTRACTREPO);

    CORE->getProcessingInterface()->points(prediction->getAnalysis(),
                                           pointsTraits,
                                           *pred);

    EXLOG(QtDebugMsg, Tucuxi::Gui::GuiUtils::NOEZERROR, "Ending a priori prediction.");

    pred->moveToThread(qApp->thread());
    emit aprPredComputed(pred);
    emit engage();

}

void LocalCalculationController::computeAprPerc(PredictionSpec* prediction)
{
    emit disengage();
    EXLOG(QtDebugMsg, Tucuxi::Gui::GuiUtils::NOEZERROR, "Running apriori percentiles.");


    AprioriTraits parametersTraits;
    PercentilesTraits percentilesTraits(prediction->getNbPoints(),
                                        prediction->getPercentileList(),
                                        prediction->getStartDate(),
                                        prediction->getEndDate(),
                                        &parametersTraits);

    percentilesTraits.clearCache = prediction->getClearCache();
    percentilesTraits.cacheId = 1;

    percentilesTraits.options[QString("percentile_method")] = QVariant::fromValue(2);

    auto* pred = CoreFactory::createEntity<PredictionResult>(ABSTRACTREPO);

    ProcessingResult result = CORE->getProcessingInterface()->percentiles(prediction->getAnalysis(),
                                                percentilesTraits,
                                                *pred,
                                                aprPercAborter);

    EXLOG(QtDebugMsg, Tucuxi::Gui::GuiUtils::NOEZERROR, "Ending apriori percentiles.");

    if (result == ProcessingResult::Aborted) {
        emit aprCalcFail();
        emit engage();
        return;
    }

    pred->moveToThread(qApp->thread());
    emit aprPercComputed(pred);
    emit engage();
}

void LocalCalculationController::computeApoPred(PredictionSpec* prediction)
{
    emit disengage();
    EXLOG(QtDebugMsg, Tucuxi::Gui::GuiUtils::NOEZERROR, "Running a posteriori prediction.");

    ParamTraits* parametersTraits = new AposterioriTraits();
    PointsTraits pointsTraits(prediction->getStartDate(),
                              prediction->getEndDate(),
                              prediction->getNbPoints(),
                              parametersTraits);

    auto* pred = CoreFactory::createEntity<PredictionResult>(ABSTRACTREPO);

    CORE->getProcessingInterface()->points(prediction->getAnalysis(),
                                           pointsTraits,
                                           *pred);

    EXLOG(QtDebugMsg, Tucuxi::Gui::GuiUtils::NOEZERROR, "Ending a posteriori prediction.");

    pred->moveToThread(qApp->thread());
    emit apoPredComputed(pred);
    emit engage();

    delete(parametersTraits);
}

void LocalCalculationController::computeApoPerc(PredictionSpec* prediction)
{
    emit disengage();
    EXLOG(QtDebugMsg, Tucuxi::Gui::GuiUtils::NOEZERROR, "Running aposteriori percentiles.");

    AposterioriTraits parametersTraits;
    PercentilesTraits percentilesTraits(prediction->getNbPoints(),
                                        prediction->getPercentileList(),
                                        prediction->getStartDate(),
                                        prediction->getEndDate(),
                                        &parametersTraits);

    percentilesTraits.clearCache = prediction->getClearCache();
    percentilesTraits.cacheId = 2;

    auto* pred = CoreFactory::createEntity<PredictionResult>(ABSTRACTREPO);

    ProcessingResult result = CORE->getProcessingInterface()->calculateAposterioriPercentiles(prediction->getAnalysis(),
                                                percentilesTraits,
                                                *pred,
                                                apoPercAborter);

    EXLOG(QtDebugMsg, Tucuxi::Gui::GuiUtils::NOEZERROR, "Ending aposteriori percentiles.");

    if (result == ProcessingResult::Aborted) {
        emit apoCalcFail();
        emit engage();
        return;
    }

    pred->moveToThread(qApp->thread());
    emit apoPercComputed(pred);
    emit engage();
}

void LocalCalculationController::computeAdjPerc(PredictionSpec* prediction)
{
    emit disengage();
    EXLOG(QtDebugMsg, Tucuxi::Gui::GuiUtils::NOEZERROR, "Running adjustments percentiles.");


    ParamTraits* parametersTraits = nullptr;
    if (prediction->getAnalysis()->getTreatment()->getMeasures()->isEmpty()) {
        parametersTraits = new AprioriTraits(prediction->getAdjustmentDate());
    } else {
        parametersTraits = new AposterioriTraits(prediction->getAdjustmentDate());
    }


    //QDateTime start = prediction->getStartDate();
    QDateTime start = prediction->getAdjustmentDate();
    QDateTime end = prediction->getEndDate();
/*
 * That could happen, seems bad, but is handled by Tucucore
    if (end < start) {
        emit adjCalcFail();
        emit engage();
        return;
    }
*/
/*
    ParamTraits* parametersTraits;
    if (prediction->getAnalysis()->getTreatment()->getMeasures()->isEmpty()) {
        parametersTraits = new AprioriTraits(prediction->getAdjustmentDate());
    } else {
        parametersTraits = new AposterioriTraits(prediction->getAdjustmentDate());
    }


    QDateTime start = prediction->getStartDate();
    QDateTime end = prediction->getStartDate().addDays(4);
    //QDateTime end = prediction->getEndDate().addDays(4);
*/
    /*
    if (prediction->getAnalysis()->getDrugModel()->getStandardTreatment()->getIsFixedDuration()) {

        // We start from the beginning of the treatment
        if (!prediction->getAnalysis()->getTreatment()->getDosages()->isEmpty()) {
            start = prediction->getAnalysis()->getTreatment()->getDosages()->firsttake();
        }

        Duration duration = prediction->getAnalysis()->getDrugModel()->getStandardTreatment()->getDuration();
        double seconds = duration.toSeconds();
        end = start.addSecs(duration.toSeconds());
        end = end.addSecs(Duration(180).toSeconds());
    }
    */

    PercentilesTraits percentilesTraits(prediction->getNbPoints(),
                                        prediction->getPercentileList(),
                                        start,
                                        end,
                                        parametersTraits);

    percentilesTraits.cacheId = 3;

    auto* pred = CoreFactory::createEntity<PredictionResult>(ABSTRACTREPO);

    ProcessingResult result = ProcessingResult::Failure;
    if (prediction->getAnalysis()->getTreatment()->getMeasures()->isEmpty()) {
        result = CORE->getProcessingInterface()->percentiles(prediction->getAnalysis(),
                                                    percentilesTraits,
                                                    *pred,
                                                    adjPercAborter);
    } else {
        result = CORE->getProcessingInterface()->calculateAposterioriPercentiles(prediction->getAnalysis(),
                                                    percentilesTraits,
                                                    *pred,
                                                    adjPercAborter);
    }


    EXLOG(QtDebugMsg, Tucuxi::Gui::GuiUtils::NOEZERROR, "Ending adjustments percentiles.");

    if (result == ProcessingResult::Aborted) {
        emit adjCalcFail();
        emit engage();
        return;
    }

    pred->moveToThread(qApp->thread());
    emit adjPercComputed(pred);
    emit engage();
}

void LocalCalculationController::computeRevPred(PredictionSpec* prediction)
{
    emit disengage();
    EXLOG(QtDebugMsg, Tucuxi::Gui::GuiUtils::NOEZERROR, "Running reverse prediction.");
    //Q_ASSERT_X(dateSetting.isValid());

    ParamTraits* parametersTraits = nullptr;
    if (prediction->getAnalysis()->getTreatment()->getMeasures()->isEmpty()) {
        parametersTraits = new AprioriTraits();
    } else {
        parametersTraits = new AposterioriTraits();
    }

    // Here we calculate the end of treatment.
    // If the drugmodel does not have a fixed duration treatment, we simply take the end date of the prediction.
    // If the drugmodel does have a fixed duration treatment, then it depends if there is already
    // a dosage or not. In the first case, we use the time of first dose to define the end of treatment.
    // In the second case, we use the time of adjustment as the time of first dose.
    QDateTime endOfTreatment;
    QDateTime endDate = prediction->getEndDate();
    if (prediction->getAnalysis()->getDrugModel()->getStandardTreatment()->getIsFixedDuration()) {
        QDateTime start;
        if (prediction->getAnalysis()->getTreatment()->getDosages()->isEmpty()) {
            start = prediction->getAdjustmentDate();
        }
        else {
            start = prediction->getAnalysis()->getTreatment()->getDosages()->firsttake();
        }
        Duration duration = prediction->getAnalysis()->getDrugModel()->getStandardTreatment()->getDuration();
        double seconds = duration.toSeconds();
        endOfTreatment = start.addSecs(seconds);
    }
    else {
        endOfTreatment = endDate;
    }

    // YJ: Temporary fix. We should make sure the computation period is long enough and
    // based on the time needed to reach stability at a higher level.
    if (prediction->getAdjustmentDate().daysTo(endDate) < 5) {
        endDate = prediction->getAdjustmentDate().addDays(5);
    }

    if (prediction->getAnalysis()->getDrugModel()->getStandardTreatment()->getIsFixedDuration()) {
        // For a fixed duration treatment, it is likely that we need to calculated the entire duration
        if (endOfTreatment.daysTo(endDate) < 5) {
            endDate = endOfTreatment.addDays(5);
        }

        // Temporary Fix to the end of treatment, for busulfan cumulative AUC
        endDate = endOfTreatment;
    }

    ReverseTraits reverseTraits(prediction->getAdjustmentWithLoadingDose(),
                                prediction->getAdjustmentWithRestPeriod(),
                                prediction->getStartDate(),
                                endDate,
                                prediction->getAdjustmentDate(),
                                endOfTreatment,
                                prediction->getNbPoints(),
                                parametersTraits);

    auto* pred = CoreFactory::createEntity<PredictionResult>(ABSTRACTREPO);
    CORE->getProcessingInterface()->computeSuggestedAdjustments(prediction->getAnalysis(),
                                            reverseTraits,
                                            *pred);

    EXLOG(QtDebugMsg, Tucuxi::Gui::GuiUtils::NOEZERROR, "Ending reverse prediction.");

    pred->moveToThread(qApp->thread());
    emit revPredComputed(pred);

//    emit adjustmentsUpdated(adjustments);
    emit engage();
    delete(parametersTraits);
}

// #include "apputils/src/predictionspecexporter.h"

void LocalCalculationController::computeAdjPred(PredictionSpec* prediction)
{
    /*
    PredictionSpecExporter exporter;
    QByteArray array;
    exporter.save(prediction, array);
    QFile f("/home/ythoma/bidon.txt");
    f.open(QFile::WriteOnly);
    f.write(array.data());
    f.close();
    */

    emit disengage();
    EXLOG(QtDebugMsg, Tucuxi::Gui::GuiUtils::NOEZERROR, "Running a priori adjustment prediction.");

    ParamTraits* parametersTraits = nullptr;
    if (prediction->getAnalysis()->getTreatment()->getMeasures()->isEmpty()) {
        parametersTraits = new AprioriTraits(prediction->getAdjustmentDate());
    } else {
        parametersTraits = new AposterioriTraits(prediction->getAdjustmentDate());
    }


    QDateTime start = prediction->getStartDate();
    QDateTime end = prediction->getEndDate();

    /*
    if (prediction->getAnalysis()->getDrugModel()->getStandardTreatment()->getIsFixedDuration()) {

        // We start from the beginning of the treatment
        if (!prediction->getAnalysis()->getTreatment()->getDosages()->isEmpty()) {
            start = prediction->getAnalysis()->getTreatment()->getDosages()->firsttake();
        }

        Duration duration = prediction->getAnalysis()->getDrugModel()->getStandardTreatment()->getDuration();
        double seconds = duration.toSeconds();
        end = start.addSecs(duration.toSeconds());
        end = end.addSecs(Duration(180).toSeconds());
    }
    */

    PointsTraits pointsTraits(start,
                              end,
                              prediction->getNbPoints(),
                              parametersTraits);

    auto* pred = CoreFactory::createEntity<PredictionResult>(ABSTRACTREPO);

    CORE->getProcessingInterface()->points(prediction->getAnalysis(),
                                           pointsTraits,
                                           *pred);

    EXLOG(QtDebugMsg, Tucuxi::Gui::GuiUtils::NOEZERROR, "Ending a priori adjustment prediction.");
    pred->moveToThread(qApp->thread());
    emit adjPredComputed(pred);
    emit engage();
}

void LocalCalculationController::abortAll()
{
}

CalculationThread::CalculationThread(QObject *parent) : QThread(parent),
    controller(nullptr), buffer(nullptr)
{

}

void CalculationThread::setCalculationController(CalculationController *controller)
{
    this->controller = controller;
}

void CalculationThread::setCalculationBuffer(CalculationBuffer *buffer)
{
    this->buffer = buffer;
}

void CalculationThread::run()
{
    Q_ASSERT(buffer!= nullptr);
    Q_ASSERT(controller != nullptr);

    CalculationRequestLogger *logger = CalculationRequestLogger::getInstance();
    
    setPriority(QThread::LowPriority); // So the GUI can be more responsive

    while (true) {
        Tucuxi::Gui::Core::PredictionSpec *spec = nullptr;

        CalculationBuffer::calculationType what = CalculationBuffer::calculationType::Quit;
        buffer->get(&spec, &what);

        logger->log(spec);

        switch(what) {
        case CalculationBuffer::PopPred : controller->computePopPred(spec); break;
        case CalculationBuffer::PopPerc : controller->computePopPerc(spec); break;
        case CalculationBuffer::AprPred : controller->computeAprPred(spec); break;
        case CalculationBuffer::AprPerc : controller->computeAprPerc(spec); break;
        case CalculationBuffer::ApoPred : controller->computeApoPred(spec); break;
        case CalculationBuffer::ApoPerc : controller->computeApoPerc(spec); break;
        case CalculationBuffer::RevPred : controller->computeRevPred(spec); break;
        case CalculationBuffer::AdjPred : controller->computeAdjPred(spec); break;
        case CalculationBuffer::AdjPerc : controller->computeAdjPerc(spec); break;
        case CalculationBuffer::Quit    : return; // delete logger; return;
        default: break;
        }
        delete spec;
    }

}

ThreadedCalculationController::ThreadedCalculationController(QObject *parent) : CalculationController(parent)
{
    // No parent, else moving to a thread don't work
    controller = new LocalCalculationController();
    buffer = new CalculationBuffer(this);
    workerThread = new CalculationThread(this);
    workerThread->setObjectName("TucuxiWorkerThread");
    workerThread->setCalculationBuffer(buffer);
    workerThread->setCalculationController(controller);
    controller->moveToThread(workerThread);



    controller->setAborters(new PopPercAborter(buffer), new AprPercAborter(buffer), new ApoPercAborter(buffer), new AdjPercAborter(buffer));


    CONNECT(controller, SIGNAL(disengage()), this, SIGNAL(disengage()));
    CONNECT(controller, SIGNAL(engage()), this, SIGNAL(engage()));
    CONNECT(controller, SIGNAL(popPredComputed(Tucuxi::Gui::Core::PredictionResult*)), buffer, SLOT(popPredComputed(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(controller, SIGNAL(aprPredComputed(Tucuxi::Gui::Core::PredictionResult*)), buffer, SLOT(aprPredComputed(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(controller, SIGNAL(apoPredComputed(Tucuxi::Gui::Core::PredictionResult*)), buffer, SLOT(apoPredComputed(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(controller, SIGNAL(revPredComputed(Tucuxi::Gui::Core::PredictionResult*)), buffer, SLOT(revPredComputed(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(controller, SIGNAL(popPercComputed(Tucuxi::Gui::Core::PredictionResult*)), buffer, SLOT(popPercComputed(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(controller, SIGNAL(aprPercComputed(Tucuxi::Gui::Core::PredictionResult*)), buffer, SLOT(aprPercComputed(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(controller, SIGNAL(apoPercComputed(Tucuxi::Gui::Core::PredictionResult*)), buffer, SLOT(apoPercComputed(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(controller, SIGNAL(adjPercComputed(Tucuxi::Gui::Core::PredictionResult*)), buffer, SLOT(adjPercComputed(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(controller, SIGNAL(adjustmentsComputed(QList<Tucuxi::Gui::Core::Dosage*>)), buffer, SLOT(adjustmentsComputed(QList<Tucuxi::Gui::Core::Dosage*>)));
    CONNECT(controller, SIGNAL(adjPredComputed(Tucuxi::Gui::Core::PredictionResult*)), buffer, SLOT(adjPredComputed(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(controller, SIGNAL(popCalcFail()), this, SIGNAL(popCalcFail()));
    CONNECT(controller, SIGNAL(aprCalcFail()), this, SIGNAL(aprCalcFail()));
    CONNECT(controller, SIGNAL(apoCalcFail()), this, SIGNAL(apoCalcFail()));
    CONNECT(controller, SIGNAL(adjCalcFail()), this, SIGNAL(adjCalcFail()));

    CONNECT(buffer, SIGNAL(sig_popPredUpdated(Tucuxi::Gui::Core::PredictionResult*)), this, SIGNAL(popPredComputed(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(buffer, SIGNAL(sig_aprPredUpdated(Tucuxi::Gui::Core::PredictionResult*)), this, SIGNAL(aprPredComputed(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(buffer, SIGNAL(sig_apoPredUpdated(Tucuxi::Gui::Core::PredictionResult*)), this, SIGNAL(apoPredComputed(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(buffer, SIGNAL(sig_revPredUpdated(Tucuxi::Gui::Core::PredictionResult*)), this, SIGNAL(revPredComputed(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(buffer, SIGNAL(sig_popPercUpdated(Tucuxi::Gui::Core::PredictionResult*)), this, SIGNAL(popPercComputed(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(buffer, SIGNAL(sig_aprPercUpdated(Tucuxi::Gui::Core::PredictionResult*)), this, SIGNAL(aprPercComputed(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(buffer, SIGNAL(sig_apoPercUpdated(Tucuxi::Gui::Core::PredictionResult*)), this, SIGNAL(apoPercComputed(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(buffer, SIGNAL(sig_adjPercUpdated(Tucuxi::Gui::Core::PredictionResult*)), this, SIGNAL(adjPercComputed(Tucuxi::Gui::Core::PredictionResult*)));
    CONNECT(buffer, SIGNAL(sig_adjustmentsUpdated(QList<Tucuxi::Gui::Core::Dosage*>)), this, SIGNAL(adjustmentsComputed(QList<Tucuxi::Gui::Core::Dosage*>)));
    CONNECT(buffer, SIGNAL(sig_adjPredUpdated(Tucuxi::Gui::Core::PredictionResult*)), this, SIGNAL(adjPredComputed(Tucuxi::Gui::Core::PredictionResult*)));

//#ifndef COMPILE_WITH_TUCUCORE
#if 0
    connect(&_popperctimer, &QTimer::timeout, this, [this](){
        buffer->abortIfRunningPopPerc();
    });

    connect(&_aprperctimer, &QTimer::timeout, this, [this](){
        buffer->abortIfRunningAprPerc();
        });

    connect(&_apoperctimer, &QTimer::timeout, this, [this](){
        buffer->abortIfRunningApoPerc();
        });
#endif // 0
    //#endif // COMPILE_WITH_TUCUCORE

    workerThread->start();
}

ThreadedCalculationController::~ThreadedCalculationController()
{
    abortAll();
    terminateWorker();
    // Wait for the thread to terminate
    workerThread->wait();
}

void ThreadedCalculationController::computePopPred(Tucuxi::Gui::Core::PredictionSpec* prediction)
{
    buffer->pushPopPred(prediction);
}

void ThreadedCalculationController::computePopPerc(Tucuxi::Gui::Core::PredictionSpec* prediction)
{
    buffer->pushPopPerc(prediction);
    _popperctimer.start(POPTIMEOUT);
}

void ThreadedCalculationController::computeAprPred(Tucuxi::Gui::Core::PredictionSpec* prediction)
{
    buffer->pushAprPred(prediction);
}

void ThreadedCalculationController::computeAprPerc(Tucuxi::Gui::Core::PredictionSpec* prediction)
{
    buffer->pushAprPerc(prediction);
    _aprperctimer.start(APRTIMEOUT);
}

void ThreadedCalculationController::computeApoPred(Tucuxi::Gui::Core::PredictionSpec* prediction)
{
    buffer->pushApoPred(prediction);
}

void ThreadedCalculationController::computeApoPerc(Tucuxi::Gui::Core::PredictionSpec* prediction)
{
    buffer->pushApoPerc(prediction);
    _apoperctimer.start(APOTIMEOUT);
}

void ThreadedCalculationController::computeAdjPerc(Tucuxi::Gui::Core::PredictionSpec* prediction)
{
    buffer->pushAdjPerc(prediction);
    _adjperctimer.start(ADJTIMEOUT);
}

void ThreadedCalculationController::computeRevPred(Tucuxi::Gui::Core::PredictionSpec* prediction)
{
    buffer->pushRevPred(prediction);
}

void ThreadedCalculationController::computeAdjPred(PredictionSpec* prediction)
{
    buffer->pushAdjPred(prediction);
}

void ThreadedCalculationController::abortAll()
{
    buffer->abortAll();
}

void ThreadedCalculationController::terminateWorker()
{
    buffer->quit();
}

















#define MESS(m)
/*
#define MESS(m) \
{ \
    std::cout << "-----------------------------------------------" << std::endl \
    << "                            " << m << std::endl; \
    std::cout << runningApoPerc << " ; " << abortApoPerc << std::endl; \
    std::cout << "-----------------------------------------------" ; \
    }
*/



CalculationBuffer::CalculationBuffer(QObject *parent) : QObject(parent),
    abortPopPerc(false),
    abortAprPerc(false),
    abortApoPerc(false),
    abortAdjPerc(false),
    runningPopPerc(false),
    runningAprPerc(false),
    runningApoPerc(false),
    runningAdjPerc(false)
{

}


void CalculationBuffer::abortIfRunningPopPerc()
{
    QMutexLocker locker(&mutex);
    if (runningPopPerc)
        abortPopPerc = true;
}

void CalculationBuffer::abortIfRunningAprPerc()
{
    QMutexLocker locker(&mutex);
    if (runningAprPerc)
        abortAprPerc = true;
}

void CalculationBuffer::abortIfRunningApoPerc()
{
    QMutexLocker locker(&mutex);
    if (runningApoPerc)
        abortApoPerc = true;
}

void CalculationBuffer::abortIfRunningAdjPerc()
{
    QMutexLocker locker(&mutex);
    if (runningAdjPerc)
        abortAdjPerc = true;
}

void CalculationBuffer::abortAll()
{
    QMutexLocker locker(&mutex);
    list.clear();
    locker.unlock();
    abortIfRunningApoPerc();
    abortIfRunningAprPerc();
    abortIfRunningPopPerc();
    abortIfRunningAdjPerc();
}

void CalculationBuffer::quit()
{
    QMutexLocker locker(&mutex);
    list.append({nullptr, Quit});
    dataReady.wakeOne();
}

void CalculationBuffer::pushPopPred(Tucuxi::Gui::Core::PredictionSpec* predSpec)
{
    MESS("Push Pop Pred");
    predSpec->setCalculationType(Tucuxi::Gui::Core::PredictionSpec::CalculationType::PopPred);
    push(predSpec, PopPred);
}

void CalculationBuffer::pushPopPerc(Tucuxi::Gui::Core::PredictionSpec* predSpec)
{
    MESS("Push Pop Perc");
    predSpec->setCalculationType(Tucuxi::Gui::Core::PredictionSpec::CalculationType::PopPerc);
    push(predSpec, PopPerc);
}

void CalculationBuffer::pushAprPred(Tucuxi::Gui::Core::PredictionSpec* predSpec)
{
    MESS("Push Apr Pred");
    predSpec->setCalculationType(Tucuxi::Gui::Core::PredictionSpec::CalculationType::AprPred);
    push(predSpec, AprPred);
}

void CalculationBuffer::pushAprPerc(Tucuxi::Gui::Core::PredictionSpec* predSpec)
{
    MESS("Push Apr Perc");
    predSpec->setCalculationType(Tucuxi::Gui::Core::PredictionSpec::CalculationType::AprPerc);
    push(predSpec, AprPerc);
}

void CalculationBuffer::pushApoPred(Tucuxi::Gui::Core::PredictionSpec* predSpec)
{
    MESS("Push Apo Pred");
    predSpec->setCalculationType(Tucuxi::Gui::Core::PredictionSpec::CalculationType::ApoPred);
    push(predSpec, ApoPred);
}

void CalculationBuffer::pushApoPerc(Tucuxi::Gui::Core::PredictionSpec* predSpec)
{
    MESS("Push Apo Perc");
    predSpec->setCalculationType(Tucuxi::Gui::Core::PredictionSpec::CalculationType::ApoPerc);
    push(predSpec, ApoPerc);
}

void CalculationBuffer::pushAdjPerc(Tucuxi::Gui::Core::PredictionSpec* predSpec)
{
    MESS("Push Adj Perc");
    predSpec->setCalculationType(Tucuxi::Gui::Core::PredictionSpec::CalculationType::AdjPerc);
    push(predSpec, AdjPerc);
}

void CalculationBuffer::pushRevPred(Tucuxi::Gui::Core::PredictionSpec* predSpec)
{
    MESS("Push Adjustment");
    predSpec->setCalculationType(Tucuxi::Gui::Core::PredictionSpec::CalculationType::RevPred);
    push(predSpec, RevPred);
}

void CalculationBuffer::pushAdjPred(Tucuxi::Gui::Core::PredictionSpec* predSpec)
{
    MESS("Push Adjustment");
    predSpec->setCalculationType(Tucuxi::Gui::Core::PredictionSpec::CalculationType::AdjPred);
    push(predSpec, AdjPred);
}

#include <QMutexLocker>

void CalculationBuffer::removeFromList(calculationType type)
{
    bool removed = true;
    while (removed) {
        removed = false;
        for(int i=0 ; i < list.size() ; i++)
            if (list.at(i).type == type) {
                list.removeAt(i);
                removed = true;
                break;
            }
    }
}

void CalculationBuffer::push(PredictionSpec *spec, calculationType what)
{
    // We copy the entire object to be sure there is no race condition between the GUI thread
    // and the worker thread. Destruction of the specCopy should be done by the worker thread when
    // computation is finished
    PredictionSpec *specCopy = PredictionSpecCopier().copy(spec);

    QMutexLocker locker(&mutex);

    switch (what) {
    case RevPred :  {
        removeFromList(RevPred);

    } break;
    case AprPred : {
        removeFromList(AprPerc);
        if (runningAprPerc) {
            abortAprPerc = true;
        }
    } break;
    case ApoPred : {
        removeFromList(ApoPerc);
        if (runningApoPerc) {
            abortApoPerc = true;
        }
    } break;
    case PopPred : {
        removeFromList(PopPerc);
        if (runningPopPerc) {
            abortPopPerc = true;
        }
    } break;
    case AprPerc : {
        removeFromList(AprPerc);
        if (runningAprPerc) {
            abortAprPerc = true;
        }
    } break;
    case ApoPerc : {
        removeFromList(ApoPerc);
        if (runningApoPerc) {
            abortApoPerc = true;
        }
    } break;
    case PopPerc : {
        removeFromList(PopPerc);
        if (runningPopPerc) {
            abortPopPerc = true;
        }
    } break;
    case AdjPred : {
        removeFromList(AdjPred);
    } break;
    case AdjPerc : {
        removeFromList(AdjPerc);
        if (runningAdjPerc) {
            abortAdjPerc = true;
        }
    } break;
    default : break;
    }

    list.append({specCopy, what});
    dataReady.wakeOne();
}


void CalculationBuffer::get(Tucuxi::Gui::Core::PredictionSpec **spec, calculationType *what)
{
    QMutexLocker locker(&mutex);
    while (list.isEmpty())
        dataReady.wait(&mutex);
    itemType item = list.first();
    *spec = item.spec;
    *what = item.type;
    switch (*what) {
    case Quit    : break;
    case PopPerc : runningPopPerc = true; break;
    case AprPerc : runningAprPerc = true; break;
    case ApoPerc : runningApoPerc = true; break;
    case AdjPerc : runningAdjPerc = true; break;
    default : break;
    }

    list.removeFirst();
}


bool CalculationBuffer::shouldAbortPopPerc()
{
    QMutexLocker locker(&mutex);
    bool shouldAbort = abortPopPerc;
    if (shouldAbort) {
        abortPopPerc = false;
        runningPopPerc = false;
    }
    return shouldAbort;
}

bool CalculationBuffer::shouldAbortAprPerc()
{
    QMutexLocker locker(&mutex);
    bool shouldAbort = abortAprPerc;
    if (shouldAbort) {
        abortAprPerc = false;
        runningAprPerc = false;
    }
    return shouldAbort;
}

bool CalculationBuffer::shouldAbortApoPerc()
{
    QMutexLocker locker(&mutex);
    bool shouldAbort = abortApoPerc;
    if (shouldAbort) {
        abortApoPerc = false;
        runningApoPerc = false;
    }
    return shouldAbort;
}

bool CalculationBuffer::shouldAbortAdjPerc()
{
    QMutexLocker locker(&mutex);
    bool shouldAbort = abortAdjPerc;
    if (shouldAbort) {
        abortAdjPerc = false;
        runningAdjPerc = false;
    }
    return shouldAbort;
}

void CalculationBuffer::popPredComputed(Tucuxi::Gui::Core::PredictionResult* pred)
{
    MESS("End Pop Pred");
    QMutexLocker locker(&mutex);

    emit sig_popPredUpdated(pred);
}

void CalculationBuffer::aprPredComputed(Tucuxi::Gui::Core::PredictionResult* pred)
{
    MESS("End Apr Pred");
    QMutexLocker locker(&mutex);

    emit sig_aprPredUpdated(pred);
}

void CalculationBuffer::apoPredComputed(Tucuxi::Gui::Core::PredictionResult* pred)
{
    MESS("End Apo Pred");
    QMutexLocker locker(&mutex);

    emit sig_apoPredUpdated(pred);
}

void CalculationBuffer::revPredComputed(Tucuxi::Gui::Core::PredictionResult* pred)
{
    MESS("End Rev Pred");
    QMutexLocker locker(&mutex);

    emit sig_revPredUpdated(pred);
}

void CalculationBuffer::popPercComputed(Tucuxi::Gui::Core::PredictionResult* pred)
{
    MESS("End Pop Perc");
    QMutexLocker locker(&mutex);

    runningPopPerc = false;
    abortPopPerc = false;

    emit sig_popPercUpdated(pred);
}

void CalculationBuffer::aprPercComputed(Tucuxi::Gui::Core::PredictionResult* pred)
{
    MESS("End Apr Perc");
    QMutexLocker locker(&mutex);

    runningAprPerc = false;
    abortAprPerc = false;

    emit sig_aprPercUpdated(pred);
}

void CalculationBuffer::apoPercComputed(Tucuxi::Gui::Core::PredictionResult* pred)
{
    MESS("End Apo Perc");
    QMutexLocker locker(&mutex);

    runningApoPerc = false;
    abortApoPerc = false;

    emit sig_apoPercUpdated(pred);
}

void CalculationBuffer::adjPercComputed(Tucuxi::Gui::Core::PredictionResult* pred)
{
    MESS("End Adj Perc");
    QMutexLocker locker(&mutex);

    runningAdjPerc = false;
    abortAdjPerc = false;

    emit sig_adjPercUpdated(pred);
}

void CalculationBuffer::adjustmentsComputed(QList<Tucuxi::Gui::Core::Dosage*> pred)
{
    MESS("End Adjustment");
    QMutexLocker locker(&mutex);

    emit sig_adjustmentsUpdated(pred);
}

void CalculationBuffer::adjPredComputed(Tucuxi::Gui::Core::PredictionResult* pred)
{
    MESS("End Pop Perc");
    QMutexLocker locker(&mutex);

    emit sig_adjPredUpdated(pred);
}

bool PopPercAborter::shouldAbort()
{
    return buffer->shouldAbortPopPerc();
}

bool AprPercAborter::shouldAbort()
{
    return buffer->shouldAbortAprPerc();
}

bool ApoPercAborter::shouldAbort()
{
    return buffer->shouldAbortApoPerc();
}


bool AdjPercAborter::shouldAbort()
{
    return buffer->shouldAbortAdjPerc();
}
