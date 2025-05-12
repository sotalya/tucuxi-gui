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


#ifndef DOSAGETABCONTROLLER_H
#define DOSAGETABCONTROLLER_H

#include <QObject>
#include <QDateTime>

#include "core/utils/autoproperty.h"
#include "abstractviewcontroller.h"
#include "core/dal/dosage.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


class DosageHistory;
class DrugModel;

}
}
}

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class DosageTabController : public AbstractViewController
{
    Q_OBJECT

public:
    explicit DosageTabController(QObject *parent = nullptr);

    //! This property is used by DosageTab.qml to access the list of dosages
    STD_PROPERTY_DECL(Tucuxi::Gui::Core::DosageHistory*, dosages, Dosages)
    //STD_PROPERTY_DECL(Tucuxi::Gui::Core::DrugModel*, drugModel, DrugModel)
    Q_PROPERTY(bool canHaveMoreDosages READ getCanHaveMoreDosages NOTIFY canHaveMoreDosagesChanged);

public:
    /**
     * @brief addDosage Adds a new dosage to the dosage history
     * The new dosage is added at the end of the history, and lasts
     * for 1 day. This could be changed in the future, but for now
     * this 1-day choice is reasonable.
     */
    Q_INVOKABLE void addDosage();
    Q_INVOKABLE void removeDosage(int index);
    Q_INVOKABLE void setDbValue(int index, double value);
    Q_INVOKABLE void setDbInterval(int index, double interval);

    // Q_INVOKABLE int getRelativeRouteValue(int index);


    /**
     * @brief setRouteValue Sets the route of administration
     * @param index index of the dosage in the dosage history
     * @param routeValue Index of the route relative to the drugmodel intakes
     * This function should be used with care, as routeValue is not the index
     * of the route in the entire routes supported by the software, but the
     * index of the route in the availables ones for the selected drug model.
     */
    // Q_INVOKABLE void setRouteValue(int index, int routeValue);
    Q_INVOKABLE void setDbTinf(int index, double value);
    Q_INVOKABLE void setAppliedTime(int index, QDateTime time);
    Q_INVOKABLE void setEndTime(int index, QDateTime time);
    Q_INVOKABLE void setHasEndDate(int index, bool hasEndDate);
    Q_INVOKABLE void setAtSteadyState(int index, bool isAtSteadyState, QDateTime lastDoseTime);
    Q_INVOKABLE void forceRefresh();
    Q_INVOKABLE bool checkModifiedDates(int index, const QDateTime &appliedDate, const QDateTime &endDate);

    Q_INVOKABLE int getNbDosages();

    Q_INVOKABLE QDateTime getNewEndTime(QDateTime start);
    bool getCanHaveMoreDosages();

    void reset();


signals:
    void canHaveMoreDosagesChanged(bool);

protected:
    bool isIndexValid(int index);

public:
    DosageTabController();

private:
    static bool compareDosage(const Tucuxi::Gui::Core::Dosage* a, const Tucuxi::Gui::Core::Dosage* b);
};

}
}
}

#endif // DOSAGETABCONTROLLER_H
