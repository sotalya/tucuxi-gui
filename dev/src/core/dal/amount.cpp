//@@license@@

#include "core/dal/amount.h"


namespace Tucuxi {
namespace GuiCore {

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
    Q_ASSERT(this->unit().base() == unit.base());

    //Check if the unit is different
    if (this->unit() == unit)
        return;

    //Check the units multipliers
    if (this->unit().multiplier() == unit.multiplier())
        return;

    //Convert the amount value
    this->setValue(value() * this->unit().multiplier() / unit.multiplier());

    //Replace the amount unit
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
    // ToDo /////////////////////////////////////////////////////////////
    // The calls to valueIn() in the computation classes (non-GUI classes
    // must be replaced by value() in order to always compute data using
    // the drug's default unit. When done, uncomment the line below and
    // use this function to convert units in the GUI classes for ease of
    // use and preferences management.
    // //////////////////////////////////////////////////////////////////

    Q_UNUSED(unit);

    //if (unit != this->unit)
    //    return this->unit.multiplier(unit) * value;
    //else
        return value();
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
    QRegExp reg("^([\\-]?[0-9\\.]+)(.*)");
    reg.indexIn(full);

    //Fetch the number
    this->setValue(reg.cap(1).toDouble(&ok));
    if (!ok)
        return false;

    //Parse the unit
    return _unit.fromString(reg.cap(2));
//    return this->unit().fromString(reg.cap(2));
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

} // namespace core
} // namespace Tucuxi
