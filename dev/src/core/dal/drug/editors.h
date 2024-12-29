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


#ifndef EDITOR_H
#define EDITOR_H

#include "core/dal/entity.h"
#include "core/utils/ezutils.h"

#include "core/dal/drug/translatablestring.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


class Editor : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(Editor)

    AUTO_PROPERTY_DECL(QString,name,Name)
    AUTO_PROPERTY_DECL(QString, institution,Institution)
    AUTO_PROPERTY_DECL(QString, email, Email)
    AUTO_PROPERTY_DECL(QDateTime,date,Date)

    AUTO_PROPERTY_DECL(TranslatableString*, comments,Comments)


    protected :

    Q_INVOKABLE Editor(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent), _comments(nullptr) {}
    Q_INVOKABLE Editor(AbstractRepository *repository, QObject * parent, const QString &name, const QString &institution, const QString &email, const QDateTime &date,TranslatableString *comments)
        : Entity(repository, parent), _name(name), _institution(institution), _email(email), _date(date), _comments(comments) {}



};
/*
class EditorSet : public QList<Editor*>
{

    ENTITY_UTILS(EditorSet)

    EditorSet(AbstractRepository *repository,QObject* parent = nullptr){ }

    virtual ~EditorSet();



};
*/
QML_POINTERLIST_CLASS_DECL(EditorList,Editor)
} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::Editor*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::Editor*>)
// Q_DECLARE_METATYPE(Tucuxi::Gui::Core::EditorSet*)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::EditorList*)

#endif // EDITOR_H
