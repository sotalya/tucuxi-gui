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
namespace GuiCore {
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
    bool save(Tucuxi::GuiCore::DrugResponseAnalysis *drugResponseAnalysis);
    bool save(Tucuxi::GuiCore::DrugTreatment *drugResponseAnalysis);
    bool save(Practician *practician, QString name);
    bool save(ValidationStatus *validationStatus);
    bool save(Patient *patient);
    bool save(Person *person, QString name);
    bool save(ClinicalSet *clinicals);
    bool save(PhoneList *list);


    QString writeDate(QDateTime date);
    bool save(Tucuxi::GuiCore::Dosage *dosage, const QString &tagName = "dosage");
    bool save(Tucuxi::GuiCore::AdjustmentDosage *dosage, const QString &tagName = "dosage");
    bool save(Tucuxi::GuiCore::DosageHistory *history, const QString &tagName = "dosageHistory");
    bool save(Tucuxi::GuiCore::PatientVariateList *list);
    bool save(Tucuxi::GuiCore::TargetList *list);
    bool save(Tucuxi::GuiCore::CoreMeasureList *list);
    bool save(Tucuxi::GuiCore::UncastedValueList *list);
    bool save(Tucuxi::GuiCore::ADME *adme);
    bool save(Tucuxi::GuiCore::ValidDoses *doses);
    bool save(Tucuxi::GuiCore::ValidInfusions *infusions);
    bool save(Tucuxi::GuiCore::ValidIntervals *intervals);
    bool save(Tucuxi::GuiCore::DrugVariateList *list);
    bool save(Tucuxi::GuiCore::ParameterSet *set);
    bool save(Tucuxi::GuiCore::OperationList *list);
    bool saveIdentifiableAmount(const QString &tagName, Tucuxi::GuiCore::IdentifiableAmount *amount);
    bool saveOperableAmount(const QString &tagName, Tucuxi::GuiCore::OperableAmount *amount);

    QXmlStreamWriter writer;


};

#endif // INTERPRETATIONXMLEXPORT_H
