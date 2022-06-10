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
namespace Gui {
namespace Core {

class Dosage;
}
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

    bool lastMeasure(Measure * measure, Tucuxi::Gui::Core::ident &patientId);
    bool lastDosage(Tucuxi::Gui::Core::Dosage* &dosage, Tucuxi::Gui::Core::ident &curveId);

private:
    static const char *const _separator;

    bool loadPatients();
    bool patientExists(const QString &name, const QString &firstname) const;
    Tucuxi::Gui::Core::ident patientId(const QString &name, const QString &firstname) const;

    bool getPatient(const QString &firstname, const QString &name, SharedPatient &patient);
    bool getDrugId(const QString &drugName, QString &drugId);
    bool getDate(const QString &dateStr, QDateTime &date);
    bool getValue(const QString &valueStr, double &value);
    bool getValue(const QString &valueStr, int &value);

    bool setDosage(const Tucuxi::Gui::Core::ident curveId, Tucuxi::Gui::Core::Dosage* &dosage);
    bool setCurve(const QString drugId, const QString &curveName, const SharedPatient &patient, Tucuxi::Gui::Core::Dosage* &dosage);

    bool tryRequest(const Tucuxi::Gui::Core::Response &r, const QString &msg);

    QHash<QString, Tucuxi::Gui::Core::ident> _patients;

    Measure * _lastMeasure;
    Tucuxi::Gui::Core::Dosage* _lastDosage;
    Tucuxi::Gui::Core::ident _lastPatientId;
    Tucuxi::Gui::Core::ident _lastCurveId;

};

} //namespace Core
 //namespace Gui
} //namespace Tucuxi
#endif // ISYPEMACTION_H
