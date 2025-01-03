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


#ifndef COREFACTORY_H
#define COREFACTORY_H

#include <QObject>
#include "core/interfaces/abstractrepository.h"

namespace Tucuxi {
namespace Gui {
namespace Core {

class CoreFactory;


class CoreFactory
{
public:
    CoreFactory();

    static CoreFactory *_instance;

    static CoreFactory *getInstance()
    {
        if (!_instance)
            _instance = new CoreFactory();
        return _instance;
    }

    static void setInstance(CoreFactory *factory)
    {
        if (_instance)
            delete _instance;
        _instance = factory;
    }

    template<typename entity>
    entity* instanceCreateEntity(AbstractRepository *repository, QObject *parent = nullptr)
    {
        return new entity(repository, parent);
    }
/*
    template<>
    static Drug* createEntity<Drug>(AbstractRepository *repository, QObject *parent = nullptr)
    {
        return new Drug(repository, parent);
    }
*/
    template<typename entity>
    entity* instanceCloneEntity(AbstractRepository *repository, QObject* parent, entity* _target)
    {
        return new entity(repository, parent, _target);
    }


    template<typename entity>
    static entity* createEntity(AbstractRepository *repository, QObject *parent = nullptr)
    {
        return entity::createEntity(repository, parent);

//        return getInstance()->instanceCreateEntity<entity>(repository, parent);
//        return new entity(repository, parent);
    }

    template<typename entity>
    static entity* cloneEntity(AbstractRepository *repository, QObject* parent, entity* _target)
    {
        return getInstance()->instanceCloneEntity<entity>(repository, parent, _target);
//        return new entity(repository, parent, _target);
    }

};

/*
 *
class CoreFactory
{
public:
    CoreFactory();

    static CoreFactory *_instance;

    static CoreFactory *getInstance()
    {
        if (!_instance)
            _instance = new CoreFactory();
        return _instance;
    }

    static void setInstance(CoreFactory *factory)
    {
        if (_instance)
            delete _instance;
        _instance = factory;
    }

    template<typename entity>
    entity* instanceCreateEntity(AbstractRepository *repository, QObject *parent = nullptr)
    {
        return new entity(repository, parent);
    }

    template<typename entity>
    entity* instanceCloneEntity(AbstractRepository *repository, QObject* parent, entity* _target)
    {
        return new entity(repository, parent, _target);
    }


    template<typename entity>
    static entity* createEntity(AbstractRepository *repository, QObject *parent = nullptr)
    {
        return getInstance()->instanceCreateEntity<entity>(repository, parent);
//        return new entity(repository, parent);
    }

    template<typename entity>
    static entity* cloneEntity(AbstractRepository *repository, QObject* parent, entity* _target)
    {
        return getInstance()->instanceCloneEntity<entity>(repository, parent, _target);
//        return new entity(repository, parent, _target);
    }

};
*/

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

#endif // COREFACTORY_H
