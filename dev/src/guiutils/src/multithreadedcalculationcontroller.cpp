#include "multithreadedcalculationcontroller.h"

#include "core/dal/predictionspec.h"

#include "core/utils/connect.h"

#define nbThreads 3

using namespace ezechiel;
using namespace core;

MultiThreadedCalculationController::MultiThreadedCalculationController(QObject *parent) : CalculationController(parent),
    nbDisengaged(0)
{
    for(int i = 0; i < nbThreads; i++) {

        // First create the 3 objects

        // No parent, else moving to a thread don't work
        LocalCalculationController *controller = new LocalCalculationController();
        CalculationBuffer *buffer = new CalculationBuffer(this);
        CalculationThread *workerThread = new CalculationThread(this);

        // Add them to the collections
        controllers.push_back(controller);
        buffers.push_back(buffer);
        workerThreads.push_back(workerThread);

        workerThread->setObjectName("TucuxiWorkerThread");
        workerThread->setCalculationBuffer(buffer);
        workerThread->setCalculationController(controller);
        controller->moveToThread(workerThread);



        controller->setAborters(new PopPercAborter(buffer), new AprPercAborter(buffer),
                                new ApoPercAborter(buffer), new AdjPercAborter(buffer));


        //CONNECT(controller, SIGNAL(disengage()), this, SIGNAL(disengage()));
        //CONNECT(controller, SIGNAL(engage()), this, SIGNAL(engage()));
        CONNECT(controller, SIGNAL(disengage()), this, SLOT(intDisengage()));
        CONNECT(controller, SIGNAL(engage()), this, SLOT(intEngage()));
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

        connect(&_adjperctimer, &QTimer::timeout, this, [this](){
            buffer->abortIfRunningAdjPerc();
        });
#endif // 0
        //#endif // COMPILE_WITH_TUCUCORE

        workerThread->start();
    }
}

MultiThreadedCalculationController::~MultiThreadedCalculationController()
{
    abortAll();
    terminateWorker();
    // Wait for the thread to terminate

    for(int i = 0; i < nbThreads; i++) {
        workerThreads[i]->wait();
    }
}


void MultiThreadedCalculationController::intEngage()
{
    nbDisengaged --;
    if (nbDisengaged == 0) {
        emit engage();
    }
}

void MultiThreadedCalculationController::intDisengage()
{
    nbDisengaged ++;
    if (nbDisengaged == 1) {
        emit disengage();
    }
}


#define PREDICTION  0
#define ADJUSTMENTS 1
#define PERCENTILES 2


#define APRTIMEOUT 20000
#define APOTIMEOUT 35000
#define POPTIMEOUT 20000
#define ADJTIMEOUT 35000

void MultiThreadedCalculationController::computePopPred(ezechiel::core::PredictionSpec* prediction)
{
    buffers[PREDICTION]->pushPopPred(prediction);
}

void MultiThreadedCalculationController::computePopPerc(ezechiel::core::PredictionSpec* prediction)
{
    buffers[PERCENTILES]->pushPopPerc(prediction);
    _popperctimer.start(POPTIMEOUT);
}

void MultiThreadedCalculationController::computeAprPred(ezechiel::core::PredictionSpec* prediction)
{
    buffers[PREDICTION]->pushAprPred(prediction);
}

void MultiThreadedCalculationController::computeAprPerc(ezechiel::core::PredictionSpec* prediction)
{
    buffers[PERCENTILES]->pushAprPerc(prediction);
    _aprperctimer.start(APRTIMEOUT);
}

void MultiThreadedCalculationController::computeApoPred(ezechiel::core::PredictionSpec* prediction)
{
    buffers[PREDICTION]->pushApoPred(prediction);
}

void MultiThreadedCalculationController::computeApoPerc(ezechiel::core::PredictionSpec* prediction)
{
    buffers[PERCENTILES]->pushApoPerc(prediction);
    _apoperctimer.start(APOTIMEOUT);
}

void MultiThreadedCalculationController::computeAdjPerc(ezechiel::core::PredictionSpec* prediction)
{
    buffers[PERCENTILES]->pushAdjPerc(prediction);
    _adjperctimer.start(ADJTIMEOUT);
}

void MultiThreadedCalculationController::computeRevPred(ezechiel::core::PredictionSpec* prediction)
{
    buffers[ADJUSTMENTS]->pushRevPred(prediction);
}

void MultiThreadedCalculationController::computeAdjPred(PredictionSpec* prediction)
{
    buffers[ADJUSTMENTS]->pushAdjPred(prediction);
}

void MultiThreadedCalculationController::abortAll()
{
    for(int i = 0; i < nbThreads; i++) {
        buffers[i]->abortAll();
    }
}

void MultiThreadedCalculationController::terminateWorker()
{
    for(int i = 0; i < nbThreads; i++) {
        buffers[i]->quit();
    }
}
