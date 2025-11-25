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


#ifndef CHARTDATACONTROLLER_H
#define CHARTDATACONTROLLER_H

#include <QObject>
#include <QDateTime>

#include "admin/src/dal/steptypes.h"

namespace Tucuxi {
namespace Gui {
namespace Core {

class ChartData;
class PredictionSpec;
class PredictionResult;
}
}
}

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class CalculationController;

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

    void launchCompute();

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

private:
    enum computationType {
        POP_PRED,
        POP_PERC,
        APO_PRED,
        APO_PERC,
        APR_PRED,
        APR_PERC,
        REV_PRED,
        ADJ_PRED,
        ADJ_PERC
    };

    QSet<computationType> toComputeSet;



protected:

    [[nodiscard]] bool isPopulationValid() const;
    [[nodiscard]] bool isAPrioriValid() const;
    [[nodiscard]] bool isAPosterioriValid() const;
    [[nodiscard]] bool isReverseValid() const;
    [[nodiscard]] bool isAdjustedValid() const;

    Tucuxi::Gui::Admin::StepType::Enum _currentTab;

    CalculationController *calculationController;


public:

    Tucuxi::Gui::Core::ChartData* chartData;

    Tucuxi::Gui::Core::PredictionSpec* predictionspec;
};

}
}
}

#endif // CHARTDATACONTROLLER_H
