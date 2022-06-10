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



namespace Tucuxi {
namespace GuiCore {
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

    bool lastMeasure(Measure * measure, Tucuxi::GuiCore::ident &patientId);
    bool lastDosage(Tucuxi::GuiCore::Dosage* &dosage, Tucuxi::GuiCore::ident &curveId);

private:
    static const char *const _separator;

    bool loadPatients();
    bool patientExists(const QString &name, const QString &firstname) const;
    Tucuxi::GuiCore::ident patientId(const QString &name, const QString &firstname) const;

    bool getPatient(const QString &firstname, const QString &name, SharedPatient &patient);
    bool getDrugId(const QString &drugName, QString &drugId);
    bool getDate(const QString &dateStr, QDateTime &date);
    bool getValue(const QString &valueStr, double &value);
    bool getValue(const QString &valueStr, int &value);

    bool setDosage(const Tucuxi::GuiCore::ident curveId, Tucuxi::GuiCore::Dosage* &dosage);
    bool setCurve(const QString drugId, const QString &curveName, const SharedPatient &patient, Tucuxi::GuiCore::Dosage* &dosage);

    bool tryRequest(const Tucuxi::GuiCore::Response &r, const QString &msg);

    QHash<QString, Tucuxi::GuiCore::ident> _patients;

    Measure * _lastMeasure;
    Tucuxi::GuiCore::Dosage* _lastDosage;
    Tucuxi::GuiCore::ident _lastPatientId;
    Tucuxi::GuiCore::ident _lastCurveId;

};
} //namespace cliutils
} //namespace Tucuxi
#endif // ISYPEMACTION_H
