#ifndef PATIENTBUILDERMODULE_H
#define PATIENTBUILDERMODULE_H

#include "core/dal/entity.h"
#include "report/reportdata.h"
#include "admin/src/dal/patient.h"
#include "reportbuildermodule.h"

class PatientBuilderModule : public ezechiel::GuiCore::Entity, public ReportBuilderModule
{
    Q_OBJECT
    AUTO_PROPERTY_DECL(Patient*, patient, Patient)

public:
    PatientBuilderModule(Patient* patient, QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);
};

#endif // PATIENTBUILDERMODULE_H
