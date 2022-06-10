//@@license@@

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
namespace GuiCore {
    class Dosage;
    class PredictionSpec;
    class PredictionResult;
}
}

class CalculationBuffer;

namespace Tucuxi {
namespace Common {
    class DateTime;
}
namespace Core {
    class DrugModel;
    class DrugTreatment;
    enum class PredictionParameterType;
}}


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
    void popPredComputed(Tucuxi::GuiCore::PredictionResult*);
    void aprPredComputed(Tucuxi::GuiCore::PredictionResult*);
    void apoPredComputed(Tucuxi::GuiCore::PredictionResult*);
    void revPredComputed(Tucuxi::GuiCore::PredictionResult*);
    void adjPredComputed(Tucuxi::GuiCore::PredictionResult*);
    void popPercComputed(Tucuxi::GuiCore::PredictionResult*);
    void aprPercComputed(Tucuxi::GuiCore::PredictionResult*);
    void apoPercComputed(Tucuxi::GuiCore::PredictionResult*);
    void adjPercComputed(Tucuxi::GuiCore::PredictionResult*);
    void adjustmentsComputed(QList<Tucuxi::GuiCore::Dosage*>);

public slots:
    virtual void abortAll() = 0;
    virtual void computePopPred(Tucuxi::GuiCore::PredictionSpec*) = 0;
    virtual void computePopPerc(Tucuxi::GuiCore::PredictionSpec*) = 0;
    virtual void computeAprPred(Tucuxi::GuiCore::PredictionSpec*) = 0;
    virtual void computeAprPerc(Tucuxi::GuiCore::PredictionSpec*) = 0;
    virtual void computeApoPred(Tucuxi::GuiCore::PredictionSpec*) = 0;
    virtual void computeApoPerc(Tucuxi::GuiCore::PredictionSpec*) = 0;
    virtual void computeRevPred(Tucuxi::GuiCore::PredictionSpec*) = 0;
    virtual void computeAdjPred(Tucuxi::GuiCore::PredictionSpec*) = 0;
    virtual void computeAdjPerc(Tucuxi::GuiCore::PredictionSpec*) = 0;

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
    void computePopPred(Tucuxi::GuiCore::PredictionSpec*);
    void computePopPerc(Tucuxi::GuiCore::PredictionSpec*);
    void computeAprPred(Tucuxi::GuiCore::PredictionSpec*);
    void computeAprPerc(Tucuxi::GuiCore::PredictionSpec*);
    void computeApoPred(Tucuxi::GuiCore::PredictionSpec*);
    void computeApoPerc(Tucuxi::GuiCore::PredictionSpec*);
    void computeRevPred(Tucuxi::GuiCore::PredictionSpec*);
    void computeAdjPred(Tucuxi::GuiCore::PredictionSpec*);
    void computeAdjPerc(Tucuxi::GuiCore::PredictionSpec*);
    void abortAll();
//    void updateAdj();

private:
    void preparePredResults(QVector<int> time, QVector<double> data, Tucuxi::GuiCore::PredictionResult* pred);

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
    void computePopPred(Tucuxi::GuiCore::PredictionSpec*);
    void computePopPerc(Tucuxi::GuiCore::PredictionSpec*);
    void computeAprPred(Tucuxi::GuiCore::PredictionSpec*);
    void computeAprPerc(Tucuxi::GuiCore::PredictionSpec*);
    void computeApoPred(Tucuxi::GuiCore::PredictionSpec*);
    void computeApoPerc(Tucuxi::GuiCore::PredictionSpec*);
    void computeRevPred(Tucuxi::GuiCore::PredictionSpec*);
    void computeAdjPred(Tucuxi::GuiCore::PredictionSpec*);
    void computeAdjPerc(Tucuxi::GuiCore::PredictionSpec*);
    void abortAll();

signals:
    void sig_updatePopPred(Tucuxi::GuiCore::PredictionSpec*);
    void sig_updatePopPerc(Tucuxi::GuiCore::PredictionSpec*);
    void sig_updateAprPred(Tucuxi::GuiCore::PredictionSpec*);
    void sig_updateAprPerc(Tucuxi::GuiCore::PredictionSpec*);
    void sig_updateApoPred(Tucuxi::GuiCore::PredictionSpec*);
    void sig_updateApoPerc(Tucuxi::GuiCore::PredictionSpec*);
    void sig_updateRevPred(Tucuxi::GuiCore::PredictionSpec*);
    void sig_updateAdjPred(Tucuxi::GuiCore::PredictionSpec*);
    void sig_updateAdjPerc(Tucuxi::GuiCore::PredictionSpec*);

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
    void pushPopPred(Tucuxi::GuiCore::PredictionSpec*);
    void pushPopPerc(Tucuxi::GuiCore::PredictionSpec*);
    void pushAprPred(Tucuxi::GuiCore::PredictionSpec*);
    void pushAprPerc(Tucuxi::GuiCore::PredictionSpec*);
    void pushApoPred(Tucuxi::GuiCore::PredictionSpec*);
    void pushApoPerc(Tucuxi::GuiCore::PredictionSpec*);
    void pushRevPred(Tucuxi::GuiCore::PredictionSpec*);
    void pushAdjPred(Tucuxi::GuiCore::PredictionSpec*);
    void pushAdjPerc(Tucuxi::GuiCore::PredictionSpec*);

public slots:

    void popPredComputed(Tucuxi::GuiCore::PredictionResult*);
    void aprPredComputed(Tucuxi::GuiCore::PredictionResult*);
    void apoPredComputed(Tucuxi::GuiCore::PredictionResult*);
    void revPredComputed(Tucuxi::GuiCore::PredictionResult*);
    void popPercComputed(Tucuxi::GuiCore::PredictionResult*);
    void aprPercComputed(Tucuxi::GuiCore::PredictionResult*);
    void apoPercComputed(Tucuxi::GuiCore::PredictionResult*);
    void adjustmentsComputed(QList<Tucuxi::GuiCore::Dosage*>);
    void adjPredComputed(Tucuxi::GuiCore::PredictionResult*);
    void adjPercComputed(Tucuxi::GuiCore::PredictionResult*);

    bool shouldAbortPopPerc();
    bool shouldAbortAprPerc();
    bool shouldAbortApoPerc();
    bool shouldAbortAdjPerc();

    void abortAll();

    void quit();

signals:

    void sig_popPredUpdated(Tucuxi::GuiCore::PredictionResult*);
    void sig_aprPredUpdated(Tucuxi::GuiCore::PredictionResult*);
    void sig_apoPredUpdated(Tucuxi::GuiCore::PredictionResult*);
    void sig_revPredUpdated(Tucuxi::GuiCore::PredictionResult*);
    void sig_popPercUpdated(Tucuxi::GuiCore::PredictionResult*);
    void sig_aprPercUpdated(Tucuxi::GuiCore::PredictionResult*);
    void sig_apoPercUpdated(Tucuxi::GuiCore::PredictionResult*);
    void sig_adjPercUpdated(Tucuxi::GuiCore::PredictionResult*);
    void sig_adjustmentsUpdated(QList<Tucuxi::GuiCore::Dosage*>);
    void sig_adjPredUpdated(Tucuxi::GuiCore::PredictionResult*);

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
    void get(Tucuxi::GuiCore::PredictionSpec **spec, calculationType *what);

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
        Tucuxi::GuiCore::PredictionSpec* spec;
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
    void push(Tucuxi::GuiCore::PredictionSpec*spec, calculationType what);
};


#endif // CALCULATIONCONTROLLER_H
