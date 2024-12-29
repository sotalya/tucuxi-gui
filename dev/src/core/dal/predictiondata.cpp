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


#include "predictiondata.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


VALUELIST_CLASS_IMPL(Points, Point)

AUTO_PROPERTY_IMPL(FancyPoint, double, time, Time)
AUTO_PROPERTY_IMPL(FancyPoint, double, value, Value)
Q_AUTO_PROPERTY_IMPL(FancyPoint, ParameterSet*, pset, Pset)

FancyPoint::FancyPoint(AbstractRepository * repository, QObject * parent)
    : Entity(repository, parent),
      _pset(CoreFactory::createEntity<ParameterSet>(repository, this)),
      _time(0),
      _value(0)
{
}

QML_POINTERLIST_CLASS_IMPL(FancyPoints, FancyPoint)
Q_AUTO_PROPERTY_IMPL(PredictionData, FancyPoints*, points, Points)

PredictionData::PredictionData(AbstractRepository *repository,QObject *parent)
    : Entity(repository, parent),
      _points(CoreFactory::createEntity<FancyPoints>(repository, this))
{
    connect(this, SIGNAL(pointsChanged(FancyPoints*)), this, SLOT(findLoci(FancyPoints*)));
    connect(_points, SIGNAL(FancyPointChanged(FancyPoint&)), this, SLOT(findLociAtPoint(FancyPoint&)));
}

void PredictionData::findLoci(FancyPoints* pts){
    _peaks.clear();
    _troughs.clear();
    QList<FancyPoint*> lpts = pts->getList();
    if (lpts.empty()) return;
    auto pit_begin = lpts.begin();
    auto pit_end = lpts.end();
    auto pit_prev = pit_begin;
    auto pit_next = pit_begin;
    auto pit = pit_begin;
    pit++;
    pit_next++;
    pit_next++;
    _troughs.append(pit - pit_begin);
    for (; pit_next != pit_end; ++pit_prev, ++pit, ++pit_next) {
        double a = (*pit_prev)->getValue();
        double b = (*pit)->getValue();
        double c = (*pit_next)->getValue();
        if ((*pit_prev)->getValue() < (*pit)->getValue() && (*pit)->getValue() > (*pit_next)->getValue()) {
            _peaks.append(pit - pit_begin);
        } else {
            if ((*pit_prev)->getValue() >= (*pit)->getValue() && (*pit)->getValue() < (*pit_next)->getValue()) {
                _troughs.append(pit - pit_begin);
            }
        }
    }
    emit troughsChanged(_troughs);
    emit peaksChanged(_peaks);
}

void PredictionData::findLociAtPoint(FancyPoint& pt) {
    QList<FancyPoint*> lpts = _points->getList();
    if (lpts.empty()) return;
    auto pit = lpts.end()--;
    auto pit_prev = pit;
    auto pit_begin = lpts.begin();
    pit_prev--;
    if ((*pit_prev)->getValue() < (*pit)->getValue() && pt.getValue() < (*pit)->getValue()) {
        _peaks.append(pit - pit_begin);
        emit peaksChanged(_peaks);
    } else if ((*pit_prev)->getValue() > (*pit)->getValue() && pt.getValue() > (*pit)->getValue()) {
        _troughs.append(pit - pit_begin);
        emit troughsChanged(_troughs);
    }
}

}
}
}
