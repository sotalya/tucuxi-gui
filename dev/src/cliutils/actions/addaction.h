//@@license@@

#ifndef ISYPEMACTION_H
#define ISYPEMACTION_H

#include <QCoreApplication>
#include <QStringList>

#include "action.h"
//#include "dbobjects.h"
#include "admin/src/dal/measure.h"
#include "admin/src/dal/patient.h"
#include "core/utils/errorenums.h"


//class Response;
class CurveModel;



namespace ezechiel {
namespace core {
class Dosage;
}
namespace cliutils {


class AddAction : public Action
{
    Q_DECLARE_TR_FUNCTIONS(ChuvAction)

public:
    AddAction();

    static QString name();
    QString help();

    bool run(const QStringList &args);

    bool addMeasure(const QString &firstname, const QString &name, const QString &drugName, const QString &dateStr, const QString &concentrationStr);
    bool addDosage(const QString &firstname, const QString &name, const QString &drugName, const QString &dateStr, const QString &doseStr, const QString &intervalStr);

    bool lastMeasure(Measure * measure, ezechiel::core::ident &patientId);
    bool lastDosage(ezechiel::core::Dosage* &dosage, ezechiel::core::ident &curveId);

private:
    static const char *const _separator;

    bool loadPatients();
    bool patientExists(const QString &name, const QString &firstname) const;
    ezechiel::core::ident patientId(const QString &name, const QString &firstname) const;

    bool getPatient(const QString &firstname, const QString &name, SharedPatient &patient);
    bool getDrugId(const QString &drugName, QString &drugId);
    bool getDate(const QString &dateStr, QDateTime &date);
    bool getValue(const QString &valueStr, double &value);
    bool getValue(const QString &valueStr, int &value);

    bool setDosage(const ezechiel::core::ident curveId, ezechiel::core::Dosage* &dosage);
    bool setCurve(const QString drugId, const QString &curveName, const SharedPatient &patient, ezechiel::core::Dosage* &dosage);

    bool tryRequest(const ezechiel::core::Response &r, const QString &msg);

    QHash<QString, ezechiel::core::ident> _patients;

    Measure * _lastMeasure;
    ezechiel::core::Dosage* _lastDosage;
    ezechiel::core::ident _lastPatientId;
    ezechiel::core::ident _lastCurveId;

};
} //namespace cliutils
} //namespace ezechiel
#endif // ISYPEMACTION_H
