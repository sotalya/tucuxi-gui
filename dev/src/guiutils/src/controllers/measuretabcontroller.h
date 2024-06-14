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


#ifndef MEASURETABCONTROLLER_H
#define MEASURETABCONTROLLER_H

#include <QObject>
#include <QDateTime>

#include "core/utils/autoproperty.h"
#include "abstractviewcontroller.h"

Q_MOC_INCLUDE("core/dal/coremeasure.h")

namespace Tucuxi {
namespace Gui {
namespace Core {

class CoreMeasureList;

}
}
}

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class MeasureTabController : public AbstractViewController
{
    Q_OBJECT
public:
    explicit MeasureTabController(QObject *parent = nullptr);

    //! This property is used by MeasureTab.qml to access the list of measures
    STD_PROPERTY_DECL(Tucuxi::Gui::Core::CoreMeasureList*, measures, Measures)

public:
    Q_INVOKABLE void addMeasure();
    Q_INVOKABLE void removeMeasure(int index);
    Q_INVOKABLE void setSampleId(int index, const QString &id);
    Q_INVOKABLE void setDbValue(int index, double value);
    Q_INVOKABLE void setMoment(int index, QDateTime time);
    Q_INVOKABLE void setArrivalDate(int index, QDateTime time);
    Q_INVOKABLE void setEnable(int index, bool enable);

    Q_INVOKABLE void forceRefresh();

private:
    bool isIndexValid(int index);
};

}
}
}

#endif // MEASURETABCONTROLLER_H
