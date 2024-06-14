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


#ifndef PERCENTILEPAIR_H
#define PERCENTILEPAIR_H

#include <QObject>
#include "core/utils/autoproperty.h"
#include "predictiondata.h"
#include "core/dal/entity.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


class PercentileData : public Entity
{
    Q_OBJECT
    ENTITY_UTILS(PercentileData)
    AUTO_PROPERTY_DECL(double, percentile, Percentile)
    AUTO_PROPERTY_DECL(PredictionData*, predictionData, PredictionData)
public:
    explicit PercentileData(AbstractRepository *repository, QObject *parent = nullptr);
};

QML_POINTERLIST_CLASS_DECL(PercentileDataList, PercentileData)
}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::PercentileData*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::PercentileData*>)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::PercentileDataList*)
#endif // PERCENTILEPAIR_H
