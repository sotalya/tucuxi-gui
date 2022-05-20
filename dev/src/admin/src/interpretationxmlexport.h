//@@license@@

#ifndef INTERPRETATIONXMLEXPORT_H
#define INTERPRETATIONXMLEXPORT_H


#include "admin/src/dal/validationstatus.h"
#include <QString>
#include <QXmlStreamWriter>

class Interpretation;
class InterpretationAnalysis;
class InterpretationRequest;
class Practician;
class ValidationStatus;

class ClinicalSet;
class Patient;
class Person;

namespace ezechiel {
namespace core {
class ActiveSubstance;
class Dosage;
class AdjustmentDosage;
class DosageHistory;
class CoreMeasureList;
class PatientVariateList;
class DrugResponseAnalysis;
class DrugTreatment;
class TargetList;
class UncastedValueList;
class ADME;
class ValidDoses;
class ValidInfusions;
class ValidIntervals;
class DrugVariateList;
class ParameterSet;
class OperationList;
class IdentifiableAmount;
class OperableAmount;
}
}

class InterpretationXmlExport
{
public:
    InterpretationXmlExport();

    QString toXml(Interpretation *interpretation);
    QString stepToString(int step);
    QString toStringValidation(ValidationStatus::ValidationStatusType status);

protected:

    bool save(InterpretationAnalysis *analysis);
    bool save(InterpretationRequest *request);
    bool save(ezechiel::core::DrugResponseAnalysis *drugResponseAnalysis);
    bool save(ezechiel::core::DrugTreatment *drugResponseAnalysis);
    bool save(Practician *analyst, QString name);
    bool save(ValidationStatus *validationStatus);
    bool save(Patient *patient);
    bool save(Person *person);
    bool save(ClinicalSet *clinicals);


    QString writeDate(QDateTime date);
    bool save(ezechiel::core::Dosage *dosage, const QString &tagName = "dosage");
    bool save(ezechiel::core::AdjustmentDosage *dosage, const QString &tagName = "dosage");
    bool save(ezechiel::core::DosageHistory *history, const QString &tagName = "dosageHistory");
    bool save(ezechiel::core::PatientVariateList *list);
    bool save(ezechiel::core::TargetList *list);
    bool save(ezechiel::core::CoreMeasureList *list);
    bool save(ezechiel::core::UncastedValueList *list);
    bool save(ezechiel::core::ADME *adme);
    bool save(ezechiel::core::ValidDoses *doses);
    bool save(ezechiel::core::ValidInfusions *infusions);
    bool save(ezechiel::core::ValidIntervals *intervals);
    bool save(ezechiel::core::DrugVariateList *list);
    bool save(ezechiel::core::ParameterSet *set);
    bool save(ezechiel::core::OperationList *list);
    bool saveIdentifiableAmount(const QString &tagName, ezechiel::core::IdentifiableAmount *amount);
    bool saveOperableAmount(const QString &tagName, ezechiel::core::OperableAmount *amount);

    QXmlStreamWriter writer;


};

#endif // INTERPRETATIONXMLEXPORT_H
