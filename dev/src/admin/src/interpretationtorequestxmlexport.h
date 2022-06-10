//@@license@@

#ifndef INTERPRETATIONTOREQUESTXMLEXPORT_H
#define INTERPRETATIONTOREQUESTXMLEXPORT_H

#include <QString>
#include <QXmlStreamWriter>

class Interpretation;
class ClinicalSet;
class Patient;

namespace Tucuxi {
namespace GuiCore {
class ActiveSubstance;
class DosageHistory;
class CoreMeasureList;
class PatientVariateList;
}
}

class InterpretationToRequestXmlExport
{
public:
    InterpretationToRequestXmlExport();

    QString toXml(Interpretation *interpretation);

protected:

    bool saveActiveSubstance(Tucuxi::GuiCore::ActiveSubstance *activeSubstance);
    bool saveDosages(Tucuxi::GuiCore::DosageHistory *dosageHistory);
    bool saveSamples(Tucuxi::GuiCore::CoreMeasureList *samples);
    bool saveCovariates(Tucuxi::GuiCore::PatientVariateList *covariates);
    bool saveClinicals(ClinicalSet *clinicals);
    bool savePatient(Patient *patient);

    QXmlStreamWriter writer;


};


#endif // INTERPRETATIONTOREQUESTXMLEXPORT_H
