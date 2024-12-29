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


#ifndef PARTIALREQUEST_H
#define PARTIALREQUEST_H

#include "core/dal/entity.h"
#include "admin/src/dal/patient.h"
#include "admin/src/dal/practician.h"
#include "admin/src/dal/measure.h"
#include "institute.h"

Q_MOC_INCLUDE("core/dal/drug/drug.h")


namespace Tucuxi {
namespace Gui {
namespace Core {
    class ActiveSubstance;
}
}
}

namespace Tucuxi {
namespace Gui {
namespace Admin {

class PartialRequest : public Tucuxi::Gui::Core::Entity
{
    Q_OBJECT

    AUTO_PROPERTY(QString, requestId)
    AUTO_PROPERTY(QString, requestCpcl)
    AUTO_PROPERTY(Patient*, patient)
    AUTO_PROPERTY(Practician*, practician)
    AUTO_PROPERTY(Institute*, institute)
    AUTO_PROPERTY(Measure*, measure)
    AUTO_PROPERTY(Tucuxi::Gui::Core::ActiveSubstance*, drug)
    AUTO_PROPERTY(QString, uncastedSourceSubstance)
    AUTO_PROPERTY(Measure*, sample)

public:
    Q_INVOKABLE PartialRequest(Tucuxi::Gui::Core::AbstractRepository *repository = 0, QObject *parent = nullptr);
    Q_INVOKABLE PartialRequest(Tucuxi::Gui::Core::AbstractRepository *repository, const int &id, QObject *parent = nullptr);

};

}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::PartialRequest*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Admin::PartialRequest*>)

namespace Tucuxi {
namespace Gui {
namespace Admin {

class PartialRequestSet : public QList<Tucuxi::Gui::Admin::PartialRequest*>
{
public :
    PartialRequestSet(Tucuxi::Gui::Core::AbstractRepository *repository, QObject* parent = 0) {}
    PartialRequestSet(Tucuxi::Gui::Core::AbstractRepository *repository, QObject* parent, const PartialRequestSet* &other);
};

}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::PartialRequestSet*)

typedef Tucuxi::Gui::Admin::PartialRequest* SharedPartialRequest;
typedef Tucuxi::Gui::Admin::PartialRequestSet* SharedPartialRequestSet;

#endif // PARTIALREQUEST_H
