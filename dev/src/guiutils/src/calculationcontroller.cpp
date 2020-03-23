
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
#include "core/dal/drug/doses.h"
#include "core/dal/drug/adme.h"
#include "core/utils/logging.h"
#include "core/utils/connect.h"
#include "errors_guiutils.h"
#include "apputils/src/calculationrequestlogger.h"
#include "guiutils/src/predictionspeccopier.h"
#include "core/dal/drug/standardtreatment.h"


#define APRTIMEOUT 20000
#define APOTIMEOUT 35000
#define POPTIMEOUT 20000
#define ADJTIMEOUT 35000

using namespace ezechiel;
using namespace core;

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
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Running population prediction.");

    DataSet dataSet;
    PopulationTraits parametersTraits;
    PointsTraits pointsTraits(prediction->getStartDate(),
                              prediction->getEndDate(),
                              prediction->getNbPoints(),
                              &parametersTraits);

    PredictionResult* pred = CoreFactory::createEntity<PredictionResult>(ABSTRACTREPO);

    CORE->getProcessingInterface()->points(prediction->getAnalysis(),
                                           pointsTraits,
                                           *pred);

    pred->moveToThread(qApp->thread());
    emit popPredComputed(pred);
    emit engage();
}

void LocalCalculationController::preparePredResults(QVector<int> time, QVector<double> data, PredictionResult* pred) {
    Predictive* predictive = CoreFactory::createEntity<Predictive>(ABSTRACTREPO, pred);
    pred->setPredictive(predictive);
    PredictionData* pdata = CoreFactory::createEntity<PredictionData>(ABSTRACTREPO, predictive);
    predictive->setPredictionData(pdata);
    FancyPoints* fpts = CoreFactory::createEntity<FancyPoints>(ABSTRACTREPO, pdata);
    pdata->setPoints(fpts);
    for (int i = 0; i < time.size(); ++i) {
        FancyPoint* fpt = CoreFactory::createEntity<FancyPoint>(ABSTRACTREPO, fpts);
        fpt->setTime(time[i]);
        fpt->setValue(data[i]);
        fpts->append(fpt);
    }
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Ending population prediction.");
    pred->moveToThread(qApp->thread());
}

void LocalCalculationController::computePopPerc(PredictionSpec* prediction)
{
    emit disengage();
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Running population percentiles.");

//    std::vector<double> percentiles = {10, 25, 75, 90};
    QMap<double, DataSet> percsDataSets;

    PopulationTraits parametersTraits;
    PercentilesTraits percentilesTraits(prediction->getNbPoints(),
//                                        percentiles,
                                        prediction->getPercentileList(),
                                        prediction->getStartDate(),
                                        prediction->getEndDate(),
                                        &parametersTraits);

    percentilesTraits.options[QString("percentile_method")] = QVariant::fromValue(2);

    PredictionResult* pred = CoreFactory::createEntity<PredictionResult>(ABSTRACTREPO);

    ProcessingResult result = CORE->getProcessingInterface()->percentiles(prediction->getAnalysis(),
                                                percentilesTraits,
                                                *pred,
                                                popPercAborter);

    if (result == ProcessingResult::Aborted) {
        emit popCalcFail();
        emit engage();
        return;
    }
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Ending population percentiles.");
    pred->moveToThread(qApp->thread());
    emit popPercComputed(pred);
    emit engage();
}

void LocalCalculationController::computeAprPred(PredictionSpec* prediction)
{
    emit disengage();
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Running a priori prediction.");

    AprioriTraits parametersTraits;
    PointsTraits pointsTraits(prediction->getStartDate(),
                              prediction->getEndDate(),
                              prediction->getNbPoints(),
                              &parametersTraits);

    PredictionResult* pred = CoreFactory::createEntity<PredictionResult>(ABSTRACTREPO);

    CORE->getProcessingInterface()->points(prediction->getAnalysis(),
                                           pointsTraits,
                                           *pred);

    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Ending a priori prediction.");

    pred->moveToThread(qApp->thread());
    emit aprPredComputed(pred);
    emit engage();

}

