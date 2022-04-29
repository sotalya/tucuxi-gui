//@@license@@

#include <QStringList>

#include "core/dal/drug/parameters.h"
//#include "core/interfaces/drugmodelengine.h"
#include "core/dal/drug/bsv.h"
#include "core/dal/drug/errormodel.h"
#include "core/core.h"
#include "core/dal/drug/operation.h"
#include "core/dal/drug/translatablestring.h"

namespace ezechiel {
namespace core {


AUTO_PROPERTY_IMPL(Parameter, QString, name, Name)
AUTO_PROPERTY_IMPL(Parameter, OperableAmount*, quantity, Quantity)
AUTO_PROPERTY_IMPL(Parameter, Bsv*, bsv, Bsv)
//! Minimal value of this parameter
AUTO_PROPERTY_IMPL(Parameter, double, min, Min)
//! Maximal value of this parameter
AUTO_PROPERTY_IMPL(Parameter, double, max, Max)
//! Step between each value (for the UI)
AUTO_PROPERTY_IMPL(Parameter, double, step, Step)

AUTO_PROPERTY_IMPL(Parameter, TranslatableString*, comments, Comments)

// YTA: Not used yet, but I guess we should go this way
AUTO_PROPERTY_IMPL(Parameter, OperationList*, operations, Operations)


QString toString(ParameterType t)
{
    switch (t) {
    case POPULATION : return "Population";
    case APRIORI : return "Apriori";
    case APOSTERIORI : return "Aposteriori";
    case MANUAL : return "Manual";
    default : return "Should not be used";
    }
}


ParameterType fromString(QString s)
{
    if (s == "Population")
        return POPULATION;
    if (s == "Apriori")
        return APRIORI;
    if (s == "Aposteriori")
        return APOSTERIORI;
    if (s == "Manuel")
        return MANUAL;
    // ERROR
    return LAST_PT;
}



//--------------------- PARAMETER --------------------

Parameter::Parameter(AbstractRepository *repository, QObject* parent) :
    Entity(repository, parent),
    _name(""),
    _quantity(CoreFactory::createEntity<OperableAmount>(repository, this)),
    _bsv(CoreFactory::createEntity<Bsv>(repository,this)),
    _min(0),_max(0),_step(0),
    _comments(CoreFactory::createEntity<TranslatableString>(repository,this)),
    _operations(nullptr)
{    _quantity->setValue(0);
    _quantity->setUnit(Unit());
}

Parameter::Parameter(AbstractRepository *repository, QObject* parent, QString pid, double value, double standard, double proportional, /*distribution::DistributionType distribution,*/ double min, double max, double step, Unit unit) :
    Entity(repository, parent), _name(pid), _quantity(CoreFactory::createEntity<OperableAmount>(repository, this)),
    _min(min), _max(max), _step(step) {

    _quantity->setValue(value);
    _quantity->setUnit(unit);
    _bsv = CoreFactory::createEntity<Bsv>(repository,this);
    _bsv->setStandard(standard);
    _bsv->setProportional(proportional);
}

Parameter::Parameter(AbstractRepository *repository, QObject* parent, SharedParameter _p) : Entity(repository, parent) {

    _bsv = CoreFactory::createEntity<Bsv>(repository,this);

    setId(_p->id());
    setName(_p->getName());
    setMax(_p->getMax());
    setMin(_p->getMin());
    _bsv->setProportional(_p->getBsv()->getProportional());
    _bsv->setStandard(_p->getBsv()->getStandard());
    setStep(_p->getStep());
    _quantity->setUnit(_p->getQuantity()->unit().name());
    _quantity->setValue(_p->getQuantity()->value());
}

//Simple print
QString Parameter::toString() const
{
    return QString("PID : %1, Value = %2, Standard = %3, Proportional = %4, Min = %5, Max = %6, Step = %77, Units = %8")
            .arg(getName())
            .arg(getQuantity()->value())
            .arg(getBsv()->getStandard())
            .arg(getBsv()->getProportional())
            .arg(getMin())
            .arg(getMax())
            .arg(getStep())
            .arg(getQuantity()->unit().name());
}

bool Parameter::isVariable() const
{
    return (_bsv->getStandard() != 0.0) || (_bsv->getProportional() != 0.0);
}

double Parameter::variance() const
{
    if (_bsv->getStandard() != 0.0)
        return _bsv->getStandard()*_bsv->getStandard();
    if (_bsv->getProportional() != 0.0)
        return _bsv->getProportional()*_bsv->getProportional();
    return 0.0;
}

double Parameter::deviation() const
{
    if (_bsv->getStandard() != 0.0)
        return _bsv->getStandard();
    if (_bsv->getProportional() != 0.0)
        return _bsv->getProportional();
    return 0.0;
}

QML_POINTERLIST_CLASS_IMPL(Parameters, Parameter)
//--------------------- PARAMETERSET --------------------

AUTO_PROPERTY_IMPL(ParameterSet, QDateTime, time, Time)
AUTO_PROPERTY_IMPL(ParameterSet, Parameters*, parameters, Parameters)

ParameterSet::ParameterSet(AbstractRepository *repository, QObject* parent)
    : Entity(repository, parent),
      _parameters(CoreFactory::createEntity<Parameters>(repository, this))
{
//    _errorModel = CoreFactory::createEntity<ErrorModel>(0);
}

ParameterSet::ParameterSet(ParameterType type, const QString &model) :
    _type(type),
    _model(model),
    _parametersmap(*(new QMap<QString,SharedParameter>()))
{
//    _errorModel = CoreFactory::createEntity<ErrorModel>(0);
}

ParameterSet::ParameterSet(AbstractRepository *repository, QObject* parent, ParameterSet* other) :
    _type(other->_type),
//    _parameters(other->_parameters),
    _correlations(other->_correlations),
    _model(other->_model)
{
//    _errorModel = CoreFactory::createEntity<ErrorModel>(0);
//    _errorModel->setProportional(other->globalProportional());
//    _errorModel->setAdditive(other->globalAdditive());
    foreach (SharedParameter _p, other->_parametersmap) {
        // YTA : We should use a copy constructor here
        SharedParameter _np = ezechiel::core::CoreFactory::createEntity<Parameter>(repository);
        _np->setName(_p->getName());
        _np->setQuantity(ezechiel::core::CoreFactory::createEntity<OperableAmount>(repository, _np));
        _np->getQuantity()->setValue(_p->getQuantity()->value());
        _np->setId(_p->id());
        _np->setMax(_p->getMax());
        _np->setMin(_p->getMin());
        _np->getBsv()->setProportional(_p->getBsv()->getProportional());
        _np->getBsv()->setStandard(_p->getBsv()->getStandard());
        _parametersmap.insert(_np->getName(), _np);
    }
}

ParameterType ParameterSet::type() const
{
    return _type;
}

void ParameterSet::setType(ParameterType type)
{
    _type = type;
}

QStringList ParameterSet::pids() const
{
    return _parametersmap.keys();
}

bool ParameterSet::contains(const QString &pid) const
{
    return _parametersmap.contains(pid);
}

SharedParameter ParameterSet::get(const QString &pid) const
{
    return _parametersmap.value(pid);
}

QString ParameterSet::model() const
{
    return _model;
}

void ParameterSet::setModel(const QString &model)
{
    _model = model;
}

void ParameterSet::set(const SharedParameter &parameter)
{
    _parametersmap.insert(parameter->getName(),parameter);
    _parameters->getList() = _parametersmap.values();
}

void ParameterSet::copy(const ParameterSet* other)
{
    _parametersmap.clear();
    foreach (SharedParameter _p, other->_parametersmap) {
        SharedParameter _np = ezechiel::core::CoreFactory::createEntity<Parameter>(ABSTRACTREPO);
        _np->setName(_p->getName());
        _np->getQuantity()->setValue(_p->getQuantity()->value());
        _np->setId(_p->id());
        _np->setMax(_p->getMax());
        _np->setMin(_p->getMin());
        _np->getBsv()->setProportional(_p->getBsv()->getProportional());
        _np->getBsv()->setStandard(_p->getBsv()->getStandard());
        _parametersmap.insert(_np->getName(), _np);
    }
//    _parameters = other->_parameters;
    _correlations = other->_correlations;
//    _errorModel->setAdditive(other->globalAdditive());
//    _errorModel->setProportional(other->globalProportional());
    _model = other->_model;
    _parameters->getList() = _parametersmap.values();
}

void ParameterSet::clear()
{
    _parametersmap.clear();
    _parameters->clear();
}

int ParameterSet::size() const
{
    return _parametersmap.size();
}

//Adds a correlation to the parameters set
bool ParameterSet::setCorrelation(QString param1, QString param2, const double &value)
{
    if (param1.isEmpty() || param2.isEmpty())
        return false;

    if (param1 < param2)
        _correlations.insert(param1 + "__" + param2, value);
    else
        _correlations.insert(param2 + "__" + param1, value);

    return true;
}

//Returns a correlation's value, or 0 if it doesn't exist
double ParameterSet::correlation(QString param1, QString param2) const
{
    if (param1 < param2)
        return _correlations.value(param1 + "__" + param2, 0.0);
    else
        return _correlations.value(param2 + "__" + param1, 0.0);
}

void ParameterSet::copyCorrelationsFrom(ParameterSet &source)
{
    _correlations = source._correlations;
}

//Adds a correlation's comment set to the parameter set
bool ParameterSet::setCorrelationComments(QString param1, QString param2, TranslatableString *comments)
{
    if (param1.isEmpty() || param2.isEmpty())
        return false;

    if (param1 < param2)
        _correlationsComments.insert(param1 + "__" + param2, comments);
    else
        _correlationsComments.insert(param2 + "__" + param1, comments);

    // The comments are now children of the ParameterSet
    comments->setParent(this);

    return true;
}

//Returns a correlation's comment set
TranslatableString* ParameterSet::correlationComments(QString param1, QString param2) const
{
    if (param1 < param2)
        return _correlationsComments.value(param1 + "__" + param2);
    else
        return _correlationsComments.value(param2 + "__" + param1);
}



QStringList ParameterSet::correlationsKeys() const
{
    return _correlations.keys();
}
/*
void ParameterSet::setGlobal(double additive, double proportional)
{
    _errorModel->setAdditive(additive);
    _errorModel->setProportional(proportional);
}

double ParameterSet::globalAdditive() const
{
    return _errorModel->getAdditive();
}

double ParameterSet::globalProportional() const
{
    return _errorModel->getProportional();
}
*/
SharedParameter &ParameterSet::operator [](const QString &pid)
{
    return _parametersmap[pid];
}

const SharedParameter ParameterSet::operator [](const QString &pid) const
{
    return _parametersmap[pid];
}

} // namespace core
} // namespace ezechiel
