//@@license@@

#ifndef CHARTDATACONTROLLER_H
#define CHARTDATACONTROLLER_H

#include <QObject>
#include <QDateTime>

#include "admin/src/dal/validationstatus.h"

class CalculationController;

namespace ezechiel {
namespace core {
class ChartData;
class PredictionSpec;
class PredictionResult;
}
}

class ChartDataController : public QObject
{
    Q_OBJECT
public:
    explicit ChartDataController(QObject *parent = nullptr);

    void setCalculationController(CalculationController *controller);

    void start();
    void adjustmentUpdated();
    void adjustmentSettingsUpdated();
    void dosageUpdated(bool bShowPerc, bool computeRev = true);
    void viewRangeUpdated(bool bShowPerc, bool computeRev = true);
    void measureUpdated(bool bShowPerc);
    void targetUpdated();
    void covariateUpdated(bool bShowPerc);

    void setCurrentTab(StepType::Enum type);

signals:

public slots:
    void popCalcFailed();
    void aprCalcFailed();
    void apoCalcFailed();
    void adjCalcFailed();
    // To be removed
    void togglePercentiles(int);



    void initiateUpdatePopPred();
    void receiveResultsPopPred(ezechiel::core::PredictionResult*);
    void initiateUpdatePopPerc();
    void receiveResultsPopPerc(ezechiel::core::PredictionResult*);
    void initiateUpdateAprPred();
    void receiveResultsAprPred(ezechiel::core::PredictionResult*);
    void initiateUpdateAprPerc();
    void receiveResultsAprPerc(ezechiel::core::PredictionResult*);
    void initiateUpdateApoPred();
    void receiveResultsApoPred(ezechiel::core::PredictionResult*);
    void initiateUpdateApoPerc();
    void receiveResultsApoPerc(ezechiel::core::PredictionResult*);
    void initiateUpdateRevPred();
    void receiveResultsRevPred(ezechiel::core::PredictionResult*);
    void initiateUpdateAdjPred();
    void receiveResultsAdjPred(ezechiel::core::PredictionResult*);
    void initiateUpdateAdjPerc();
    void receiveResultsAdjPerc(ezechiel::core::PredictionResult*);


protected:

    bool isPopulationValid() const;
    bool isAPrioriValid() const;
    bool isAPosterioriValid() const;
    bool isReverseValid() const;
    bool isAdjustedValid() const;

    StepType::Enum _currentTab;

    CalculationController *calculationController;


public:

    ezechiel::core::ChartData* chartData;

    ezechiel::core::PredictionSpec* predictionspec;
};

#endif // CHARTDATACONTROLLER_H
