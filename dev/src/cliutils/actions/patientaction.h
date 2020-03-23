#ifndef PATIENTACTION_H
#define PATIENTACTION_H

#include <QCoreApplication>
#include <QList>
#include <QString>

#include "action.h"
//#include "core/interfaces/database.h"
#include "core/core.h"
//#include "coremessagehandler.h"
//#include "dbobjects.h"
#include "core/utils/errorenums.h"
#include "admin/src/dal/patient.h"

/** The patient related action.
 * \ingroup actions cli
 *
 * The patient action regroups all the patient and its person commands.
 * With it, its possible to :
 * \li add a new patient
 * \li edit or remove a patient
 * \li manage the patient measures
 * \li manage the patient covariates
 */

namespace ezechiel {
namespace cliutils {
class PatientAction : public Action
{
    Q_DECLARE_TR_FUNCTIONS(PatientAction)

public:
    PatientAction();

    static QString name() { return "patient"; }

    QString help();

    //Will separate between the sub-actions
    virtual bool run (const QStringList &args);

private:

    //Try to do this DB request, and return false if it didn't work with the given message
    bool tryRequest(const ezechiel::core::Response &r, const QString &msg);

    // Patients
    bool listPatients ();
    bool createPatient (const QStringList &args);
    bool removePatient (SharedPatient &patient, QStringList &args);

    // Measures
    bool measure (const SharedPatient &patient, QStringList &args);
    bool listMeasures (const SharedPatient &patient);
    bool setMeasure (const SharedPatient &patient,QStringList &args);
    bool removeMeasure (const SharedPatient &patient, QStringList &args);

    // Covariates
    bool covariate (const SharedPatient &patient, QStringList &args);
    bool listCovariates (const SharedPatient &patient);
    bool setCovariate (const SharedPatient &patient, QStringList &args);
    bool removeCovariate (const SharedPatient &patient, QStringList &args);
};

} //namespace cliutils
} //namespace ezechiel
#endif // PATIENTACTION_H