void LocalCalculationController::computeAprPerc(PredictionSpec* prediction)
{
    emit disengage();
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Running apriori percentiles.");

//    std::vector<double> percentiles = {10, 25, 75, 90};

    AprioriTraits parametersTraits;
    PercentilesTraits percentilesTraits(prediction->getNbPoints(),
//                                        percentiles,
                                        prediction->getPercentileList(),
                                        prediction->getStartDate(),
                                        prediction->getEndDate(),
                                        &parametersTraits);

    percentilesTraits.options[QString("percentile_method")] = QVariant::fromValue(2);

    PredictionResult* pred = CoreFactory::createEntity<PredictionResult>(ABSTRACTREPO);

    ProcessingResult result = CORE->getProcessingInterface()->percentiles(prediction->getAnalysis(),
                                                percentilesTraits,
                                                *pred,
                                                aprPercAborter);

    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Ending apriori percentiles.");

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
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Running a posteriori prediction.");

    ParamTraits* parametersTraits = new AposterioriTraits();
    PointsTraits pointsTraits(prediction->getStartDate(),
                              prediction->getEndDate(),
                              prediction->getNbPoints(),
                              parametersTraits);

    PredictionResult* pred = CoreFactory::createEntity<PredictionResult>(ABSTRACTREPO);

    CORE->getProcessingInterface()->points(prediction->getAnalysis(),
                                           pointsTraits,
                                           *pred);

    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Ending a posteriori prediction.");

    pred->moveToThread(qApp->thread());
    emit apoPredComputed(pred);
    emit engage();

    delete(parametersTraits);
}

