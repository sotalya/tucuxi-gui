#include "admin/src/dal/measure.h"
#include "core/dal/coremeasure.h"
#include "core/utils/units.h"
#include "core/dal/amount.h"

Measure::Measure(Tucuxi::GuiCore::AbstractRepository *repository, QObject* parent)
    : Tucuxi::GuiCore::CoreMeasure(repository, parent),
    _arrivalDate(QDateTime::currentDateTime())
{
    this->setId(-1);
//    this->setConcentration(Tucuxi::GuiCore::Amount(0, Tucuxi::GuiCore::Unit("mg/l")));

}

Measure::Measure(Tucuxi::GuiCore::AbstractRepository *repository,
                 QDateTime moment,
                 Tucuxi::GuiCore::IdentifiableAmount* amount,
                 QString drug,
                 int dbid,
                 QString sampleID,
                 QDateTime arrivalDate) :
    Tucuxi::GuiCore::CoreMeasure(repository)
{
    this->setMoment(moment);
    this->setConcentration(amount);
    this->setSdrug(drug);
    this->sampleID(sampleID);
    this->arrivalDate(arrivalDate);
    this->setId(dbid);
}

//MeasureList::MeasureList(Tucuxi::GuiCore::AbstractRepository* repository, QObject* parent) : Entity(repository, parent) {}
//void MeasureList::append(Measure* target) { _list.append(target); setModified(true); emit objListChanged(getObjList());}
//void MeasureList::add() { _list.append(Tucuxi::GuiCore::CoreFactory::createEntity<Measure>(ABSTRACTREPO, this)); setModified(true); emit objListChanged(getObjList());}
//Measure *MeasureList::first() { return _list.first(); }
//Measure *MeasureList::last() { return _list.last(); }
//Measure *MeasureList::at(const int i) { return _list.at(i); }
//QList<Measure*>& MeasureList::getList() { return _list; }
