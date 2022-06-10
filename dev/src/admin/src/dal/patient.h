#ifndef PATIENT_H
#define PATIENT_H

#include "core/dal/corepatient.h"
#include "person.h"

class Patient : public Tucuxi::Gui::Core::CorePatient
{
    Q_OBJECT

    ADMIN_ENTITY_UTILS(Patient)

    AUTO_PROPERTY(QString, externalId)
    AUTO_PROPERTY(QString, stayNumber)

    // Do not know what is this statOk about
    AUTO_PROPERTY(bool, statOk)
    AUTO_PROPERTY(Person*, person)
    AUTO_PROPERTY(int, person_id)

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

public:
    Q_INVOKABLE Patient(Tucuxi::Gui::Core::AbstractRepository *repository = 0, QObject *parent = nullptr);
    Q_INVOKABLE Patient(Tucuxi::Gui::Core::AbstractRepository *repository, const int &id, QObject *parent = nullptr);

};
Q_DECLARE_METATYPE(Patient*)
Q_DECLARE_METATYPE(QList<Patient*>)

//QML_POINTERLIST_CLASS_DECL(PatientList, Patient)

typedef Tucuxi::Gui::Core::CorePatient* SharedPatient;

#endif // PATIENT_H