void LocalCalculationController::computeApoPerc(PredictionSpec* prediction)
{
    emit disengage();
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Running aposteriori percentiles.");

    AposterioriTraits parametersTraits;
    PercentilesTraits percentilesTraits(prediction->getNbPoints(),
                                        prediction->getPercentileList(),
                                        prediction->getStartDate(),
                                        prediction->getEndDate(),
                                        &parametersTraits);

    PredictionResult* pred = CoreFactory::createEntity<PredictionResult>(ABSTRACTREPO);

    ProcessingResult result = CORE->getProcessingInterface()->calculateAposterioriPercentiles(prediction->getAnalysis(),
                                                percentilesTraits,
                                                *pred,
                                                apoPercAborter);

    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Ending aposteriori percentiles.");

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
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Running adjustments percentiles.");


    ParamTraits* parametersTraits;
    if (prediction->getAnalysis()->getTreatment()->getMeasures()->isEmpty()) {
        parametersTraits = new AprioriTraits(prediction->getAdjustmentDate());
    } else {
        parametersTraits = new AposterioriTraits(prediction->getAdjustmentDate());
    }


    //QDateTime start = prediction->getStartDate();
    QDateTime start = prediction->getAdjustmentDate();
    QDateTime end = prediction->getEndDate();

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

    PredictionResult* pred = CoreFactory::createEntity<PredictionResult>(ABSTRACTREPO);

    ProcessingResult result;
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


    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Ending adjustments percentiles.");

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
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Running reverse prediction.");
    //Q_ASSERT_X(dateSetting.isValid());

    ParamTraits* parametersTraits;
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

    PredictionResult* pred = CoreFactory::createEntity<PredictionResult>(ABSTRACTREPO);
    CORE->getProcessingInterface()->computeSuggestedAdjustments(prediction->getAnalysis(),
                                            reverseTraits,
                                            *pred);

    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Ending reverse prediction.");

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
    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Running a priori adjustment prediction.");

    ParamTraits* parametersTraits;
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

    PredictionResult* pred = CoreFactory::createEntity<PredictionResult>(ABSTRACTREPO);

    CORE->getProcessingInterface()->points(prediction->getAnalysis(),
                                           pointsTraits,
                                           *pred);

    EXLOG(QtDebugMsg, ezechiel::guiutils::NOEZERROR, "Ending a priori adjustment prediction.");
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

    while (1) {
        ezechiel::core::PredictionSpec *spec;

        CalculationBuffer::calculationType what;
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
    CONNECT(controller, SIGNAL(popPredComputed(ezechiel::core::PredictionResult*)), buffer, SLOT(popPredComputed(ezechiel::core::PredictionResult*)));
    CONNECT(controller, SIGNAL(aprPredComputed(ezechiel::core::PredictionResult*)), buffer, SLOT(aprPredComputed(ezechiel::core::PredictionResult*)));
    CONNECT(controller, SIGNAL(apoPredComputed(ezechiel::core::PredictionResult*)), buffer, SLOT(apoPredComputed(ezechiel::core::PredictionResult*)));
    CONNECT(controller, SIGNAL(revPredComputed(ezechiel::core::PredictionResult*)), buffer, SLOT(revPredComputed(ezechiel::core::PredictionResult*)));
    CONNECT(controller, SIGNAL(popPercComputed(ezechiel::core::PredictionResult*)), buffer, SLOT(popPercComputed(ezechiel::core::PredictionResult*)));
    CONNECT(controller, SIGNAL(aprPercComputed(ezechiel::core::PredictionResult*)), buffer, SLOT(aprPercComputed(ezechiel::core::PredictionResult*)));
    CONNECT(controller, SIGNAL(apoPercComputed(ezechiel::core::PredictionResult*)), buffer, SLOT(apoPercComputed(ezechiel::core::PredictionResult*)));
    CONNECT(controller, SIGNAL(adjPercComputed(ezechiel::core::PredictionResult*)), buffer, SLOT(adjPercComputed(ezechiel::core::PredictionResult*)));
    CONNECT(controller, SIGNAL(adjustmentsComputed(QList<ezechiel::core::Dosage*>)), buffer, SLOT(adjustmentsComputed(QList<ezechiel::core::Dosage*>)));
    CONNECT(controller, SIGNAL(adjPredComputed(ezechiel::core::PredictionResult*)), buffer, SLOT(adjPredComputed(ezechiel::core::PredictionResult*)));
    CONNECT(controller, SIGNAL(popCalcFail()), this, SIGNAL(popCalcFail()));
    CONNECT(controller, SIGNAL(aprCalcFail()), this, SIGNAL(aprCalcFail()));
    CONNECT(controller, SIGNAL(apoCalcFail()), this, SIGNAL(apoCalcFail()));
    CONNECT(controller, SIGNAL(adjCalcFail()), this, SIGNAL(adjCalcFail()));

    CONNECT(buffer, SIGNAL(sig_popPredUpdated(ezechiel::core::PredictionResult*)), this, SIGNAL(popPredComputed(ezechiel::core::PredictionResult*)));
    CONNECT(buffer, SIGNAL(sig_aprPredUpdated(ezechiel::core::PredictionResult*)), this, SIGNAL(aprPredComputed(ezechiel::core::PredictionResult*)));
    CONNECT(buffer, SIGNAL(sig_apoPredUpdated(ezechiel::core::PredictionResult*)), this, SIGNAL(apoPredComputed(ezechiel::core::PredictionResult*)));
    CONNECT(buffer, SIGNAL(sig_revPredUpdated(ezechiel::core::PredictionResult*)), this, SIGNAL(revPredComputed(ezechiel::core::PredictionResult*)));
    CONNECT(buffer, SIGNAL(sig_popPercUpdated(ezechiel::core::PredictionResult*)), this, SIGNAL(popPercComputed(ezechiel::core::PredictionResult*)));
    CONNECT(buffer, SIGNAL(sig_aprPercUpdated(ezechiel::core::PredictionResult*)), this, SIGNAL(aprPercComputed(ezechiel::core::PredictionResult*)));
    CONNECT(buffer, SIGNAL(sig_apoPercUpdated(ezechiel::core::PredictionResult*)), this, SIGNAL(apoPercComputed(ezechiel::core::PredictionResult*)));
    CONNECT(buffer, SIGNAL(sig_adjPercUpdated(ezechiel::core::PredictionResult*)), this, SIGNAL(adjPercComputed(ezechiel::core::PredictionResult*)));
    CONNECT(buffer, SIGNAL(sig_adjustmentsUpdated(QList<ezechiel::core::Dosage*>)), this, SIGNAL(adjustmentsComputed(QList<ezechiel::core::Dosage*>)));
    CONNECT(buffer, SIGNAL(sig_adjPredUpdated(ezechiel::core::PredictionResult*)), this, SIGNAL(adjPredComputed(ezechiel::core::PredictionResult*)));

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

void ThreadedCalculationController::computePopPred(ezechiel::core::PredictionSpec* prediction)
{
    buffer->pushPopPred(prediction);
}

void ThreadedCalculationController::computePopPerc(ezechiel::core::PredictionSpec* prediction)
{
    buffer->pushPopPerc(prediction);
    _popperctimer.start(POPTIMEOUT);
}

void ThreadedCalculationController::computeAprPred(ezechiel::core::PredictionSpec* prediction)
{
    buffer->pushAprPred(prediction);
}

void ThreadedCalculationController::computeAprPerc(ezechiel::core::PredictionSpec* prediction)
{
    buffer->pushAprPerc(prediction);
    _aprperctimer.start(APRTIMEOUT);
}

void ThreadedCalculationController::computeApoPred(ezechiel::core::PredictionSpec* prediction)
{
    buffer->pushApoPred(prediction);
}

void ThreadedCalculationController::computeApoPerc(ezechiel::core::PredictionSpec* prediction)
{
    buffer->pushApoPerc(prediction);
    _apoperctimer.start(APOTIMEOUT);
}

void ThreadedCalculationController::computeAdjPerc(ezechiel::core::PredictionSpec* prediction)
{
    buffer->pushAdjPerc(prediction);
    _adjperctimer.start(ADJTIMEOUT);
}

void ThreadedCalculationController::computeRevPred(ezechiel::core::PredictionSpec* prediction)
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

void CalculationBuffer::pushPopPred(ezechiel::core::PredictionSpec* predSpec)
{
    MESS("Push Pop Pred");
    predSpec->setCalculationType(ezechiel::core::PredictionSpec::CalculationType::PopPred);
    push(predSpec, PopPred);
}

void CalculationBuffer::pushPopPerc(ezechiel::core::PredictionSpec* predSpec)
{
    MESS("Push Pop Perc");
    predSpec->setCalculationType(ezechiel::core::PredictionSpec::CalculationType::PopPerc);
    push(predSpec, PopPerc);
}

void CalculationBuffer::pushAprPred(ezechiel::core::PredictionSpec* predSpec)
{
    MESS("Push Apr Pred");
    predSpec->setCalculationType(ezechiel::core::PredictionSpec::CalculationType::AprPred);
    push(predSpec, AprPred);
}

void CalculationBuffer::pushAprPerc(ezechiel::core::PredictionSpec* predSpec)
{
    MESS("Push Apr Perc");
    predSpec->setCalculationType(ezechiel::core::PredictionSpec::CalculationType::AprPerc);
    push(predSpec, AprPerc);
}

void CalculationBuffer::pushApoPred(ezechiel::core::PredictionSpec* predSpec)
{
    MESS("Push Apo Pred");
    predSpec->setCalculationType(ezechiel::core::PredictionSpec::CalculationType::ApoPred);
    push(predSpec, ApoPred);
}

void CalculationBuffer::pushApoPerc(ezechiel::core::PredictionSpec* predSpec)
{
    MESS("Push Apo Perc");
    predSpec->setCalculationType(ezechiel::core::PredictionSpec::CalculationType::ApoPerc);
    push(predSpec, ApoPerc);
}

void CalculationBuffer::pushAdjPerc(ezechiel::core::PredictionSpec* predSpec)
{
    MESS("Push Adj Perc");
    predSpec->setCalculationType(ezechiel::core::PredictionSpec::CalculationType::AdjPerc);
    push(predSpec, AdjPerc);
}

void CalculationBuffer::pushRevPred(ezechiel::core::PredictionSpec* predSpec)
{
    MESS("Push Adjustment");
    predSpec->setCalculationType(ezechiel::core::PredictionSpec::CalculationType::RevPred);
    push(predSpec, RevPred);
}

void CalculationBuffer::pushAdjPred(ezechiel::core::PredictionSpec* predSpec)
{
    MESS("Push Adjustment");
    predSpec->setCalculationType(ezechiel::core::PredictionSpec::CalculationType::AdjPred);
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
    PredictionSpec *specCopy;

    // We copy the entire object to be sure there is no race condition between the GUI thread
    // and the worker thread. Destruction of the specCopy should be done by the worker thread when
    // computation is finished
    specCopy = PredictionSpecCopier().copy(spec);

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


void CalculationBuffer::get(ezechiel::core::PredictionSpec **spec, calculationType *what)
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

void CalculationBuffer::popPredComputed(ezechiel::core::PredictionResult* pred)
{
    MESS("End Pop Pred");
    QMutexLocker locker(&mutex);

    emit sig_popPredUpdated(pred);
}

void CalculationBuffer::aprPredComputed(ezechiel::core::PredictionResult* pred)
{
    MESS("End Apr Pred");
    QMutexLocker locker(&mutex);

    emit sig_aprPredUpdated(pred);
}

void CalculationBuffer::apoPredComputed(ezechiel::core::PredictionResult* pred)
{
    MESS("End Apo Pred");
    QMutexLocker locker(&mutex);

    emit sig_apoPredUpdated(pred);
}

void CalculationBuffer::revPredComputed(ezechiel::core::PredictionResult* pred)
{
    MESS("End Rev Pred");
    QMutexLocker locker(&mutex);

    emit sig_revPredUpdated(pred);
}

void CalculationBuffer::popPercComputed(ezechiel::core::PredictionResult* pred)
{
    MESS("End Pop Perc");
    QMutexLocker locker(&mutex);

    runningPopPerc = false;
    abortPopPerc = false;

    emit sig_popPercUpdated(pred);
}

void CalculationBuffer::aprPercComputed(ezechiel::core::PredictionResult* pred)
{
    MESS("End Apr Perc");
    QMutexLocker locker(&mutex);

    runningAprPerc = false;
    abortAprPerc = false;

    emit sig_aprPercUpdated(pred);
}

void CalculationBuffer::apoPercComputed(ezechiel::core::PredictionResult* pred)
{
    MESS("End Apo Perc");
    QMutexLocker locker(&mutex);

    runningApoPerc = false;
    abortApoPerc = false;

    emit sig_apoPercUpdated(pred);
}

void CalculationBuffer::adjPercComputed(ezechiel::core::PredictionResult* pred)
{
    MESS("End Adj Perc");
    QMutexLocker locker(&mutex);

    runningAdjPerc = false;
    abortAdjPerc = false;

    emit sig_adjPercUpdated(pred);
}

void CalculationBuffer::adjustmentsComputed(QList<ezechiel::core::Dosage*> pred)
{
    MESS("End Adjustment");
    QMutexLocker locker(&mutex);

    emit sig_adjustmentsUpdated(pred);
}

void CalculationBuffer::adjPredComputed(ezechiel::core::PredictionResult* pred)
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
