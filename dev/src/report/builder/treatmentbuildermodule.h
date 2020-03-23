#ifndef TREATMENTBUILDERMODULE_H
#define TREATMENTBUILDERMODULE_H

#include "core/dal/entity.h"
#include "core/dal/drugtreatment.h"
#include "reportbuildermodule.h"

class TreatmentBuilderModule : public ezechiel::core::Entity, public ReportBuilderModule
{
    Q_OBJECT

public:
    TreatmentBuilderModule(ezechiel::core::DrugTreatment* treatment = 0, QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);

    const ezechiel::core::DrugTreatment* treatment() const;
    TreatmentBuilderModule &setTreatment(ezechiel::core::DrugTreatment* treatment);

private:
    ezechiel::core::DrugTreatment* _treatment;
};

#endif // TREATMENTBUILDERMODULE_H
