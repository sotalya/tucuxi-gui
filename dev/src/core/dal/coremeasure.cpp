#include "core/dal/coremeasure.h"
#include "core/dal/uncastedvalue.h"

namespace ezechiel {
namespace core {

AUTO_PROPERTY_IMPL(CoreMeasure, IdentifiableAmount*, concentration, Concentration)
AUTO_PROPERTY_IMPL(CoreMeasure, QDateTime, moment, Moment)
AUTO_PROPERTY_IMPL(CoreMeasure, QString, sdrug, Sdrug)
AUTO_PROPERTY_IMPL(CoreMeasure, UncastedValueList*, uncastedValues, UncastedValues)


CoreMeasure::CoreMeasure(AbstractRepository *repository, QObject* parent) :
    Entity(repository, parent),
    _moment(QDateTime::currentDateTime()),
    _concentration(CoreFactory::createEntity<IdentifiableAmount>(repository, this)),
    _uncastedValues(CoreFactory::createEntity<UncastedValueList>(repository, this))
{
    this->setId(-1);
    _concentration->setValue(0);
    _concentration->setUnit(Unit("mg/l"));
}

CoreMeasure::CoreMeasure(AbstractRepository *repository, QDateTime moment, IdentifiableAmount* amount, QString drug, ident dbid) :
    Entity(repository),
    _moment(moment),
    _concentration(amount),
    _sdrug(drug)
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
} // namespace core
} // namespace ezechiel
