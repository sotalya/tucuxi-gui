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


#ifndef BSV_H
#define BSV_H

#include "core/dal/entity.h"

namespace Tucuxi {
namespace Gui {
namespace Core {



class Bsv : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(Bsv)

    //! The parameter's value standard variability
    AUTO_PROPERTY_DECL(double, standard, Standard)
    //! The parameter's value proportional variability
    AUTO_PROPERTY_DECL(double, proportional, Proportional)

protected:

    Q_INVOKABLE Bsv(AbstractRepository *repository, QObject *parent = nullptr) :
      Entity(repository, parent), _standard(0), _proportional(0) {}
    Q_INVOKABLE Bsv(AbstractRepository *repository, QObject * parent, double standard, double proportional)
        : Entity(repository, parent), _standard(standard), _proportional(proportional) {}
};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::Bsv*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::Bsv*>)

#endif // BSV_H
