#ifndef TREATMENTBUILDERMODULE_H
#define TREATMENTBUILDERMODULE_H

#include "core/dal/entity.h"
#include "core/dal/drugtreatment.h"
#include "reportbuildermodule.h"

class TreatmentBuilderModule : public ezechiel::GuiCore::Entity, public ReportBuilderModule
{
    Q_OBJECT

public:
    TreatmentBuilderModule(ezechiel::GuiCore::DrugTreatment* treatment = 0, QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);

    const ezechiel::GuiCore::DrugTreatment* treatment() const;
    TreatmentBuilderModule &setTreatment(ezechiel::GuiCore::DrugTreatment* treatment);

private:
    ezechiel::GuiCore::DrugTreatment* _treatment;
};

#endif // TREATMENTBUILDERMODULE_H
