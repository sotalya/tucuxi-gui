//@@license@@

#ifndef INTERPRETATIONTOREQUESTXMLEXPORT_H
#define INTERPRETATIONTOREQUESTXMLEXPORT_H

#include <QString>
#include <QXmlStreamWriter>

class Interpretation;
class ClinicalSet;
class Patient;

namespace ezechiel {
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

    bool saveActiveSubstance(ezechiel::GuiCore::ActiveSubstance *activeSubstance);
    bool saveDosages(ezechiel::GuiCore::DosageHistory *dosageHistory);
    bool saveSamples(ezechiel::GuiCore::CoreMeasureList *samples);
    bool saveCovariates(ezechiel::GuiCore::PatientVariateList *covariates);
    bool saveClinicals(ClinicalSet *clinicals);
    bool savePatient(Patient *patient);

    QXmlStreamWriter writer;


};


#endif // INTERPRETATIONTOREQUESTXMLEXPORT_H
