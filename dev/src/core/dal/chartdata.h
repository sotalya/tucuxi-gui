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


#ifndef CHARTDATA_H
#define CHARTDATA_H

#include <QObject>
#include <QString>
#include "core/utils/autoproperty.h"
#include "core/dal/predictionresult.h"
#include "core/dal/entity.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


class ChartData : public Entity
{
    Q_OBJECT
    ENTITY_UTILS(ChartData)

    AUTO_PROPERTY_DECL(PredictionResult*, popPred, PopPred)
    AUTO_PROPERTY_DECL(PredictionResult*, aprPred, AprPred)
    AUTO_PROPERTY_DECL(PredictionResult*, apoPred, ApoPred)
    AUTO_PROPERTY_DECL(PredictionResult*, revPred, RevPred)
    AUTO_PROPERTY_DECL(PredictionResult*, adjPred, AdjPred)

public:

    void setInfo(QString key, QString value) {
        info[key] = value;
    }

    Q_INVOKABLE QString getInfo(QString key) const {
        return info[key];
    }


public:
    explicit ChartData(AbstractRepository *repository,QObject *parent = nullptr);

    void initialize();

protected:

    QMap<QString, QString> info;

};

}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::ChartData*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::ChartData*>)

#endif // CHARTDATA_H
