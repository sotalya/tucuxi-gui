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


#include "clinical.h"

#include "core/core.h"
#include "core/definitionmanager.h"

#include <QDebug>

using namespace Tucuxi::Gui::Admin;

AUTO_PROPERTY_IMPL(Clinical, QString, name, Name)
AUTO_PROPERTY_IMPL(Clinical, QDateTime, date, Date)
AUTO_PROPERTY_IMPL(Clinical, QString, value, Value)
AUTO_PROPERTY_IMPL(Clinical, QString, comment, Comment)
AUTO_PROPERTY_IMPL(Clinical, Patient*, patient, Patient)
AUTO_PROPERTY_IMPL(Clinical, int, patient_id, Patient_id)

Clinical::Clinical(Tucuxi::Gui::Core::AbstractRepository *repository, QObject* parent, const QString &name, const QDateTime &date) :
    Entity(repository, parent), _date(date), _name(name), _valid(false)
{
    setId(-1);
}

//Constructor
Clinical::Clinical(Tucuxi::Gui::Core::AbstractRepository *repository, QObject* parent, const QString& name, const QDateTime &date, QString value) :
    Entity(repository, parent), _value(value), _name(name), _date(date)
{
    //Check if the type is in the definitions
    _valid = CORE->definitionManager()->clinicalExists(name);
    setId(-1);
}

//Returns a string description of the clinical
QString Clinical::toString() const
{
    if (!isValid())
        return QString("Invalid clinical");

    QString clinicalType = _name;
    clinicalType[0] = clinicalType.at(0).toTitleCase();

    return QString("%1: %2").arg(clinicalType).arg(_value);
}

//Checks if the clinical is valid
bool Clinical::isValid() const
{
    return _valid;
}

//Assigns the value of the clinical
void Clinical::operator=(QString value)
{
    _value = value;
}

//Compares the clinical with another
bool Clinical::operator ==(Clinical* other) const
{
    return _id == other->id() &&
            _date == other->getDate() &&
            _value == other->getValue() &&
            _valid == other->isValid();
}

//Compares the clinical with another
bool Clinical::operator !=(Clinical* other) const
{
    return !(*this == other);
}

//----------------------------- CLINICAL SET -------------------------------

//Simple search
int ClinicalSet::find(const QString &name) const
{
    for (int i=0; i<this->size(); i++)
        if (this->at(i)->getName() == name)
            return i;
    return -1;
}

//Loop through the lists, and compare each element
bool ClinicalSet::operator ==(const ClinicalSet &other) const
{
    //Check the size to begin with
    if (this->size() != other.size())
        return false;

    //Compare each item
    for (int i=0; i < this->size(); i++)
        if (this->at(i) != other.at(i))
            return false;

    return true;
}
