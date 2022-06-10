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

/** \ingroup utils
 *  \brief A patient medical information or condition.
 *
 * A clinical is a medical information about a patient.
 * Its identifier is unique along the whole software and in the local database.
 */
class Clinical : public Tucuxi::GuiCore::Entity {
    Q_OBJECT

    ADMIN_ENTITY_UTILS(Clinical)

    AUTO_PROPERTY_DECL(QString, name, Name)
    AUTO_PROPERTY_DECL(QDateTime, date, Date)
    AUTO_PROPERTY_DECL(QString, value, Value)
    AUTO_PROPERTY_DECL(QString, comment, Comment)
    AUTO_PROPERTY_DECL(Patient*, patient, Patient)
    AUTO_PROPERTY_DECL(int, patient_id, Patient_id)

public:

    Q_INVOKABLE Clinical(Tucuxi::GuiCore::AbstractRepository *repository, QObject* parent = 0, const QString &name = "", const QDateTime &date = QDateTime());
    Q_INVOKABLE Clinical(Tucuxi::GuiCore::AbstractRepository *repository, QObject* parent, const QString &name, const QDateTime &date, QString value);

    QString description() const {return _description;}
    void setDescription(QString description) {_description = description;}

    Tucuxi::GuiCore::Descriptor descriptor() const {return _descriptor;}
    void setDescriptor(Tucuxi::GuiCore::Descriptor descriptor) {_descriptor = descriptor;}

    QString toString() const;
    bool isValid () const;

    void operator= (QString value);

    bool operator== (Clinical* ) const;

    bool operator!= (Clinical* ) const;

signals:
    void descriptorChanged(Tucuxi::GuiCore::Descriptor);
    void descriptionChanged(QString);

private:
    QString _description;
    Tucuxi::GuiCore::Descriptor _descriptor;
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
    ClinicalSet(Tucuxi::GuiCore::AbstractRepository* repository, QObject* parent) {}
    //! Find if a clinical is in the list, and return its place (or -1 if not found).
    int find (const QString &name) const;
    //! Compare the two lists, item by item.
    bool operator== (const ClinicalSet &) const;
};
typedef ClinicalSet* SharedClinicalSet;


Q_DECLARE_METATYPE(Clinical*)
Q_DECLARE_METATYPE(QList<Clinical*>)
Q_DECLARE_METATYPE(ClinicalSet*)

#endif // CLINICAL_H
