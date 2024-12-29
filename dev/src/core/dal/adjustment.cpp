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


#include "adjustment.h"
#include "dal/drug/target.h"

namespace Tucuxi {
namespace Gui {
namespace Core {

AUTO_PROPERTY_IMPL(TargetEvaluationResult, TargetMethod*, targetType, TargetType)
AUTO_PROPERTY_IMPL(TargetEvaluationResult, double, value, Value)
AUTO_PROPERTY_IMPL(TargetEvaluationResult, QString, unit, Unit)
AUTO_PROPERTY_IMPL(TargetEvaluationResult, double, score, Score)

TargetEvaluationResult::TargetEvaluationResult(AbstractRepository *repository, QObject *parent) :
    Entity(repository, parent),
    _targetType(nullptr),
    _value(0.0),
    _unit("-"),
    _score(-1.0)
{

}

QString TargetEvaluationResult::toHtml() const
{
    QString res;
    res += "Target type : " + _targetType->getLabel() + "\n";
    QString val;
    val.setNum(_value, 'f', 2);
    res += "Value : " + val + " " + _unit + "\n";
    val.setNum(_score, 'f', 2);
    res += "Score : " +val + "\n";
    return res;

}


QML_POINTERLIST_CLASS_IMPL(TargetEvaluationResults, TargetEvaluationResult)

QML_POINTERLIST_CLASS_IMPL(Adjustments, Adjustment)

AUTO_PROPERTY_IMPL(Adjustment, DosageHistory*, dosageHistory, DosageHistory)
AUTO_PROPERTY_IMPL(Adjustment, double, score, Score)
AUTO_PROPERTY_IMPL(Adjustment, TargetEvaluationResults*, targetEvaluationResults, TargetEvaluationResults)

Adjustment::Adjustment(AbstractRepository *repository, QObject *parent)
    : Predictive(repository, parent),
      _dosageHistory(CoreFactory::createEntity<DosageHistory>(repository, this)),
      _score(-1.0),
      _targetEvaluationResults(CoreFactory::createEntity<TargetEvaluationResults>(repository, this))
{

}

}
}
}

