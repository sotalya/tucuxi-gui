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


#include "core/dal/amount.h"


namespace Tucuxi {
namespace Gui {
namespace Core {


//Amount::Amount(QObject *parent) : QObject(parent)
//{

//}

Amount::Amount () : _value(-1.0)
{

}

Amount::Amount(Unit unit) : _value(-1.0), _unit(unit)
{

}

Amount::Amount(double value, Unit unit) : _value(value), _unit(unit)
{

}

//Use fromString
Amount::Amount(const QString &str)
{
    fromString(str);
}

//Convert the amount unit
void Amount::convert(const Unit &unit)
{
    double newValue = Unit::convertToUnit(value(), this->unit(), unit);
    this->setValue(newValue);
    this->setUnit(unit);
}

//Returns true if the amount is valid
bool Amount::isValid()
{
    return value() != -1.0 && unit().isValid();
}

//Use the multiplier between these two units
double Amount::valueIn(const Unit &unit) const
{
    return Unit::convertToUnit(value(), this->unit(), unit);
}

//If the unit is not here, only print the value
QString Amount::toString() const
{
    QString u = unit().toString();
    if (u.isEmpty())
        return QString::number(value());
    else
        return QString("%1 %2").arg(value()).arg(u);
}

QString Amount::toString(int precision)
{
    QString unitStr = unit().toString();
    if (unitStr.isEmpty())
        return QString::number(value(), 'f', precision);
    else
        return QString("%1 %2").arg(QString::number(value(), 'f', precision), unitStr);
}

//Parse the number value, and give the rest to the Unit
bool Amount::fromString(const QString &str)
{
    bool ok = true;

    //Remove any spaces
    QString full = str;
    full = full.remove(" ");

    //Create the regex and apply it
    QRegularExpression reg("^([\\-]?[0-9\\.]+)(.*)");
    QRegularExpressionMatch match = reg.match(full);

    //Fetch the number
    this->setValue(match.captured(1).toDouble(&ok));
    if (!ok)
        return false;

    //Parse the unit
    return _unit.fromString(match.captured(2));
}

bool Amount::operator ==(const Amount &other) const
{
    return (value() == other.value()) && (unit() == other.unit());
}

bool Amount::operator !=(const Amount &other) const
{
    return !(*this==other);
}

Amount Amount::operator +(const Amount &other) const
{
    return Amount( other.valueIn(unit()) + value(), unit());
}

void Amount::operator +=(const Amount &other)
{
    setValue(value() + other.valueIn(unit()));
}

QString Amount::operator +(const QString &str) const
{
    return this->toString() + str;
}

Amount Amount::operator *(double d) const
{
    return Amount( d * value(), unit());
}

Amount Amount::operator *(const Amount &other) const
{
    return Amount( other.valueIn(unit()) * value(), unit());
}

Amount Amount::operator /(double d) const
{
    return Amount( value() / d, unit());
}

Amount Amount::operator /(const Amount &other) const
{
    return Amount( value() / other.valueIn(unit()), unit());
}

Amount Amount::operator -(const Amount &other) const
{
    return Amount( value() - other.valueIn(unit()), unit());
}

void Amount::operator -=(const Amount &other)
{
    setValue(value() - other.valueIn(unit()));
}

bool Amount::operator <(const Amount &other) const
{
    return value() < other.valueIn(unit());
}

bool Amount::operator >(const Amount &other) const
{
    return value() > other.valueIn(unit());
}

bool Amount::operator >=(const Amount &other) const
{
    return value() >= other.valueIn(unit());
}

bool Amount::operator <=(const Amount &other) const
{
    return value() <= other.valueIn(unit());
}

} // namespace Core
} // namespace Gui
} // namespace Tucuxi
