#ifndef INTERPRETATIONTOREQUESTXMLEXPORT_H
#define INTERPRETATIONTOREQUESTXMLEXPORT_H

#include <QString>
#include <QXmlStreamWriter>

class Interpretation;
class ClinicalSet;
class Patient;

namespace ezechiel {
namespace core {
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

    bool saveActiveSubstance(ezechiel::core::ActiveSubstance *activeSubstance);
    bool saveDosages(ezechiel::core::DosageHistory *dosageHistory);
    bool saveSamples(ezechiel::core::CoreMeasureList *samples);
    bool saveCovariates(ezechiel::core::PatientVariateList *covariates);
    bool saveClinicals(ClinicalSet *clinicals);
    bool savePatient(Patient *patient);

    QXmlStreamWriter writer;


};


#endif // INTERPRETATIONTOREQUESTXMLEXPORT_H
