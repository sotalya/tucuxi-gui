//@@license@@

#include "core/dal/drug/target.h"
#include "core/dal/drug/operableamount.h"
#include "core/dal/drug/translatablestring.h"


#include <iostream>

namespace Tucuxi {
namespace Gui {
namespace Core {


QML_POINTERLIST_CLASS_IMPL(TargetMethodList, TargetMethod)

QML_POINTERLIST_CLASS_IMPL(TargetList, Target)


AUTO_PROPERTY_IMPL(Target, TargetMethod*, type, Type)
//! Minimum concentration value
AUTO_PROPERTY_IMPL(Target, OperableAmount*, cmin, Cmin)
//! Maximum concentration value
AUTO_PROPERTY_IMPL(Target, OperableAmount*, cmax, Cmax)
//! Optimal concentration value
AUTO_PROPERTY_IMPL(Target, OperableAmount*, cbest, Cbest)
//! Minimum time value
AUTO_PROPERTY_IMPL(Target, OperableAmount*, tmin, Tmin)
//! Maximum time value
AUTO_PROPERTY_IMPL(Target, OperableAmount*, tmax, Tmax)
//! Best time value
AUTO_PROPERTY_IMPL(Target, OperableAmount*, tbest, Tbest)
//! Minimum inhibitory concentration
AUTO_PROPERTY_IMPL(Target, OperableAmount*, mic, Mic)

AUTO_PROPERTY_IMPL(Target, TranslatableString*, comments, Comments)


Target::Target(AbstractRepository *repository, QObject *parent) : Entity(repository, parent),
    _type(CoreFactory::createEntity<TargetMethod>(repository, this)),
    _cmin(CoreFactory::createEntity<OperableAmount>(repository, this)),
    _cmax(CoreFactory::createEntity<OperableAmount>(repository, this)),
    _cbest(CoreFactory::createEntity<OperableAmount>(repository, this)),
    _tmin(CoreFactory::createEntity<OperableAmount>(repository, this)),
    _tmax(CoreFactory::createEntity<OperableAmount>(repository, this)),
    _tbest(CoreFactory::createEntity<OperableAmount>(repository, this)),
    _mic(CoreFactory::createEntity<OperableAmount>(repository, this)),
    _comments(CoreFactory::createEntity<TranslatableString>(repository, this))
{
          _cmin->setValue(0);
          _cmin->setUnit(Unit("mg/l"));
          _cmax->setValue(0);
          _cmax->setUnit(Unit("mg/l"));
          _cbest->setValue(0);
          _cbest->setUnit(Unit("mg/l"));
          _tmin->setValue(-1);
          _tmin->setUnit(Unit("h"));
          _tmax->setValue(-1);
          _tmax->setUnit(Unit("h"));
          _tbest->setValue(-1);
          _tbest->setUnit(Unit("h"));
          _mic->setValue(-1);
          _mic->setUnit(Unit("mg*h/l"));

}

/*
 * Target::Target(AbstractRepository *repository, Target* target, QObject *parent) : Entity(repository, parent),
    _cmin(CoreFactory::createEntity<OperableAmount>(repository, this)),
    _cmax(CoreFactory::createEntity<OperableAmount>(repository, this)),
    _cbest(CoreFactory::createEntity<OperableAmount>(repository, this)),
    _tmin(CoreFactory::createEntity<OperableAmount>(repository, this)),
    _tmax(CoreFactory::createEntity<OperableAmount>(repository, this)),
    _tbest(CoreFactory::createEntity<OperableAmount>(repository, this))
{
          _cmin->setValue(target->getCmin()->value());
          _cmin->setUnit(target->getCmin()->unit());
          _cmax->setValue(target->getCmax()->value());
          _cmax->setUnit(target->getCmax()->unit());
          _cbest->setValue(target->getCbest()->value());
          _cbest->setUnit(target->getCbest()->unit());
          _tmin->setValue(target->getTmin()->value());
          _tmin->setUnit(target->getTmin()->unit());
          _tmax->setValue(target->getTmax()->value());
          _tmax->setUnit(target->getTmax()->unit());
          _tbest->setValue(target->getTbest()->value());
          _tbest->setUnit(target->getTbest()->unit());

}
*/
void Target::copyFrom(Target *other)
{
    _cmin->setValue(other->getCmin()->value());
    _cmin->setUnit(other->getCmin()->unit());
    _cmax->setValue(other->getCmax()->value());
    _cmax->setUnit(other->getCmax()->unit());
    _cbest->setValue(other->getCbest()->value());
    _cbest->setUnit(other->getCbest()->unit());
    _tmin->setValue(other->getTmin()->value());
    _tmin->setUnit(other->getTmin()->unit());
    _tmax->setValue(other->getTmax()->value());
    _tmax->setUnit(other->getTmax()->unit());
    _tbest->setValue(other->getTbest()->value());
    _tbest->setUnit(other->getTbest()->unit());
    _mic->setValue(other->getMic()->value());
    _mic->setUnit(other->getMic()->unit());
    _type->setTargetType(other->getType()->getTargetType());


    _comments->clear();
    for(auto key : other->getComments()->keys()) {
        _comments->insert(key,other->getComments()->value(key));
    }

}

//bool Target::isValid()
//{
    // ToDo ///////////////////////////////////////////////////////////////////////////////////////
    // We also need to check the target type (using the definition manager) when the targets are
    // refactored.
    // ////////////////////////////////////////////////////////////////////////////////////////////

//    return _cmin->isValid() && _cmax->isValid() && _cbest->isValid();
//}

//QString Target::name(TargetType type)
//{
//    switch (type) {
//    case ResidualTarget:
//        return tr("Residual");
//    case PeakTarget:
//        return tr("Peak");
//    case MeanTarget:
//        return tr("Mean");
//    default:
//        return "Unknown type";
//    }
//}

//TargetSet::TargetSet(){}

//TargetSet::TargetSet(const TargetSet* other)
//{
//    foreach (SharedTarget _m, *other.data()) {
//        SharedTarget _nm = SharedTarget(new Target());
//        _nm->setAmount(_m->amount());
//        _nm->setArrivalDate(_m->arrivalDate());
//        _nm->setDrug(_m->drug());
//        _nm->setDrug_id(_m->drug_id());
//        _nm->setId(_m->id());
//        _nm->setMoment(_m->moment());
//        _nm->setPatient(_m->patient());
//        _nm->setPatient_id(_m->patient_id());
//        _nm->setSampleID(_m->sampleID());
//        _nm->setUnit(_m->unit());
//        _nm->setValue(_m->value());
//        this->append(_nm);
//    }
//}


const QMap<QString, TargetMethod::TargetType> Target::_typeMap = Target::createMap();


} // namespace Core
} // namespace Gui
} // namespace Tucuxi
