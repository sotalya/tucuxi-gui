//@@license@@

#include <QDebug>

#include "core/dal/dosage.h"
#include "core/dal/prediction.h"
#include "admin/src/stdadminrepository.h"
#include "core/dal/uncastedvalue.h"


namespace ezechiel {
namespace core {

//AUTO_PROPERTY_IMPL(Dosage, ADME::Route, route, Route)
AUTO_PROPERTY_IMPL(Dosage, IdentifiableAmount*, quantity, Quantity)
AUTO_PROPERTY_IMPL(Dosage, double, stepQuantity, StepQuantity)
AUTO_PROPERTY_IMPL(Dosage, QDateTime, applied, Applied)
AUTO_PROPERTY_IMPL(Dosage, QDateTime, endtime, EndTime)
AUTO_PROPERTY_IMPL(Dosage, Duration, tinf, Tinf)
AUTO_PROPERTY_IMPL(Dosage, Duration, interval, Interval)
AUTO_PROPERTY_IMPL(Dosage, bool, hasEndDate, HasEndDate)
AUTO_PROPERTY_IMPL(Dosage, bool, isAtSteadyState, IsAtSteadyState)
AUTO_PROPERTY_IMPL(Dosage, QDateTime, steadyStateLastDoseDate, SteadyStateLastDoseDate)
AUTO_PROPERTY_IMPL(Dosage, UncastedValueList*, uncastedValues, UncastedValues)

Dosage::Dosage(AbstractRepository *repository, QObject* parent)
    : Entity(repository, parent), _interval(Duration(24,0,0,0)), _tinf(Duration()),
      _route(CoreFactory::createEntity<Admin>(repository, this)),
      _quantity(CoreFactory::createEntity<IdentifiableAmount>(repository, this)),
      _applied(QDateTime(QDate::currentDate(), QTime(8, 0))),
      _endtime(QDateTime(QDate::currentDate(), QTime(8, 0)).addDays(1)),
      _hasEndDate(true),
      _isAtSteadyState(true),
      _steadyStateLastDoseDate(QDateTime(QDate::currentDate(), QTime(8, 0))),
      _uncastedValues(CoreFactory::createEntity<UncastedValueList>(repository, this))
{
    _route->setRoute(Admin::EXTRA);
    _quantity->setValue(0.5);
    _quantity->setUnit(Unit("mg"));
    setId(-1);
}

Dosage::Dosage(AbstractRepository *repository, QObject *parent, Dosage* &other)
   : Entity(repository, parent), _quantity(other->getQuantity()),
      _route(CoreFactory::cloneEntity<Admin>(repository, this, other->getRoute())),
     _applied(other->getApplied()),
     _endtime(other->getEndTime()),
     _interval(other->getInterval()),
     _tinf(other->getTinf()),
     _hasEndDate(other->getHasEndDate()),
     _isAtSteadyState(other->getIsAtSteadyState()),
     _steadyStateLastDoseDate(other->getSteadyStateLastDoseDate()),
     _uncastedValues(CoreFactory::createEntity<UncastedValueList>(repository, this))
{
}


Admin* Dosage::getRoute() const
{
    return _route;
}

void Dosage::setRoute(Admin* route)
{
    if (_route != route) {
        _route = route;
        emit routeChanged(_route);
    }
}

double Dosage::getDbinterval() const
{
    return _interval.toHours();
}

void Dosage::setDbinterval(double dbinterval)
{
    if (_interval != Duration(dbinterval)) {

        _interval = Duration(dbinterval);
        emit intervalChanged(dbinterval);
        emit dbintervalChanged(dbinterval);
    }
}

double Dosage::getDbtinf() const
{
    return _tinf.toMinutes();
}

void Dosage::setDbtinf(double dbtinf)
{
    if (_tinf != Duration(0,dbtinf)) {
        _tinf = Duration(0, dbtinf);
        emit dbtinfChanged(dbtinf);
    }
}

void Dosage::convert(const Unit &unit)
{
    _quantity->convert(unit);
}

//bool Dosage::isValid()
//{
//    return _quantity->isValid() && getApplied().isValid() && !getInterval().isEmpty();
//}

bool Dosage::operator ==(Dosage* &other)
{
    return (_quantity == other->getQuantity() &&
            getInterval() == other->getInterval() &&
            getApplied() == other->getApplied() &&
            getTinf() == other->getTinf()
            );
}

bool Dosage::operator !=(Dosage* &other)
{
    return !(*this==other);
}

QString Dosage::toString()
{
    return QString::number(_quantity->value()) + " / " + getInterval().toString();
}

bool Dosage::getNextIntake(const QDateTime& fromTime, QDateTime &nextTime)
{
    QDateTime time = getApplied();
    qint64 dT = time.secsTo(fromTime);
    int nIntervals = dT / getInterval().toSeconds();
    nextTime = time.addSecs((nIntervals+1)*getInterval().toSeconds());
    return true;
}

AUTO_PROPERTY_IMPL(AdjustmentDosage, QString, type, Type)

AdjustmentDosage::AdjustmentDosage(AbstractRepository *repository, QObject* parent)
    : Dosage(repository, parent)
{
}

AdjustmentDosage::AdjustmentDosage(AbstractRepository *repository, QObject * parent, Dosage*& other)
    : Dosage(repository, parent, other)
{
}

DosageHistory::DosageHistory(AbstractRepository *repository, QObject* parent) : Entity(repository, parent) {

}

DosageHistory::DosageHistory(AbstractRepository *repository, QObject* parent, const DosageHistory* &other) {
}

//Returns the dosage at the given time
Dosage* DosageHistory::dosage(const Duration &duration)
{
    foreach (Dosage* _d, getList()) {
        if (firsttake().secsTo(_d->getApplied()) == duration.seconds()) {
            return _d;
        }
    }
    return 0;
}

void DosageHistory::setList(QList<Dosage*> data)
{
    _list = data; setModified(true);
}

QList<Dosage*>& DosageHistory::getList()
{
    return _list;
}

Dosage* DosageHistory::at(int i) const
{
    return _list.at(i);
}

void DosageHistory::add()
{
    _list.append(CoreFactory::createEntity<Dosage>(ABSTRACTREPO, this));
    update();
}

void DosageHistory::remove(const int i)
{
   _list.removeAt(i);
   update();
}

QObject* DosageHistory::objat(int i) const { return _list.at(i);}
Dosage* DosageHistory::first() const { return _list.first();}

void DosageHistory::update()
{
    emit objListChanged(&getObjList());
}

void DosageHistory::clear()
{
    _list.clear();
    setModified(true);
    update();
}

void DosageHistory::append(Dosage* target)
{
    _list.append(target);
    setModified(true);
    update();
}

int DosageHistory::size() const
{
    return _list.size();
}

Dosage* DosageHistory::last() const
{
    return _list.last();
}

bool DosageHistory::isEmpty() const
{
    return _list.isEmpty();
}

bool DosageHistory::isValid()
{
    if (isEmpty()) {
        return false;
    }
    for (int i = 0; i < size(); ++i) {
        if (!at(i)->isValid()) {
            return false;
        }
    }
    return true;
}

//Returns the dosage at the given date
Dosage* DosageHistory::dosage(const QDateTime &date)
{
    if (getList().empty()) {
        return 0;
    }

    //Get the duration from the first take
    Duration duration(0, 0, 0, firsttake().msecsTo(date));

    //Return the dosage at the exact duration
    return dosage(duration);
}

DosageHistory* DosageHistory::clone()
{
    DosageHistory* ret = CoreFactory::createEntity<DosageHistory>(ABSTRACTREPO);
    for (QList<Dosage*>::iterator it = getList().begin(); it != getList().end(); ++it) {
        Dosage* ndosage = CoreFactory::cloneEntity<Dosage>(ABSTRACTREPO, ret, *it);
        ret->getList().append(ndosage);
    }
    return ret;
}

QDateTime DosageHistory::endTime()
{
    if (getList().size() == 0) {
        return QDateTime();
    }
    QDateTime end = getList().at(0)->getEndTime();
    for (QList<Dosage*>::iterator it = getList().begin(); it != getList().end(); ++it) {
        if ((*it)->getEndTime() >= end) {
            end = (*it)->getEndTime();
        }
    }
    return end;
}

//Returns the last dosage
Dosage* DosageHistory::lastDosage() const
{
    if (size() > 0) {
        return this->last();
    }
    return nullptr;
}

bool DosageHistory::getNextIntake(const QDateTime& fromTime, QDateTime &nextTime)
{
    bool found = false;
    Dosage* nextDosage = NULL;
    foreach (Dosage* dosage, getList())
    {
        // Look for the closest next intake from all dosages active at the given "fromTime"
        if (dosage->getApplied() <= fromTime &&
            dosage->getEndTime() >= fromTime)
        {
            QDateTime t;
            if (dosage->getNextIntake(fromTime, t))
            {
                if (!found || nextTime > t) nextTime = t;
                found = true;
            }
        }
        else
        {
            // Find the first dosage after "fromTime"
            if (dosage->getApplied() > fromTime &&
                (nextDosage == NULL || dosage->getApplied() < nextDosage->getApplied()))
            {
                nextDosage = dosage;
            }
        }
    }

    // Use the next dosage if there's no active at the given "fromTime"
    if (!found)
    {
        if (nextDosage == NULL) return false;
        nextTime = nextDosage->getApplied();
    }

    return true;
}

} // namespace core
} // namespace ezechiel
