/*
 * Tucuxi - Tucuxi-gui software.
 * This software is able to perform prediction of drug concentration in blood
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV.
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


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
    this->setAnalyteId(drug);
    this->sampleID(sampleID);
    this->arrivalDate(arrivalDate);
    this->setId(dbid);
    this->setEnable(enable);
}

//MeasureList::MeasureList(Tucuxi::Gui::Core::AbstractRepository* repository, QObject* parent) : Entity(repository, parent) {}
//void MeasureList::append(Measure* target) { _list.append(target); setModified(true); emit objListChanged(getObjList());}
//void MeasureList::add() { _list.append(Tucuxi::Gui::Core::CoreFactory::createEntity<Measure>(ABSTRACTREPO, this)); setModified(true); emit objListChanged(getObjList());}
//Measure *MeasureList::first() { return _list.first(); }
//Measure *MeasureList::last() { return _list.last(); }
//Measure *MeasureList::at(const int i) { return _list.at(i); }
//QList<Measure*>& MeasureList::getList() { return _list; }
