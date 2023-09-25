//@@license@@

#include <QString>

#include "core/utils/units.h"
#include "core/core.h"

namespace Tucuxi {
namespace Gui {
namespace Core {



//-------------------------- UNIT OBJECT -----------------------

//Default constructor
Unit::Unit()
{

}

//Copy constructor
Unit::Unit(const Unit &other) : _unit(other._unit)
{

}

//Constructor from a string
Unit::Unit(QString str)
{
    fromString(str);
}

//Sets the unit from a string
bool Unit::fromString(QString str)
{
    QString unitStr = str;

    //Clean the unit name
    unitStr = unitStr.simplified();
    unitStr.remove("[");
    unitStr.remove("]");
    unitStr = unitStr.toLower();

    _unit = Tucuxi::Common::TucuUnit(unitStr.toStdString());
    return Tucuxi::Common::UnitManager::isKnown(_unit);
}

//Returns the unit as a string surrounded with brackets, unless it is empty
QString Unit::toString() const
{
    QString n = name();

    if (n.isEmpty())
        return n;
    else
        return QString("[%1]").arg(n);
}

//Returns the unit as a string
QString Unit::name() const
{
    return QString::fromStdString(_unit.toString());
}

//Returns the unit validity
bool Unit::isValid() const
{
    return Tucuxi::Common::UnitManager::isKnown(_unit);
}

Tucuxi::Common::TucuUnit Unit::tucuUnit() const
{
    return _unit;
}

double Unit::convertToUnit(double value, const Unit& initialUnit, const Unit& finalUnit)
{
    return Tucuxi::Common::UnitManager::convertToUnit(value, initialUnit.tucuUnit(), finalUnit.tucuUnit());
}

bool Unit::isCompatible(const Unit &unit1, const Unit &unit2)
{
    return Tucuxi::Common::UnitManager::isCompatible(unit1.tucuUnit(), unit2.tucuUnit());
}

//Compare the two stored values
bool Unit::operator ==(const Unit &other) const
{
    return _unit == other._unit;
}

//Use the ==
bool Unit::operator !=(const Unit &other) const
{
    return !(*this == other);
}

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

