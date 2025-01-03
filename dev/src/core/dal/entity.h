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


#ifndef ENTITY_H
#define ENTITY_H

#include <QObject>
#include <QVariant>
#include <QMetaObject>
#include <QMetaProperty>
#include <QVector>

#include "core/interfaces/abstractrepository.h"

#include "factory.h"
#include "core/corefactory.h"
#include "core/utils/autoproperty.h"

#include "core/utils/logging.h"
#include "core/errors_core.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


typedef int Identifier;


class Entity : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(bool isValid READ isValid NOTIFY isValidChanged)

public:
    Entity(AbstractRepository *repository = nullptr, QObject *parent = nullptr);

    Entity(AbstractRepository *repository, const int &id, QObject *parent = nullptr);

    virtual ~Entity() = 0;

    virtual int id() const {return _id;}
    virtual void setId(const int &id) {_id = id; emit idChanged(id);}

    virtual bool isValid();

    virtual bool save();
    bool isModified() const;

//    Q_INVOKABLE QObject* raw(QString spropname) const {
//        Entity* that = this->property(&spropname.toStdString()[0]).value<Entity*>();
//        return that.data();
//    }
//    bool save() {DB->setDBObject(*this);}

    void setRecursiveModified(bool modified = false);

protected slots:
    void setModified(bool modified = true);

signals:
    void idChanged(const int &id);
    void isValidChanged(const int &id);
    void hasBeenModified();

protected:
    int _id;
    int _isValid;
    bool _isModified;
    AbstractRepository *_repository;
    QVector<bool> _isValids;
};
typedef Entity* SharedEntity;
typedef QList<SharedEntity> SharedEntitySet;

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

//Q_DECLARE_METATYPE(int)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::Entity*)

#endif // ENTITY_H
