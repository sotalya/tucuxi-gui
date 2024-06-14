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


#include "multithreadedcalculationcontroller.h"

#include "core/dal/predictionspec.h"

#include "core/utils/connect.h"

#define nbThreads 3

using namespace Tucuxi;
using namespace Gui::Core;
using namespace Tucuxi::Gui::GuiUtils;

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

void MultiThreadedCalculationController::computePopPred(Tucuxi::Gui::Core::PredictionSpec* prediction)
{
    buffers[PREDICTION]->pushPopPred(prediction);
}

void MultiThreadedCalculationController::computePopPerc(Tucuxi::Gui::Core::PredictionSpec* prediction)
{
    buffers[PERCENTILES]->pushPopPerc(prediction);
    _popperctimer.start(POPTIMEOUT);
}

void MultiThreadedCalculationController::computeAprPred(Tucuxi::Gui::Core::PredictionSpec* prediction)
{
    buffers[PREDICTION]->pushAprPred(prediction);
}

void MultiThreadedCalculationController::computeAprPerc(Tucuxi::Gui::Core::PredictionSpec* prediction)
{
    buffers[PERCENTILES]->pushAprPerc(prediction);
    _aprperctimer.start(APRTIMEOUT);
}

void MultiThreadedCalculationController::computeApoPred(Tucuxi::Gui::Core::PredictionSpec* prediction)
{
    buffers[PREDICTION]->pushApoPred(prediction);
}

void MultiThreadedCalculationController::computeApoPerc(Tucuxi::Gui::Core::PredictionSpec* prediction)
{
    buffers[PERCENTILES]->pushApoPerc(prediction);
    _apoperctimer.start(APOTIMEOUT);
}

void MultiThreadedCalculationController::computeAdjPerc(Tucuxi::Gui::Core::PredictionSpec* prediction)
{
    buffers[PERCENTILES]->pushAdjPerc(prediction);
    _adjperctimer.start(ADJTIMEOUT);
}

void MultiThreadedCalculationController::computeRevPred(Tucuxi::Gui::Core::PredictionSpec* prediction)
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
