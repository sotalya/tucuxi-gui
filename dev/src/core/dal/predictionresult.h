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


#ifndef PREDICTIONRESULT_H
#define PREDICTIONRESULT_H

#include <QObject>
#include "core/dal/drug/parameters.h"
#include "predictive.h"
#include "adjustment.h"
#include "core/dal/entity.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


class PredictionResult : public Entity
{
    Q_OBJECT
    ENTITY_UTILS(PredictionResult)
//    AUTO_PROPERTY_DECL(ParameterType, paramsType, ParamsType)
    AUTO_PROPERTY_DECL(Predictive*, predictive, Predictive)
    AUTO_PROPERTY_DECL(Adjustments*, adjustments, Adjustments)
    AUTO_PROPERTY_DECL(bool, isValidDomain, IsValidDomain)
    AUTO_PROPERTY_DECL(QString, domainMessage, DomainMessage)
    public:
    explicit PredictionResult(AbstractRepository *repository,QObject *parent = nullptr);

};

}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::PredictionResult*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::PredictionResult*>)
#endif // PREDICTIONRESULT_H
