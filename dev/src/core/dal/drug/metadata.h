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


#ifndef METADATA_H
#define METADATA_H

#include "core/dal/entity.h"

Q_MOC_INCLUDE("core/dal/drug/editors.h")
Q_MOC_INCLUDE("core/dal/drug/references.h")

namespace Tucuxi {
namespace Gui {
namespace Core {


class EditorList;
class ReferenceList;

class MetaData : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(MetaData)

    //first editor is always the creator
    AUTO_PROPERTY_DECL(EditorList*,editors,Editors)

    AUTO_PROPERTY_DECL(ReferenceList*,references,References)


    protected :
        Q_INVOKABLE MetaData(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent), _editors(0), _references(0) {}
    Q_INVOKABLE MetaData(AbstractRepository *repository, QObject * parent, EditorList *editors, ReferenceList *references)
        : Entity(repository, parent), _editors(editors), _references(references) {}


};


} //namespace Core
} // namespace Gui
} //namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::MetaData*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::MetaData*>)

#endif // METADATA_H
