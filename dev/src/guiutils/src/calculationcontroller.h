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


#ifndef CALCULATIONCONTROLLER_H
#define CALCULATIONCONTROLLER_H

#include <QObject>
#include <QVariant>
#include <QDateTime>
#include <QThread>
#include <QTimer>
#include <QMutex>
#include <QWaitCondition>

#ifdef COMPILE_WITH_TUCUCORE

#include "tucucore/computingservice/computingaborter.h"
#include "core/aborter.h"
#else

#include "math/aborter.h"

#endif

namespace Tucuxi {
namespace Gui {
namespace Core {

    class Dosage;
    class PredictionSpec;
    class PredictionResult;
}
}
}

namespace Tucuxi {
namespace Common {
    class DateTime;
}
namespace Core {
    class DrugModel;
    class DrugTreatment;
    enum class PredictionParameterType;
}
}

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class CalculationBuffer;


#ifdef COMPILE_WITH_TUCUCORE

class TucuxiComputingAborter : public Tucuxi::Core::ComputingAborter {
public:

    bool shouldAbort() Q_DECL_OVERRIDE {
        return ezechielAborter->shouldAbort();
    }

    void setEzeChielAborter (Tucuxi::math::ProcessingAborter *aborter) {
        ezechielAborter = aborter;
    }
    Tucuxi::math::ProcessingAborter *ezechielAborter;
};

class MixAborter : public Tucuxi::math::ProcessingAborter
{
public:
    MixAborter(CalculationBuffer *b) : buffer(b) {
        tucuxiAborter = new TucuxiComputingAborter();
        tucuxiAborter->setEzeChielAborter(this);
    }

    Tucuxi::Core::ComputingAborter *getTucuxiAborter() {
        return tucuxiAborter;
    }

protected:
    CalculationBuffer *buffer;

    TucuxiComputingAborter *tucuxiAborter;

};

/**
 * @brief The PopPercAborter class
 * This class is passed to methods doing calculation for Population percentiles.
 * It only supply a method to test if the processing should be aborted or not.
 * If it returns true, the processing should be aborted and all resources freed.
 * If it returns true a second call would return false, so be carful with that.
 */
class PopPercAborter : public MixAborter
{
public:
    PopPercAborter(CalculationBuffer *b) : MixAborter(b) {}

    //! This method simply calls buffer->shouldAbortPopPerc()
    bool shouldAbort() Q_DECL_OVERRIDE;

};



/**
 * @brief The AprPercAborter class
 * This class is passed to methods doing calculation for A priori percentiles.
 * It only supply a method to test if the processing should be aborted or not.
 * If it returns true, the processing should be aborted and all resources freed.
 * If it returns true a second call would return false, so be carful with that.
 */
class AprPercAborter : public MixAborter
{
public:
    AprPercAborter(CalculationBuffer *b) : MixAborter(b) {}

    //! This method simply calls buffer->shouldAbortAprPerc()
    bool shouldAbort() Q_DECL_OVERRIDE;

};


/**
 * @brief The ApoPercAborter class
 * This class is passed to methods doing calculation for A posteriori percentiles.
 * It only supply a method to test if the processing should be aborted or not.
 * If it returns true, the processing should be aborted and all resources freed.
 * If it returns true a second call would return false, so be carful with that.
 */
class ApoPercAborter : public MixAborter
{
public:
    ApoPercAborter(CalculationBuffer *b) : MixAborter(b) {}

    //! This method simply calls buffer->shouldAbortApoPerc()
    bool shouldAbort() Q_DECL_OVERRIDE;

};
/**
 * @brief The AdjPercAborter class
 * This class is passed to methods doing calculation for adjustments percentiles.
 * It only supply a method to test if the processing should be aborted or not.
 * If it returns true, the processing should be aborted and all resources freed.
 * If it returns true a second call would return false, so be carful with that.
 */
