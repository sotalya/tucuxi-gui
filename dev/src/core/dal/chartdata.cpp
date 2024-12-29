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


#include "core/dal/chartdata.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


Q_AUTO_PROPERTY_IMPL(ChartData, PredictionResult*, popPred, PopPred)
Q_AUTO_PROPERTY_IMPL(ChartData, PredictionResult*, aprPred, AprPred)
Q_AUTO_PROPERTY_IMPL(ChartData, PredictionResult*, apoPred, ApoPred)
Q_AUTO_PROPERTY_IMPL(ChartData, PredictionResult*, revPred, RevPred)
Q_AUTO_PROPERTY_IMPL(ChartData, PredictionResult*, adjPred, AdjPred)

ChartData::ChartData(AbstractRepository *repository,QObject *parent)
    : Entity(repository, parent),
      _popPred(CoreFactory::createEntity<PredictionResult>(repository, this)),
      _aprPred(CoreFactory::createEntity<PredictionResult>(repository, this)),
      _apoPred(CoreFactory::createEntity<PredictionResult>(repository, this)),
      _revPred(CoreFactory::createEntity<PredictionResult>(repository, this)),
      _adjPred(CoreFactory::createEntity<PredictionResult>(repository, this))
{

}

void ChartData::initialize()
{/*
    if (_popPred)
        _popPred->deleteLater();
    if (_aprPred)
        _aprPred->deleteLater();
    if (_apoPred)
        _apoPred->deleteLater();
    if (_revPred)
        _revPred->deleteLater();
    if (_adjPred)
        _adjPred->deleteLater();
*/
    _popPred = CoreFactory::createEntity<PredictionResult>(_repository, this);
    _aprPred = CoreFactory::createEntity<PredictionResult>(_repository, this);
    _apoPred = CoreFactory::createEntity<PredictionResult>(_repository, this);
    _revPred = CoreFactory::createEntity<PredictionResult>(_repository, this);
    _adjPred = CoreFactory::createEntity<PredictionResult>(_repository, this);

}

}
}
}
