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


#ifndef ADJUSTMENTTABCONTROLLER_H
#define ADJUSTMENTTABCONTROLLER_H

#include <QObject>
#include <QDateTime>

#include "core/utils/autoproperty.h"
#include "abstractviewcontroller.h"

Q_MOC_INCLUDE("core/dal/dosage.h")

namespace Tucuxi {
namespace Gui {
namespace Core {


class DosageHistory;
class Dosage;
class ChartData;
class AdjustmentDosage;
}
}
}

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class AdjustmentTabController : public AbstractViewController
{
    Q_OBJECT

public:
    explicit AdjustmentTabController(QObject *parent = nullptr);

    //! This property is used by AdjustmentTab.qml to access the list of dosages
    STD_PROPERTY_DECL(Tucuxi::Gui::Core::DosageHistory*, adjustments, Adjustments)

    //! This property is not implemented using the standard macro, as it
    //! Shall refer to the PredictionSpec object
    STD_PROPERTY_DECL(QDateTime, adjustmentDate, AdjustmentDate)

    //! This property is not implemented using the standard macro, as it
    //! Shall refer to the PredictionSpec object
    STD_PROPERTY_DECL(bool, adjustmentWithLoadingDose, AdjustmentWithLoadingDose)

    //! This property is not implemented using the standard macro, as it
    //! Shall refer to the PredictionSpec object
    STD_PROPERTY_DECL(bool, adjustmentWithRestPeriod, AdjustmentWithRestPeriod)

    STD_PROPERTY_DECL(bool, isManualAdjustmentDefined, IsManualAdjustmentDefined)

public:
    Q_INVOKABLE void addAdjustment();
    Q_INVOKABLE void removeAdjustment(int index);
    Q_INVOKABLE void setDbValue(int index, double value);
    Q_INVOKABLE void setDbInterval(int index, double interval);
    Q_INVOKABLE void selectAdjustment(int index);

    void setAdjustmentDateFromMaster(QDateTime date);
    void setInitialAdjustmentDateFromMaster(QDateTime date);

    /**
     * @brief setRouteValue Sets the route of administration
     * @param index index of the dosage in the dosage history
     * @param routeValue Index of the route relative to the drugmodel intakes
     * This function should be used with care, as routeValue is not the index
     * of the route in the entire routes supported by the software, but the
     * index of the route in the availables ones for the selected drug model.
     */
    Q_INVOKABLE void setRouteValue(int index, int routeValue);
    Q_INVOKABLE void setDbTinf(int index, double value);
    Q_INVOKABLE void setAppliedTime(int index, QDateTime time);
    Q_INVOKABLE void setEndTime(int index, QDateTime time);
    Q_INVOKABLE void forceRefresh();

    void setChartData(Tucuxi::Gui::Core::ChartData *chartData);
    void onDosageUpdated();
    void reset();

private:
    bool isIndexValid(int index);
    void removeFromTreatement(const QString &type);
    static bool compareAdjustment(const Tucuxi::Gui::Core::Dosage* a, const Tucuxi::Gui::Core::Dosage* b);

    Tucuxi::Gui::Core::AdjustmentDosage* getAdjustment(const QString &type);
    Tucuxi::Gui::Core::AdjustmentDosage* getLastAdjustment(const QString &type);
    void adaptDates(const Tucuxi::Gui::Core::AdjustmentDosage *manual, Tucuxi::Gui::Core::AdjustmentDosage *suggested);

private:
    Tucuxi::Gui::Core::ChartData *_chartData;
};

}
}
}


#endif // ADJUSTMENTTABCONTROLLER_H
