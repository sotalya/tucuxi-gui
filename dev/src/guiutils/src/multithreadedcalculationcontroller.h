//@@license@@

#ifndef MULTITHREADEDCALCULATIONCONTROLLER_H
#define MULTITHREADEDCALCULATIONCONTROLLER_H

#include "calculationcontroller.h"

/**
 * @brief The MultiThreadedCalculationController class
 * This class implements 3 worker threads:
 * 1. Single predictions
 * 2. Adjustments
 * 3. Percentiles
 *
 * It allows to have single predictions more rapidly, if going straight to
 * A posteriori curves. It does not need to wait for pop and apriori percentiles
 * to finish.
 *
 * The Buffer object, as well as the Calculation and the worker thread are the
 * same as for the ThreadedCalculationController. The difference only resides in
 * MultiThreadedCalculationController methods. It creates 3 objects of these 3 classes
 * and do the correct connections. It centralizes the engage() and disengage() and
 * stores a counter to know if it should forward the engage() and disengage() signals.
 * Finally, the implementation of the various computeX() methods are specific, as
 * for each computeX(), a choice is made concerning the buffer into which the
 * calculation is added. It allows to select here what computation is performed by
 * what worker thread.
 *
 * Using this Multithreaded calculation imposes a certain caution with the
 * ProcessingInterface responsible to do the real calculation. As it is now, the
 * threads could start computations in parallel, and if the ProcessingInterface is
 * a singleton, then its methods should be reentrant in order to perform as expected.
 */
class MultiThreadedCalculationController : public CalculationController
{

    Q_OBJECT
public:
    explicit MultiThreadedCalculationController(QObject *parent = nullptr);
    virtual ~MultiThreadedCalculationController();

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

    void intEngage();
    void intDisengage();

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

    std::vector<CalculationThread *> workerThreads;
    std::vector<CalculationController *> controllers;
    std::vector<CalculationBuffer *> buffers;

    int nbDisengaged;

};

#endif // MULTITHREADEDCALCULATIONCONTROLLER_H
