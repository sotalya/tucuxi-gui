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


#include "partialrequest.h"
#include "../adminfactory.h"
#include "core/corefactory.h"
#include "core/dal/drug/drug.h"

using namespace Tucuxi::Gui::Admin;

PartialRequest::PartialRequest(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent) :
    Entity(repository, parent)
{
    setId(-1);
    _patient = AdminFactory::createEntity<Patient>(repository);
    _practician = AdminFactory::createEntity<Practician>(repository);
    _institute = AdminFactory::createEntity<Institute>(repository);
    _sample = AdminFactory::createEntity<Measure>(repository);
}

PartialRequest::PartialRequest(Tucuxi::Gui::Core::AbstractRepository *repository, const int &id, QObject *parent) :
    Entity(repository, id, parent)
{
    setId(id);
}

PartialRequestSet::PartialRequestSet(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent, const PartialRequestSet* &other)
{
    foreach (SharedPartialRequest _pr, *other) {
        auto _npr = SharedPartialRequest(new PartialRequest(repository, parent));
        _npr->requestId(_pr->requestId());
        _npr->requestCpcl(_pr->requestCpcl());
        _npr->patient(_pr->patient());
        _npr->practician(_pr->practician());
        _npr->institute(_pr->institute());
        _npr->drug(_pr->drug());
        _npr->sample(_pr->sample());
        this->append(_npr);
    }
}
