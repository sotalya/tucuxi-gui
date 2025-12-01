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


#include "admin/src/dal/practician.h"
#include "core/interfaces/abstractrepository.h"
#include "../adminfactory.h"
#include "core/settings.h"

using namespace Tucuxi::Gui::Admin;

Practician::Practician(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent) :
    Entity(repository,parent),
    _externalId(),
    _title(),
    _role(),
    _person_id(-1),
    _institute_id(-1)
{
    _institute = AdminFactory::createEntity<Institute>(repository);
    _person = AdminFactory::createEntity<Person>(repository);
}

Practician::Practician(Tucuxi::Gui::Core::AbstractRepository *repository, const int &id, QObject *parent) :
    Entity(repository,parent),
    _externalId(),
    _title(),
    _role(),
    _person_id(-1),
    _institute_id(-1)
{
    auto _ins = AdminFactory::createEntity<Institute>(repository);
    institute(_ins);
    auto _pers = AdminFactory::createEntity<Person>(repository);
    person(_pers);
}

/*
void Practician::copyTo(Practician *other)
{
    other->externalId(_externalId);
    other->title(_title);
    other->role(_role);
    other->person_id(_person_id);
    other->institute_id(_institute_id);
    _person->copyTo(other->person());
}
*/

PracticianSet::PracticianSet(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent, const PracticianSet* &other)
{
    foreach (SharedPractician _p, *other) {
        auto _np = SharedPractician(new Practician(repository, parent));
        _np->setId(_p->id());
        _np->role(_p->role());
        _np->person_id(_p->person_id());
        _np->institute_id(_p->institute_id());
        _np->externalId(_p->externalId());
        _np->title(_p->title());
        this->append(_np);
    }
}
