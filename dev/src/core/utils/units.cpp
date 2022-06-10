//@@license@@

#include <QString>

#include "core/utils/units.h"
#include "core/core.h"

namespace Tucuxi {
namespace GuiCore {


//-------------------------- UNIT OBJECT -----------------------

//Default constructor
Unit::Unit() : _base(invalid_unit_id), _multiplier(-1.0)
{

}

//Copy constructor
Unit::Unit(const Unit &other) : _base(other._base), _multiplier(other._multiplier)
{

}

//Constructor from a string
Unit::Unit(QString str) : _base(invalid_unit_id), _multiplier(-1.0)
{
    fromString(str);
}

//Sets the unit from a string
bool Unit::fromString(QString str)
{
    return CORE->unitsRegister()->unitForName(str, _base, _multiplier);
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
QString Unit::name () const
{
    return CORE->unitsRegister()->nameForUnit(_base, _multiplier);
}

//Returns the unit base name (without the multiplier)
QString Unit::base() const
{
    return CORE->unitsRegister()->nameForUnit(_base, 1);
}

//Returns the unit multiplier
double Unit::multiplier() const
{
    return _multiplier;
}

//Returns the unit validity
bool Unit::isValid() const
{
    return _base != invalid_unit_id && _multiplier != -1.0;
}

//Returns the multiplier between two units
double Unit::multiplier (const Unit &other) const
{
    if (_base != other._base)
        return -1.0;
    else
        return _multiplier / other._multiplier;
}

//Compare the two stored values
bool Unit::operator ==(const Unit &other) const
{
    return _base == other._base && _multiplier == other._multiplier;
}

//Use the ==
bool Unit::operator !=(const Unit &other) const
{
    return !(*this == other);
}


//-------------------------- UNITS REGISTER -----------------------

const char *const UnitsRegister::_invalid_unit_name = "Unknown unit";

//Create the char map and its multiplier value
UnitsRegister::UnitsRegister()
{
    _multipliers.insert('p', 0.000000000001);
    _multipliers.insert('n', 0.000000001);
    _multipliers.insert('u', 0.000001);
    _multipliers.insert('m', 0.001);
    _multipliers.insert('K', 1000);
    _multipliers.insert('M', 1000000);
    _multipliers.insert('G', 1000000000);
    _multipliers.insert('T', 1000000000000);
}

//Returns the name of a unit
QString UnitsRegister::nameForUnit(unit_id unit, double multiplier)
{
    //Get the unit base name
    QString name = _baseNames.value(unit, _invalid_unit_name);
    if (name == _invalid_unit_name || name.isEmpty())
        return name;

    //Get the unit multiplier
    QChar weight = _multipliers.key(multiplier);
    if (!weight.isNull())
        name.prepend(weight);

    return name;
}

//Returns the unit given its name
bool UnitsRegister::unitForName (QString name, unit_id &base, double &multiplier)
{
    //Check if valid
    if (name == _invalid_unit_name) {
        base = invalid_unit_id;
        multiplier = -1.0;

        return false;
    }

    //Clean the name
    name = name.simplified();
    name.remove("[");
    name.remove("]");

    //Set the multiplier
    if (name.size() > 1) {
        multiplier = _multipliers.value(name.at(0), 1.0);

        if (multiplier != 1.0)
            name.remove(0, 1);
    }
    else
        multiplier = 1.0;

    //Set the base ID
    base = _baseNames.indexOf(name);
    if (base == -1) {
        base = _baseNames.size();
        _baseNames.append(name);
    }

    return true;
}

} // namespace core
} // namespace Tucuxi

