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


#include "core/dal/percentile.h"

namespace Tucuxi {
namespace Gui {
namespace Core {

Percentile::Percentile(int low, int high) : _low(low), _high(high)
{

}

int Percentile::low() const
{
    return _low;
}

int Percentile::high() const
{
    return _high;
}

void Percentile::setLow(int value)
{
    _low = value;
}

void Percentile::setHigh(int value)
{
    _high = value;
}

//----------------------- PERCENTILES --------------------------

PercentileList::PercentileList() : _type(POPULATION)
{

}

PercentileList::PercentileList(const QList<Percentile> &other) : QList<Percentile>(other), _type(POPULATION)
{

}

PercentileList::PercentileList(const PercentileList &other) : QList<Percentile>(other), _type(other._type)
{

}

PercentileList::PercentileList(ParameterType type) : _type(type)
{

}

ParameterType PercentileList::type() const
{
    return _type;
}

void PercentileList::setType(ParameterType type)
{
    _type = type;
}

} // namespace Core
} // namespace Gui
} // namespace Tucuxi
