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


#ifndef REFERENCES_H
#define REFERENCES_H

#include "core/dal/entity.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


class Reference : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(Reference)


    AUTO_PROPERTY_DECL(QString, text, Text)
    AUTO_PROPERTY_DECL(QString, type, Type)


    protected :
        Q_INVOKABLE Reference(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent) {}
        Q_INVOKABLE Reference(AbstractRepository *repository, QObject * parent, const QString &text, const QString &type)
            : Entity(repository, parent), _text(text), _type(type){}

};

POINTERLIST_CLASS_DECL(ReferenceList, Reference)

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::Reference*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::Reference*>)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::ReferenceList*)

#endif // REFERENCES_H
