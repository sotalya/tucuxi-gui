//@@license@@

#include "multithreadedcalculationcontroller.h"

#include "core/dal/predictionspec.h"

#include "core/utils/connect.h"

#define nbThreads 3

using namespace Tucuxi;
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
        CONNECT(controller, SIGNAL(popPredComputed(Tucuxi::GuiCore::PredictionResult*)), buffer, SLOT(popPredComputed(Tucuxi::GuiCore::PredictionResult*)));
        CONNECT(controller, SIGNAL(aprPredComputed(Tucuxi::GuiCore::PredictionResult*)), buffer, SLOT(aprPredComputed(Tucuxi::GuiCore::PredictionResult*)));
        CONNECT(controller, SIGNAL(apoPredComputed(Tucuxi::GuiCore::PredictionResult*)), buffer, SLOT(apoPredComputed(Tucuxi::GuiCore::PredictionResult*)));
        CONNECT(controller, SIGNAL(revPredComputed(Tucuxi::GuiCore::PredictionResult*)), buffer, SLOT(revPredComputed(Tucuxi::GuiCore::PredictionResult*)));
        CONNECT(controller, SIGNAL(popPercComputed(Tucuxi::GuiCore::PredictionResult*)), buffer, SLOT(popPercComputed(Tucuxi::GuiCore::PredictionResult*)));
        CONNECT(controller, SIGNAL(aprPercComputed(Tucuxi::GuiCore::PredictionResult*)), buffer, SLOT(aprPercComputed(Tucuxi::GuiCore::PredictionResult*)));
        CONNECT(controller, SIGNAL(apoPercComputed(Tucuxi::GuiCore::PredictionResult*)), buffer, SLOT(apoPercComputed(Tucuxi::GuiCore::PredictionResult*)));
        CONNECT(controller, SIGNAL(adjPercComputed(Tucuxi::GuiCore::PredictionResult*)), buffer, SLOT(adjPercComputed(Tucuxi::GuiCore::PredictionResult*)));
        CONNECT(controller, SIGNAL(adjustmentsComputed(QList<Tucuxi::GuiCore::Dosage*>)), buffer, SLOT(adjustmentsComputed(QList<Tucuxi::GuiCore::Dosage*>)));
        CONNECT(controller, SIGNAL(adjPredComputed(Tucuxi::GuiCore::PredictionResult*)), buffer, SLOT(adjPredComputed(Tucuxi::GuiCore::PredictionResult*)));
        CONNECT(controller, SIGNAL(popCalcFail()), this, SIGNAL(popCalcFail()));
        CONNECT(controller, SIGNAL(aprCalcFail()), this, SIGNAL(aprCalcFail()));
        CONNECT(controller, SIGNAL(apoCalcFail()), this, SIGNAL(apoCalcFail()));
        CONNECT(controller, SIGNAL(adjCalcFail()), this, SIGNAL(adjCalcFail()));

        CONNECT(buffer, SIGNAL(sig_popPredUpdated(Tucuxi::GuiCore::PredictionResult*)), this, SIGNAL(popPredComputed(Tucuxi::GuiCore::PredictionResult*)));
        CONNECT(buffer, SIGNAL(sig_aprPredUpdated(Tucuxi::GuiCore::PredictionResult*)), this, SIGNAL(aprPredComputed(Tucuxi::GuiCore::PredictionResult*)));
        CONNECT(buffer, SIGNAL(sig_apoPredUpdated(Tucuxi::GuiCore::PredictionResult*)), this, SIGNAL(apoPredComputed(Tucuxi::GuiCore::PredictionResult*)));
        CONNECT(buffer, SIGNAL(sig_revPredUpdated(Tucuxi::GuiCore::PredictionResult*)), this, SIGNAL(revPredComputed(Tucuxi::GuiCore::PredictionResult*)));
        CONNECT(buffer, SIGNAL(sig_popPercUpdated(Tucuxi::GuiCore::PredictionResult*)), this, SIGNAL(popPercComputed(Tucuxi::GuiCore::PredictionResult*)));
        CONNECT(buffer, SIGNAL(sig_aprPercUpdated(Tucuxi::GuiCore::PredictionResult*)), this, SIGNAL(aprPercComputed(Tucuxi::GuiCore::PredictionResult*)));
        CONNECT(buffer, SIGNAL(sig_apoPercUpdated(Tucuxi::GuiCore::PredictionResult*)), this, SIGNAL(apoPercComputed(Tucuxi::GuiCore::PredictionResult*)));
        CONNECT(buffer, SIGNAL(sig_adjPercUpdated(Tucuxi::GuiCore::PredictionResult*)), this, SIGNAL(adjPercComputed(Tucuxi::GuiCore::PredictionResult*)));
        CONNECT(buffer, SIGNAL(sig_adjustmentsUpdated(QList<Tucuxi::GuiCore::Dosage*>)), this, SIGNAL(adjustmentsComputed(QList<Tucuxi::GuiCore::Dosage*>)));
        CONNECT(buffer, SIGNAL(sig_adjPredUpdated(Tucuxi::GuiCore::PredictionResult*)), this, SIGNAL(adjPredComputed(Tucuxi::GuiCore::PredictionResult*)));

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

void MultiThreadedCalculationController::computePopPred(Tucuxi::GuiCore::PredictionSpec* prediction)
{
    buffers[PREDICTION]->pushPopPred(prediction);
}

void MultiThreadedCalculationController::computePopPerc(Tucuxi::GuiCore::PredictionSpec* prediction)
{
    buffers[PERCENTILES]->pushPopPerc(prediction);
    _popperctimer.start(POPTIMEOUT);
}

void MultiThreadedCalculationController::computeAprPred(Tucuxi::GuiCore::PredictionSpec* prediction)
{
    buffers[PREDICTION]->pushAprPred(prediction);
}

void MultiThreadedCalculationController::computeAprPerc(Tucuxi::GuiCore::PredictionSpec* prediction)
{
    buffers[PERCENTILES]->pushAprPerc(prediction);
    _aprperctimer.start(APRTIMEOUT);
}

void MultiThreadedCalculationController::computeApoPred(Tucuxi::GuiCore::PredictionSpec* prediction)
{
    buffers[PREDICTION]->pushApoPred(prediction);
}

void MultiThreadedCalculationController::computeApoPerc(Tucuxi::GuiCore::PredictionSpec* prediction)
{
    buffers[PERCENTILES]->pushApoPerc(prediction);
    _apoperctimer.start(APOTIMEOUT);
}

void MultiThreadedCalculationController::computeAdjPerc(Tucuxi::GuiCore::PredictionSpec* prediction)
{
    buffers[PERCENTILES]->pushAdjPerc(prediction);
    _adjperctimer.start(ADJTIMEOUT);
}

void MultiThreadedCalculationController::computeRevPred(Tucuxi::GuiCore::PredictionSpec* prediction)
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
