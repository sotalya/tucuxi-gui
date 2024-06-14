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


#include "core/dal/coremeasure.h"
#include "core/dal/uncastedvalue.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


AUTO_PROPERTY_IMPL(CoreMeasure, IdentifiableAmount*, concentration, Concentration)
AUTO_PROPERTY_IMPL(CoreMeasure, QDateTime, moment, Moment)
AUTO_PROPERTY_IMPL(CoreMeasure, QString, sdrug, Sdrug)
AUTO_PROPERTY_IMPL(CoreMeasure, UncastedValueList*, uncastedValues, UncastedValues)
AUTO_PROPERTY_IMPL(CoreMeasure, bool, enable, Enable)


CoreMeasure::CoreMeasure(AbstractRepository *repository, QObject* parent) :
    Entity(repository, parent),
    _concentration(CoreFactory::createEntity<IdentifiableAmount>(repository, this)),
    _moment(QDateTime::currentDateTime()),
    _enable(true),
    _uncastedValues(CoreFactory::createEntity<UncastedValueList>(repository, this))
{
    this->setId(-1);
    _concentration->setValue(0);
    _concentration->setUnit(Unit("mg/l"));
}

CoreMeasure::CoreMeasure(AbstractRepository *repository, QDateTime moment, IdentifiableAmount* amount, QString drug, bool enable, ident dbid) :
    Entity(repository),
    _concentration(amount),
    _moment(moment),
    _sdrug(drug),
    _enable(enable)
{
    this->setId(dbid);
}

void CoreMeasure::convert(const Unit &unit)
{
    getConcentration()->convert(unit);
}

//bool CoreMeasure::isValid()
//{
//    return _moment.isValid() && getConcentration()->isValid() && !getSdrug().isEmpty();
//}

bool CoreMeasure::operator ==(CoreMeasure* other)
{
    return (_moment == other->getMoment() &&
            getConcentration() == other->getConcentration() &&
            getSdrug() == other->getSdrug()
            );
}

bool CoreMeasure::operator !=(CoreMeasure* other)
{
    return !(*this==other);
}

//CoreMeasureSet::CoreMeasureSet(AbstractRepository *repository, QObject* parent, const CoreMeasureSet*& other)
//{
//    foreach (CoreMeasure* _m, *other) {
//        CoreMeasure* _nm = CoreFactory::createEntity<CoreMeasure>(repository, parent);
//        _nm->setConcentration(_m->getConcentration());
//        _nm->setId(_m->id());
//        _nm->setMoment(_m->getMoment());
//        this->append(_nm);
//    }
//}

QML_POINTERLIST_CLASS_IMPL(CoreMeasureList, CoreMeasure)
} // namespace Core
} // namespace Gui
} // namespace Tucuxi