class AdjPercAborter : public MixAborter
{
public:
    AdjPercAborter(CalculationBuffer *b) : MixAborter(b) {}

    //! This method simply calls buffer->shouldAbortApoPerc()
    bool shouldAbort() Q_DECL_OVERRIDE;

};

#else

/**
 * @brief The PopPercAborter class
 * This class is passed to methods doing calculation for Population percentiles.
 * It only supply a method to test if the processing should be aborted or not.
 * If it returns true, the processing should be aborted and all resources freed.
 * If it returns true a second call would return false, so be carful with that.
 */
class PopPercAborter : public Tucuxi::math::ProcessingAborter
{
public:
    PopPercAborter(CalculationBuffer *b) : buffer(b) {}

    //! This method simply calls buffer->shouldAbortPopPerc()
    bool shouldAbort() Q_DECL_OVERRIDE;

protected:
    CalculationBuffer *buffer;
};



/**
 * @brief The AprPercAborter class
 * This class is passed to methods doing calculation for A priori percentiles.
 * It only supply a method to test if the processing should be aborted or not.
 * If it returns true, the processing should be aborted and all resources freed.
 * If it returns true a second call would return false, so be carful with that.
 */
class AprPercAborter : public Tucuxi::math::ProcessingAborter
{
public:
    AprPercAborter(CalculationBuffer *b) : buffer(b) {}

    //! This method simply calls buffer->shouldAbortAprPerc()
    bool shouldAbort() Q_DECL_OVERRIDE;

protected:
    CalculationBuffer *buffer;
};


/**
 * @brief The ApoPercAborter class
 * This class is passed to methods doing calculation for A posteriori percentiles.
 * It only supply a method to test if the processing should be aborted or not.
 * If it returns true, the processing should be aborted and all resources freed.
 * If it returns true a second call would return false, so be carful with that.
 */
class ApoPercAborter : public Tucuxi::math::ProcessingAborter
{
public:
    ApoPercAborter(CalculationBuffer *b) : buffer(b) {}

    //! This method simply calls buffer->shouldAbortApoPerc()
    bool shouldAbort() Q_DECL_OVERRIDE;

protected:
    CalculationBuffer *buffer;
};

/**
 * @brief The AdjPercAborter class
 * This class is passed to methods doing calculation for adjustments percentiles.
 * It only supply a method to test if the processing should be aborted or not.
 * If it returns true, the processing should be aborted and all resources freed.
 * If it returns true a second call would return false, so be carful with that.
 */
class AdjPercAborter : public Tucuxi::math::ProcessingAborter
{
public:
    AdjPercAborter(CalculationBuffer *b) : buffer(b) {}

    //! This method simply calls buffer->shouldAbortAdjPerc()
    bool shouldAbort() Q_DECL_OVERRIDE;

protected:
    CalculationBuffer *buffer;
};

#endif // COMPILE_WITH_TUCUCORE

class CalculationController : public QObject
{
    Q_OBJECT
public:
    explicit CalculationController(QObject *parent = nullptr);


