#include "admin/src/dal/measure.h"
#include "core/dal/coremeasure.h"
#include "core/utils/units.h"
#include "core/dal/amount.h"

using namespace Tucuxi::Gui::Admin;

Measure::Measure(Tucuxi::Gui::Core::AbstractRepository *repository, QObject* parent)
    : Tucuxi::Gui::Core::CoreMeasure(repository, parent),
    _arrivalDate(QDateTime::currentDateTime())
{
    this->setId(-1);
//    this->setConcentration(Tucuxi::Gui::Core::Amount(0, Tucuxi::Gui::Core::Unit("mg/l")));

}

Measure::Measure(Tucuxi::Gui::Core::AbstractRepository *repository,
                 QDateTime moment,
                 Tucuxi::Gui::Core::IdentifiableAmount* amount,
                 QString drug,
                 int dbid,
                 QString sampleID,
                 QDateTime arrivalDate,
                 bool enable) :
    Tucuxi::Gui::Core::CoreMeasure(repository)
{
    this->setMoment(moment);
    this->setConcentration(amount);
    this->setSdrug(drug);
    this->sampleID(sampleID);
    this->arrivalDate(arrivalDate);
    this->setId(dbid);
    this->enable(enable);
}

//MeasureList::MeasureList(Tucuxi::Gui::Core::AbstractRepository* repository, QObject* parent) : Entity(repository, parent) {}
//void MeasureList::append(Measure* target) { _list.append(target); setModified(true); emit objListChanged(getObjList());}
//void MeasureList::add() { _list.append(Tucuxi::Gui::Core::CoreFactory::createEntity<Measure>(ABSTRACTREPO, this)); setModified(true); emit objListChanged(getObjList());}
//Measure *MeasureList::first() { return _list.first(); }
//Measure *MeasureList::last() { return _list.last(); }
//Measure *MeasureList::at(const int i) { return _list.at(i); }
//QList<Measure*>& MeasureList::getList() { return _list; }
