//@@license@@

#ifndef CHARTDATACONTROLLER_H
#define CHARTDATACONTROLLER_H

#include <QObject>
#include <QDateTime>

#include "admin/src/dal/validationstatus.h"

class CalculationController;

namespace Tucuxi {
namespace GuiCore {
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
    void receiveResultsPopPred(Tucuxi::GuiCore::PredictionResult*);
    void initiateUpdatePopPerc();
    void receiveResultsPopPerc(Tucuxi::GuiCore::PredictionResult*);
    void initiateUpdateAprPred();
    void receiveResultsAprPred(Tucuxi::GuiCore::PredictionResult*);
    void initiateUpdateAprPerc();
    void receiveResultsAprPerc(Tucuxi::GuiCore::PredictionResult*);
    void initiateUpdateApoPred();
    void receiveResultsApoPred(Tucuxi::GuiCore::PredictionResult*);
    void initiateUpdateApoPerc();
    void receiveResultsApoPerc(Tucuxi::GuiCore::PredictionResult*);
    void initiateUpdateRevPred();
    void receiveResultsRevPred(Tucuxi::GuiCore::PredictionResult*);
    void initiateUpdateAdjPred();
    void receiveResultsAdjPred(Tucuxi::GuiCore::PredictionResult*);
    void initiateUpdateAdjPerc();
    void receiveResultsAdjPerc(Tucuxi::GuiCore::PredictionResult*);


protected:

    bool isPopulationValid() const;
    bool isAPrioriValid() const;
    bool isAPosterioriValid() const;
    bool isReverseValid() const;
    bool isAdjustedValid() const;

    StepType::Enum _currentTab;

    CalculationController *calculationController;


public:

    Tucuxi::GuiCore::ChartData* chartData;

    Tucuxi::GuiCore::PredictionSpec* predictionspec;
};

#endif // CHARTDATACONTROLLER_H