    virtual void setAborters(
            PopPercAborter *pop,
            AprPercAborter *apr,
            ApoPercAborter *apo,
            AdjPercAborter *adj) {
        Q_UNUSED(pop);
        Q_UNUSED(apr);
        Q_UNUSED(apo);
        Q_UNUSED(adj);
    }

signals:
    void disengage();
    void engage();
    void popCalcFail();
    void aprCalcFail();
    void apoCalcFail();
    void adjCalcFail();
    void popPredComputed(Tucuxi::Gui::Core::PredictionResult*);
    void aprPredComputed(Tucuxi::Gui::Core::PredictionResult*);
    void apoPredComputed(Tucuxi::Gui::Core::PredictionResult*);
    void revPredComputed(Tucuxi::Gui::Core::PredictionResult*);
    void adjPredComputed(Tucuxi::Gui::Core::PredictionResult*);
    void popPercComputed(Tucuxi::Gui::Core::PredictionResult*);
    void aprPercComputed(Tucuxi::Gui::Core::PredictionResult*);
    void apoPercComputed(Tucuxi::Gui::Core::PredictionResult*);
    void adjPercComputed(Tucuxi::Gui::Core::PredictionResult*);
    void adjustmentsComputed(QList<Tucuxi::Gui::Core::Dosage*>);

public slots:
    virtual void abortAll() = 0;
    virtual void computePopPred(Tucuxi::Gui::Core::PredictionSpec*) = 0;
    virtual void computePopPerc(Tucuxi::Gui::Core::PredictionSpec*) = 0;
    virtual void computeAprPred(Tucuxi::Gui::Core::PredictionSpec*) = 0;
    virtual void computeAprPerc(Tucuxi::Gui::Core::PredictionSpec*) = 0;
    virtual void computeApoPred(Tucuxi::Gui::Core::PredictionSpec*) = 0;
    virtual void computeApoPerc(Tucuxi::Gui::Core::PredictionSpec*) = 0;
    virtual void computeRevPred(Tucuxi::Gui::Core::PredictionSpec*) = 0;
    virtual void computeAdjPred(Tucuxi::Gui::Core::PredictionSpec*) = 0;
    virtual void computeAdjPerc(Tucuxi::Gui::Core::PredictionSpec*) = 0;

protected:
    QTimer _popperctimer;
    QTimer _aprperctimer;
    QTimer _apoperctimer;
    QTimer _adjperctimer;
};

class LocalCalculationController : public CalculationController
{
    Q_OBJECT
public:
    explicit LocalCalculationController(QObject *parent = nullptr);

    void setAborters(PopPercAborter *pop,
        AprPercAborter *apr,
        ApoPercAborter *apo,
                     AdjPercAborter *adj);


public slots:
    void computePopPred(Tucuxi::Gui::Core::PredictionSpec*);
    void computePopPerc(Tucuxi::Gui::Core::PredictionSpec*);
    void computeAprPred(Tucuxi::Gui::Core::PredictionSpec*);
    void computeAprPerc(Tucuxi::Gui::Core::PredictionSpec*);
    void computeApoPred(Tucuxi::Gui::Core::PredictionSpec*);
    void computeApoPerc(Tucuxi::Gui::Core::PredictionSpec*);
    void computeRevPred(Tucuxi::Gui::Core::PredictionSpec*);
    void computeAdjPred(Tucuxi::Gui::Core::PredictionSpec*);
    void computeAdjPerc(Tucuxi::Gui::Core::PredictionSpec*);
    void abortAll();
//    void updateAdj();

private:
    void preparePredResults(QVector<int> time, QVector<double> data, Tucuxi::Gui::Core::PredictionResult* pred);

    PopPercAborter *popPercAborter;
    AprPercAborter *aprPercAborter;
    ApoPercAborter *apoPercAborter;
    AdjPercAborter *adjPercAborter;

};



class CalculationThread : public QThread
{
public:
    explicit CalculationThread(QObject *parent = nullptr);

    void setCalculationController(CalculationController *controller);
    void setCalculationBuffer(CalculationBuffer *buffer);

protected:

    CalculationController *controller;
    CalculationBuffer *buffer;

    void run() Q_DECL_OVERRIDE;
};


class ThreadedCalculationController : public CalculationController
{

