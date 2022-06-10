//@@license@@

#ifndef INTERPRETATIONTOREQUESTXMLEXPORT_H
#define INTERPRETATIONTOREQUESTXMLEXPORT_H

#include <QString>
#include <QXmlStreamWriter>

class Interpretation;
class ClinicalSet;
class Patient;

namespace Tucuxi {
namespace Gui {
namespace Core {
class ActiveSubstance;
class DosageHistory;
class CoreMeasureList;
class PatientVariateList;
}
}
}

class InterpretationToRequestXmlExport
{
public:
    InterpretationToRequestXmlExport();

    QString toXml(Interpretation *interpretation);

protected:

    bool saveActiveSubstance(Tucuxi::Gui::Core::ActiveSubstance *activeSubstance);
    bool saveDosages(Tucuxi::Gui::Core::DosageHistory *dosageHistory);
    bool saveSamples(Tucuxi::Gui::Core::CoreMeasureList *samples);
    bool saveCovariates(Tucuxi::Gui::Core::PatientVariateList *covariates);
    bool saveClinicals(ClinicalSet *clinicals);
    bool savePatient(Patient *patient);

    QXmlStreamWriter writer;


};


#endif // INTERPRETATIONTOREQUESTXMLEXPORT_H
