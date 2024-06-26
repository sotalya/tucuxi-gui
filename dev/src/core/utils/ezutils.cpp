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


#include "core/utils/ezutils.h"

#include "core/utils/units.h"
#include "stdparamnames.h"

namespace Tucuxi {
namespace Gui {
namespace Core {



//----------------------- DESCRIPTOR --------------------------

Descriptor::Descriptor()
{

}

QString Descriptor::getName () const
{
    QString n = StdParamNames::getName(id);
    if (n.isEmpty())
        return name;
    return n;
}

QString Descriptor::getDescription () const
{
    QString d = StdParamNames::getDesc(id);
    if (d.isEmpty())
        return description;
    return d;
}

bool Descriptor::isValid() const
{
    return !id.isEmpty() && !name.isEmpty() && !description.isEmpty();
}

QString Descriptor::toString()
{
    return "ID: " + id + ", Name: " + name + ", Description: " + description;
}

//Check only the non-empty fields
bool Descriptor::operator ==(const Descriptor &other)
{
    bool allEmpty = id.isEmpty() && name.isEmpty() && description.isEmpty();
    bool allOtherEmpty = other.id.isEmpty() && other.name.isEmpty() && other.description.isEmpty();
    return  ( id == other.id || id.isEmpty() || other.id.isEmpty() ) &&
            ( name == other.name || name.isEmpty() || other.name.isEmpty() ) &&
            ( description == other.description || description.isEmpty() || other.description.isEmpty() ) &&
            ! ( allEmpty ^ allOtherEmpty )
            ;
}

//----------------------- MODELDESCRIPTOR --------------------------

ModelDescriptor::ModelDescriptor(QString id, QString modelId, QString name, QString description, QString intake)
    : intake(intake), modelId(modelId)
{
    this->id = id;
    this->name = name;
    this->description = description;
}

QString ModelDescriptor::toString()
{
    return QString("%1, Known Model Id: %2, Intake : %3").arg(Descriptor::toString(), modelId, intake);
}

bool ModelDescriptor::operator ==(const ModelDescriptor &other)
{
    bool allEmpty = id.isEmpty() && modelId.isEmpty() && name.isEmpty() && description.isEmpty() && intake.isEmpty();
    bool allOtherEmpty = other.id.isEmpty() && other.modelId.isEmpty() && other.name.isEmpty() && other.description.isEmpty() && intake.isEmpty();
    return  ( id == other.id || id.isEmpty() || other.id.isEmpty() ) &&
            ( modelId == other.modelId || modelId.isEmpty() || other.modelId.isEmpty() ) &&
            ( name == other.name || name.isEmpty() || other.name.isEmpty() ) &&
            ( description == other.description || description.isEmpty() || other.description.isEmpty() ) &&
            ( intake == other.intake || intake.isEmpty() || other.intake.isEmpty() ) &&
            ! ( allEmpty ^ allOtherEmpty )
            ;
}

//----------------------- OPTION --------------------------

Option::Option() : readonly(false), sqlId(invalid_ident)
{

}

Option::Option(Descriptor infos, int type, QVariant value, QString oid, QString group, bool readonly)
    : infos(infos), value(value), oid(oid), group(group), readonly(readonly), sqlId(invalid_ident)
{
    this->type = QMetaType::Type(type);
}

//----------------------- FILENAME-------------------------

FileName::FileName()
{
}

FileName::FileName(const QString &name):QString(name)
{
}

FileName::FileName(const QString &name,const QString &ext):QString(name)
{
    this->ext=ext;
}


//----------------------- DIRNAME-------------------------

DirName::DirName()
{
}

DirName::DirName(const QString &name):QString(name)
{
}

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

