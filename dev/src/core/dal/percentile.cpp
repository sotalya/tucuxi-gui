//@@license@@

#include "core/dal/percentile.h"

namespace Tucuxi {
namespace GuiCore {

//Percentile::Percentile(QObject *parent) : QObject(parent)
//{

//}

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

} // namespace core
} // namespace Tucuxi
