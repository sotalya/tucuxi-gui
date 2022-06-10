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
class PhoneList;

class ClinicalSet;
class Patient;
class Person;

namespace Tucuxi {
namespace Gui {
namespace Core {

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
    bool save(Tucuxi::Gui::Core::DrugResponseAnalysis *drugResponseAnalysis);
    bool save(Tucuxi::Gui::Core::DrugTreatment *drugResponseAnalysis);
    bool save(Practician *practician, QString name);
    bool save(ValidationStatus *validationStatus);
    bool save(Patient *patient);
    bool save(Person *person, QString name);
    bool save(ClinicalSet *clinicals);
    bool save(PhoneList *list);


    QString writeDate(QDateTime date);
    bool save(Tucuxi::Gui::Core::Dosage *dosage, const QString &tagName = "dosage");
    bool save(Tucuxi::Gui::Core::AdjustmentDosage *dosage, const QString &tagName = "dosage");
    bool save(Tucuxi::Gui::Core::DosageHistory *history, const QString &tagName = "dosageHistory");
    bool save(Tucuxi::Gui::Core::PatientVariateList *list);
    bool save(Tucuxi::Gui::Core::TargetList *list);
    bool save(Tucuxi::Gui::Core::CoreMeasureList *list);
    bool save(Tucuxi::Gui::Core::UncastedValueList *list);
    bool save(Tucuxi::Gui::Core::ADME *adme);
    bool save(Tucuxi::Gui::Core::ValidDoses *doses);
    bool save(Tucuxi::Gui::Core::ValidInfusions *infusions);
    bool save(Tucuxi::Gui::Core::ValidIntervals *intervals);
    bool save(Tucuxi::Gui::Core::DrugVariateList *list);
    bool save(Tucuxi::Gui::Core::ParameterSet *set);
    bool save(Tucuxi::Gui::Core::OperationList *list);
    bool saveIdentifiableAmount(const QString &tagName, Tucuxi::Gui::Core::IdentifiableAmount *amount);
    bool saveOperableAmount(const QString &tagName, Tucuxi::Gui::Core::OperableAmount *amount);

    QXmlStreamWriter writer;


};

#endif // INTERPRETATIONXMLEXPORT_H
