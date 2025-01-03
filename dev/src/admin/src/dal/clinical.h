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


#ifndef CLINICAL_H
#define CLINICAL_H

#include <QString>
#include <QMetaType>
#include <QDateTime>
#include <QMetaType>
#include <QObject>

#include "core/utils/ezutils.h"
#include "core/utils/units.h"

#include "core/dal/entity.h"
#include "core/utils/autoproperty.h"
#include "admin/src/dal/patient.h"

//class Patient;

//class Patient;

namespace Tucuxi {
namespace Gui {
namespace Admin {

/** \ingroup utils
 *  \brief A patient medical information or condition.
 *
 * A clinical is a medical information about a patient.
 * Its identifier is unique along the whole software and in the local database.
 */
class Clinical : public Tucuxi::Gui::Core::Entity {
    Q_OBJECT

    ADMIN_ENTITY_UTILS(Clinical)

    AUTO_PROPERTY_DECL(QString, name, Name)
    AUTO_PROPERTY_DECL(QDateTime, date, Date)
    AUTO_PROPERTY_DECL(QString, value, Value)
    AUTO_PROPERTY_DECL(QString, comment, Comment)
    AUTO_PROPERTY_DECL(Patient*, patient, Patient)
    AUTO_PROPERTY_DECL(int, patient_id, Patient_id)

public:

    Q_INVOKABLE Clinical(Tucuxi::Gui::Core::AbstractRepository *repository, QObject* parent = 0, const QString &name = "", const QDateTime &date = QDateTime());
    Q_INVOKABLE Clinical(Tucuxi::Gui::Core::AbstractRepository *repository, QObject* parent, const QString &name, const QDateTime &date, QString value);

    QString description() const {return _description;}
    void setDescription(QString description) {_description = description;}

    Tucuxi::Gui::Core::Descriptor descriptor() const {return _descriptor;}
    void setDescriptor(Tucuxi::Gui::Core::Descriptor descriptor) {_descriptor = descriptor;}

    QString toString() const;
    bool isValid () const;

    void operator= (QString value);

    bool operator== (Clinical* ) const;

    bool operator!= (Clinical* ) const;

signals:
    void descriptorChanged(Tucuxi::Gui::Core::Descriptor);
    void descriptionChanged(QString);

private:
    QString _description;
    Tucuxi::Gui::Core::Descriptor _descriptor;
    bool _valid;
};

/** \ingroup utils
 *  \brief A set of clinicals.
 *
 * A set of clinical is simply a list of them, with some functions added.
 */
class ClinicalSet : public QList<Clinical*>
{
public :
    ClinicalSet(Tucuxi::Gui::Core::AbstractRepository* repository, QObject* parent) {}
    //! Find if a clinical is in the list, and return its place (or -1 if not found).
    int find (const QString &name) const;
    //! Compare the two lists, item by item.
    bool operator== (const ClinicalSet &) const;
};
typedef ClinicalSet* SharedClinicalSet;

}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::Clinical*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Admin::Clinical*>)
Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::ClinicalSet*)

#endif // CLINICAL_H
