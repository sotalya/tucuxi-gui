//@@license@@

#include "multithreadedcalculationcontroller.h"

#include "core/dal/predictionspec.h"

#include "core/utils/connect.h"

#define nbThreads 3

using namespace ezechiel;
using namespace GuiCore;

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
        CONNECT(controller, SIGNAL(popPredComputed(ezechiel::GuiCore::PredictionResult*)), buffer, SLOT(popPredComputed(ezechiel::GuiCore::PredictionResult*)));
        CONNECT(controller, SIGNAL(aprPredComputed(ezechiel::GuiCore::PredictionResult*)), buffer, SLOT(aprPredComputed(ezechiel::GuiCore::PredictionResult*)));
        CONNECT(controller, SIGNAL(apoPredComputed(ezechiel::GuiCore::PredictionResult*)), buffer, SLOT(apoPredComputed(ezechiel::GuiCore::PredictionResult*)));
        CONNECT(controller, SIGNAL(revPredComputed(ezechiel::GuiCore::PredictionResult*)), buffer, SLOT(revPredComputed(ezechiel::GuiCore::PredictionResult*)));
        CONNECT(controller, SIGNAL(popPercComputed(ezechiel::GuiCore::PredictionResult*)), buffer, SLOT(popPercComputed(ezechiel::GuiCore::PredictionResult*)));
        CONNECT(controller, SIGNAL(aprPercComputed(ezechiel::GuiCore::PredictionResult*)), buffer, SLOT(aprPercComputed(ezechiel::GuiCore::PredictionResult*)));
        CONNECT(controller, SIGNAL(apoPercComputed(ezechiel::GuiCore::PredictionResult*)), buffer, SLOT(apoPercComputed(ezechiel::GuiCore::PredictionResult*)));
        CONNECT(controller, SIGNAL(adjPercComputed(ezechiel::GuiCore::PredictionResult*)), buffer, SLOT(adjPercComputed(ezechiel::GuiCore::PredictionResult*)));
        CONNECT(controller, SIGNAL(adjustmentsComputed(QList<ezechiel::GuiCore::Dosage*>)), buffer, SLOT(adjustmentsComputed(QList<ezechiel::GuiCore::Dosage*>)));
        CONNECT(controller, SIGNAL(adjPredComputed(ezechiel::GuiCore::PredictionResult*)), buffer, SLOT(adjPredComputed(ezechiel::GuiCore::PredictionResult*)));
        CONNECT(controller, SIGNAL(popCalcFail()), this, SIGNAL(popCalcFail()));
        CONNECT(controller, SIGNAL(aprCalcFail()), this, SIGNAL(aprCalcFail()));
        CONNECT(controller, SIGNAL(apoCalcFail()), this, SIGNAL(apoCalcFail()));
        CONNECT(controller, SIGNAL(adjCalcFail()), this, SIGNAL(adjCalcFail()));

        CONNECT(buffer, SIGNAL(sig_popPredUpdated(ezechiel::GuiCore::PredictionResult*)), this, SIGNAL(popPredComputed(ezechiel::GuiCore::PredictionResult*)));
        CONNECT(buffer, SIGNAL(sig_aprPredUpdated(ezechiel::GuiCore::PredictionResult*)), this, SIGNAL(aprPredComputed(ezechiel::GuiCore::PredictionResult*)));
        CONNECT(buffer, SIGNAL(sig_apoPredUpdated(ezechiel::GuiCore::PredictionResult*)), this, SIGNAL(apoPredComputed(ezechiel::GuiCore::PredictionResult*)));
        CONNECT(buffer, SIGNAL(sig_revPredUpdated(ezechiel::GuiCore::PredictionResult*)), this, SIGNAL(revPredComputed(ezechiel::GuiCore::PredictionResult*)));
        CONNECT(buffer, SIGNAL(sig_popPercUpdated(ezechiel::GuiCore::PredictionResult*)), this, SIGNAL(popPercComputed(ezechiel::GuiCore::PredictionResult*)));
        CONNECT(buffer, SIGNAL(sig_aprPercUpdated(ezechiel::GuiCore::PredictionResult*)), this, SIGNAL(aprPercComputed(ezechiel::GuiCore::PredictionResult*)));
        CONNECT(buffer, SIGNAL(sig_apoPercUpdated(ezechiel::GuiCore::PredictionResult*)), this, SIGNAL(apoPercComputed(ezechiel::GuiCore::PredictionResult*)));
        CONNECT(buffer, SIGNAL(sig_adjPercUpdated(ezechiel::GuiCore::PredictionResult*)), this, SIGNAL(adjPercComputed(ezechiel::GuiCore::PredictionResult*)));
        CONNECT(buffer, SIGNAL(sig_adjustmentsUpdated(QList<ezechiel::GuiCore::Dosage*>)), this, SIGNAL(adjustmentsComputed(QList<ezechiel::GuiCore::Dosage*>)));
        CONNECT(buffer, SIGNAL(sig_adjPredUpdated(ezechiel::GuiCore::PredictionResult*)), this, SIGNAL(adjPredComputed(ezechiel::GuiCore::PredictionResult*)));

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

void MultiThreadedCalculationController::computePopPred(ezechiel::GuiCore::PredictionSpec* prediction)
{
    buffers[PREDICTION]->pushPopPred(prediction);
}

void MultiThreadedCalculationController::computePopPerc(ezechiel::GuiCore::PredictionSpec* prediction)
{
    buffers[PERCENTILES]->pushPopPerc(prediction);
    _popperctimer.start(POPTIMEOUT);
}

void MultiThreadedCalculationController::computeAprPred(ezechiel::GuiCore::PredictionSpec* prediction)
{
    buffers[PREDICTION]->pushAprPred(prediction);
}

void MultiThreadedCalculationController::computeAprPerc(ezechiel::GuiCore::PredictionSpec* prediction)
{
    buffers[PERCENTILES]->pushAprPerc(prediction);
    _aprperctimer.start(APRTIMEOUT);
}

void MultiThreadedCalculationController::computeApoPred(ezechiel::GuiCore::PredictionSpec* prediction)
{
    buffers[PREDICTION]->pushApoPred(prediction);
}

void MultiThreadedCalculationController::computeApoPerc(ezechiel::GuiCore::PredictionSpec* prediction)
{
    buffers[PERCENTILES]->pushApoPerc(prediction);
    _apoperctimer.start(APOTIMEOUT);
}

void MultiThreadedCalculationController::computeAdjPerc(ezechiel::GuiCore::PredictionSpec* prediction)
{
    buffers[PERCENTILES]->pushAdjPerc(prediction);
    _adjperctimer.start(ADJTIMEOUT);
}

void MultiThreadedCalculationController::computeRevPred(ezechiel::GuiCore::PredictionSpec* prediction)
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
