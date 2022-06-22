//@@license@@

#ifndef CHARTDATACONTROLLER_H
#define CHARTDATACONTROLLER_H

#include <QObject>
#include <QDateTime>

#include "admin/src/dal/validationstatus.h"

class CalculationController;

namespace Tucuxi {
namespace Gui {
namespace Core {

class ChartData;
class PredictionSpec;
class PredictionResult;
}
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

    void setCurrentTab(Tucuxi::Gui::Admin::StepType::Enum type);

signals:

public slots:
    void popCalcFailed();
    void aprCalcFailed();
    void apoCalcFailed();
    void adjCalcFailed();
    // To be removed
    void togglePercentiles(int);



    void initiateUpdatePopPred();
    void receiveResultsPopPred(Tucuxi::Gui::Core::PredictionResult*);
    void initiateUpdatePopPerc();
    void receiveResultsPopPerc(Tucuxi::Gui::Core::PredictionResult*);
    void initiateUpdateAprPred();
    void receiveResultsAprPred(Tucuxi::Gui::Core::PredictionResult*);
    void initiateUpdateAprPerc();
    void receiveResultsAprPerc(Tucuxi::Gui::Core::PredictionResult*);
    void initiateUpdateApoPred();
    void receiveResultsApoPred(Tucuxi::Gui::Core::PredictionResult*);
    void initiateUpdateApoPerc();
    void receiveResultsApoPerc(Tucuxi::Gui::Core::PredictionResult*);
    void initiateUpdateRevPred();
    void receiveResultsRevPred(Tucuxi::Gui::Core::PredictionResult*);
    void initiateUpdateAdjPred();
    void receiveResultsAdjPred(Tucuxi::Gui::Core::PredictionResult*);
    void initiateUpdateAdjPerc();
    void receiveResultsAdjPerc(Tucuxi::Gui::Core::PredictionResult*);


protected:

    bool isPopulationValid() const;
    bool isAPrioriValid() const;
    bool isAPosterioriValid() const;
    bool isReverseValid() const;
    bool isAdjustedValid() const;

    Tucuxi::Gui::Admin::StepType::Enum _currentTab;

    CalculationController *calculationController;


public:

    Tucuxi::Gui::Core::ChartData* chartData;

    Tucuxi::Gui::Core::PredictionSpec* predictionspec;
};

#endif // CHARTDATACONTROLLER_H