    Q_OBJECT
public:
    explicit ThreadedCalculationController(QObject *parent = nullptr);
    virtual ~ThreadedCalculationController();

public slots:
    void computePopPred(Tucuxi::Gui::Core::PredictionSpec*);
    void computePopPerc(Tucuxi::Gui::Core::PredictionSpec*);
    void computeAprPred(Tucuxi::Gui::Core::PredictionSpec*);
    void computeAprPerc(Tucuxi::Gui::Core::PredictionSpec*);
    void computeApoPred(Tucuxi::Gui::Core::PredictionSpec*);
    void computeApoPerc(Tucuxi::Gui::Core::PredictionSpec*);
    void computeRevPred(Tucuxi::Gui::Core::PredictionSpec*);
    void computeAdjPred(Tucuxi::Gui::Core::PredictionSpec*);
    void computeAdjPerc(Tucuxi::Gui::Core::PredictionSpec*);
    void abortAll();

signals:
    void sig_updatePopPred(Tucuxi::Gui::Core::PredictionSpec*);
    void sig_updatePopPerc(Tucuxi::Gui::Core::PredictionSpec*);
    void sig_updateAprPred(Tucuxi::Gui::Core::PredictionSpec*);
    void sig_updateAprPerc(Tucuxi::Gui::Core::PredictionSpec*);
    void sig_updateApoPred(Tucuxi::Gui::Core::PredictionSpec*);
    void sig_updateApoPerc(Tucuxi::Gui::Core::PredictionSpec*);
    void sig_updateRevPred(Tucuxi::Gui::Core::PredictionSpec*);
    void sig_updateAdjPred(Tucuxi::Gui::Core::PredictionSpec*);
    void sig_updateAdjPerc(Tucuxi::Gui::Core::PredictionSpec*);

private:

    void terminateWorker();

    CalculationThread *workerThread;
    //LocalCalculationController *controller;
    CalculationController *controller;
    CalculationBuffer *buffer;

};


/**
 * @brief The CalculationBuffer class
 * This buffer is meant to store the calculation asked.
 * A thread should push calculations and another one get these calculations to
 * process them. Then the one doing the calculation should call the slots
 * *Updated in order to signify the end of calculation with the corresponding
 * results. It allows the buffer to update its internal state and let it
 * emit signals with the results.
 * Methods abortIfRunning*() allow to ask for the termination of a long
 * processing, typically after a timeout in case something goes wrong.
 * Methods shouldAbort*() allow to test if a specific processing should be
 * aborted or not. Typically used by the Aborter* classes within the math
 * module.
 */
class CalculationBuffer : public QObject
{
    Q_OBJECT

public:
    explicit CalculationBuffer(QObject *parent = nullptr);

public slots:
    void pushPopPred(Tucuxi::Gui::Core::PredictionSpec*);
    void pushPopPerc(Tucuxi::Gui::Core::PredictionSpec*);
    void pushAprPred(Tucuxi::Gui::Core::PredictionSpec*);
    void pushAprPerc(Tucuxi::Gui::Core::PredictionSpec*);
    void pushApoPred(Tucuxi::Gui::Core::PredictionSpec*);
    void pushApoPerc(Tucuxi::Gui::Core::PredictionSpec*);
    void pushRevPred(Tucuxi::Gui::Core::PredictionSpec*);
    void pushAdjPred(Tucuxi::Gui::Core::PredictionSpec*);
    void pushAdjPerc(Tucuxi::Gui::Core::PredictionSpec*);

public slots:

    void popPredComputed(Tucuxi::Gui::Core::PredictionResult*);
    void aprPredComputed(Tucuxi::Gui::Core::PredictionResult*);
    void apoPredComputed(Tucuxi::Gui::Core::PredictionResult*);
    void revPredComputed(Tucuxi::Gui::Core::PredictionResult*);
    void popPercComputed(Tucuxi::Gui::Core::PredictionResult*);
    void aprPercComputed(Tucuxi::Gui::Core::PredictionResult*);
    void apoPercComputed(Tucuxi::Gui::Core::PredictionResult*);
    void adjustmentsComputed(QList<Tucuxi::Gui::Core::Dosage*>);
    void adjPredComputed(Tucuxi::Gui::Core::PredictionResult*);
    void adjPercComputed(Tucuxi::Gui::Core::PredictionResult*);

    bool shouldAbortPopPerc();
    bool shouldAbortAprPerc();
    bool shouldAbortApoPerc();
    bool shouldAbortAdjPerc();

