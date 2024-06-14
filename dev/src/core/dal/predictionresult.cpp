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


#include "core/dal/predictionresult.h"
#include "core/dal/entity.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


//AUTO_PROPERTY_IMPL(PredictionResult, ParameterType, paramsType, ParamsType)
Q_AUTO_PROPERTY_IMPL(PredictionResult, Predictive*, predictive, Predictive)
Q_AUTO_PROPERTY_IMPL(PredictionResult, Adjustments*, adjustments, Adjustments)
AUTO_PROPERTY_IMPL(PredictionResult, bool, isValidDomain, IsValidDomain)
AUTO_PROPERTY_IMPL(PredictionResult, QString, domainMessage, DomainMessage)

PredictionResult::PredictionResult(AbstractRepository *repository,QObject *parent)
    : Entity(repository, parent),
      _predictive(CoreFactory::createEntity<Predictive>(repository, this)),
      _adjustments(CoreFactory::createEntity<Adjustments>(repository, this)),
      _isValidDomain(true),
      _domainMessage(QString("Init"))
//      _paramsType(ParameterType::APRIORI)
{

}

}
}
}
