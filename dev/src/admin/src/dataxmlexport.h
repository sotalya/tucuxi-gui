//@@license@@

#ifndef DATAXMLEXPORT_H
#define DATAXMLEXPORT_H


#include <QString>
#include <QXmlStreamWriter>

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

namespace Tucuxi {
namespace Gui {
namespace Admin {

class Interpretation;
class InterpretationAnalysis;
class InterpretationRequest;
class Practician;
class ValidationStatus;
class PhoneList;

class ClinicalSet;
class Patient;
class Person;

class DataXmlExport
{
public:
    DataXmlExport();

    QString toXml(Interpretation *interpretation);

protected:
    QString writeDate(QDateTime date);
    bool save(Tucuxi::Gui::Core::DrugResponseAnalysis *drugResponseAnalysis);
    bool save(Tucuxi::Gui::Core::DrugTreatment *treatment);
    bool save(Tucuxi::Gui::Core::PatientVariateList *list);
    bool save(Tucuxi::Gui::Core::CoreMeasureList *list);
    bool save(Tucuxi::Gui::Core::ActiveSubstance *substance);
    bool save(Patient *patient);
    bool save(Core::Dosage *dosage);
    bool save(Tucuxi::Gui::Core::DosageHistory *history);
    bool saveIdentifiableAmount(const QString &tagName, Tucuxi::Gui::Core::IdentifiableAmount *amount);

    QXmlStreamWriter writer;

};

}
}
}

#endif // DATAXMLEXPORT_H
