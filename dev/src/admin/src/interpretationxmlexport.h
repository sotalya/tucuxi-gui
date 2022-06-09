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

namespace ezechiel {
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
    bool save(ezechiel::GuiCore::DrugResponseAnalysis *drugResponseAnalysis);
    bool save(ezechiel::GuiCore::DrugTreatment *drugResponseAnalysis);
    bool save(Practician *practician, QString name);
    bool save(ValidationStatus *validationStatus);
    bool save(Patient *patient);
    bool save(Person *person, QString name);
    bool save(ClinicalSet *clinicals);
    bool save(PhoneList *list);


    QString writeDate(QDateTime date);
    bool save(ezechiel::GuiCore::Dosage *dosage, const QString &tagName = "dosage");
    bool save(ezechiel::GuiCore::AdjustmentDosage *dosage, const QString &tagName = "dosage");
    bool save(ezechiel::GuiCore::DosageHistory *history, const QString &tagName = "dosageHistory");
    bool save(ezechiel::GuiCore::PatientVariateList *list);
    bool save(ezechiel::GuiCore::TargetList *list);
    bool save(ezechiel::GuiCore::CoreMeasureList *list);
    bool save(ezechiel::GuiCore::UncastedValueList *list);
    bool save(ezechiel::GuiCore::ADME *adme);
    bool save(ezechiel::GuiCore::ValidDoses *doses);
    bool save(ezechiel::GuiCore::ValidInfusions *infusions);
    bool save(ezechiel::GuiCore::ValidIntervals *intervals);
    bool save(ezechiel::GuiCore::DrugVariateList *list);
    bool save(ezechiel::GuiCore::ParameterSet *set);
    bool save(ezechiel::GuiCore::OperationList *list);
    bool saveIdentifiableAmount(const QString &tagName, ezechiel::GuiCore::IdentifiableAmount *amount);
    bool saveOperableAmount(const QString &tagName, ezechiel::GuiCore::OperableAmount *amount);

    QXmlStreamWriter writer;


};

#endif // INTERPRETATIONXMLEXPORT_H