    void abortAll();

    void quit();

signals:

    void sig_popPredUpdated(Tucuxi::Gui::Core::PredictionResult*);
    void sig_aprPredUpdated(Tucuxi::Gui::Core::PredictionResult*);
    void sig_apoPredUpdated(Tucuxi::Gui::Core::PredictionResult*);
    void sig_revPredUpdated(Tucuxi::Gui::Core::PredictionResult*);
    void sig_popPercUpdated(Tucuxi::Gui::Core::PredictionResult*);
    void sig_aprPercUpdated(Tucuxi::Gui::Core::PredictionResult*);
    void sig_apoPercUpdated(Tucuxi::Gui::Core::PredictionResult*);
    void sig_adjPercUpdated(Tucuxi::Gui::Core::PredictionResult*);
    void sig_adjustmentsUpdated(QList<Tucuxi::Gui::Core::Dosage*>);
    void sig_adjPredUpdated(Tucuxi::Gui::Core::PredictionResult*);

public:
    enum calculationType {
        PopPred,
        PopPerc,
        AprPred,
        AprPerc,
        ApoPred,
        ApoPerc,
        RevPred,
        AdjPred,
        AdjPerc,
        Quit
    };
    
    /**
     * @brief get Allows to get a calculation spec
     * @param spec Specifications required by the calculation method
     * @param what Determines the type of calculation to do
     * This method should be called by the worker thread. Then it can do the
     * calculation with the values retrieved.
     */
    void get(Tucuxi::Gui::Core::PredictionSpec **spec, calculationType *what);

    /**
     * @brief abortIfRunningPopPerc : abort Population percentiles calculation
     * This method tests if a Population percentiles calculation is under way.
     * If yes then it asks for its cancellation.
     * This method has no effect on calculation that could be in the buffer
     * and not currently being processed.
     */
    void abortIfRunningPopPerc();

    /**
     * @brief abortIfRunningAprPerc : abort A priori percentiles calculation
     * This method tests if an apriori percentiles calculation is under way.
     * If yes then it asks for its cancellation.
     * This method has no effect on calculation that could be in the buffer
     * and not currently being processed.
     */
    void abortIfRunningAprPerc();

    /**
     * @brief abortIfRunningApoPerc : abort A posteriori percentiles calculation
     * This method tests if an a posteriori percentiles calculation is under way.
     * If yes then it asks for its cancellation.
     * This method has no effect on calculation that could be in the buffer
     * and not currently being processed.
     */
    void abortIfRunningApoPerc();

    /**
     * @brief abortIfRunningAdjPerc : abort adjustments percentiles calculation
     * This method tests if an adjustment percentiles calculation is under way.
     * If yes then it asks for its cancellation.
     * This method has no effect on calculation that could be in the buffer
     * and not currently being processed.
     */
    void abortIfRunningAdjPerc();

protected:

    bool abortPopPerc;
    bool abortAprPerc;
    bool abortApoPerc;
    bool abortAdjPerc;

    bool runningPopPerc;
    bool runningAprPerc;
    bool runningApoPerc;
    bool runningAdjPerc;


    //! Content of the buffer. The specifications linked with the type
    //! of calculation.
    struct itemType {
        Tucuxi::Gui::Core::PredictionSpec* spec;
        calculationType type;
    };

    //! Internal buffer of calculation items
    QList<itemType> list;

    //! Mutex to protect all methods (multi-threaded access)
    QMutex mutex;

    //! Condition variable to let the worker thread wait for jobs
    QWaitCondition dataReady;

    //! Removes all calculations of a specific type from the buffer
    void removeFromList(calculationType type);

    //! Push a calculation in the buffer (generic method used internally)
    void push(Tucuxi::Gui::Core::PredictionSpec*spec, calculationType what);
};

}
}
}


#endif // CALCULATIONCONTROLLER_H
