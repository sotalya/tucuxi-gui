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


#ifndef TARGETTABCONTROLLER_H
#define TARGETTABCONTROLLER_H


#include <QObject>

#include "core/utils/autoproperty.h"
#include "abstractviewcontroller.h"
#include "core/dal/drug/target.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


class TargetList;

}
}
}

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class TargetTabController : public AbstractViewController
{
    Q_OBJECT

public:
    explicit TargetTabController(QObject *parent = nullptr);

    //! This property is used by TargetTab.qml to access the list of targets
    STD_PROPERTY_DECL(Tucuxi::Gui::Core::TargetList*, targets, Targets)

public:
    Q_INVOKABLE void addTarget();
    Q_INVOKABLE void removeTarget(int index);
    Q_INVOKABLE void setTargetType(int index, int value);
    Q_INVOKABLE void setCminDbValue(int index, double value);
    Q_INVOKABLE void setCbestDbValue(int index, double value);
    Q_INVOKABLE void setCmaxDbValue(int index, double value);
    Q_INVOKABLE void setTminDbValue(int index, double value);
    Q_INVOKABLE void setTbestDbValue(int index, double value);
    Q_INVOKABLE void setTmaxDbValue(int index, double value);
    Q_INVOKABLE void setMicDbValue(int index, double value);
    Q_INVOKABLE void setUnit(int index, QString unitString);

    Q_INVOKABLE void forceRefresh();

    Q_INVOKABLE QStringList getTargetTypes();

    Q_INVOKABLE QString calculateUnit(int type, QString baseUnit);

private:
    bool isIndexValid(int index);

    void adaptMin(double &min, double &best, double &max, const double dMinBest, const double dMaxBest);
    void adaptBest(double &min, double &best, double &max, const double dMinBest, const double dMaxBest);
    void adaptMax(double &min, double &best, double &max, const double dMinBest, const double dMaxBest);

    void prepareCupdate(int index, double &min, double &best, double &max, double &dMinBest, double& dMaxBest);
    void updateCvalues(int index, double &min, double &best, double &max);

    void prepareTupdate(int index, double &min, double &best, double &max, double &dMinBest, double& dMaxBest);
    void updateTvalues(int index, double &min, double &best, double &max);
};

}
}
}

#endif // TARGETTABCONTROLLER_H
