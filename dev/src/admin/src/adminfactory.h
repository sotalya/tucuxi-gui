/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
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


#ifndef ADMINFACTORY_H
#define ADMINFACTORY_H

#include <QObject>
#include "core/interfaces/abstractrepository.h"

namespace Tucuxi {
namespace Gui {
namespace Admin {

class AdminFactory
{
public:
    AdminFactory();

    template<typename entity>
    static entity* createEntity(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent = nullptr)
    {
        return new entity(repository, parent);
    }

    template<typename entity>
    static entity* cloneEntity(Tucuxi::Gui::Core::AbstractRepository *repository, QObject* parent, entity* _target)
    {
        return new entity(repository, parent, _target);
    }

};

}
}
}

#endif // COREFACTORY_H
