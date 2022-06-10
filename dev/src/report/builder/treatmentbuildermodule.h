#ifndef TREATMENTBUILDERMODULE_H
#define TREATMENTBUILDERMODULE_H

#include "core/dal/entity.h"
#include "core/dal/drugtreatment.h"
#include "reportbuildermodule.h"

class TreatmentBuilderModule : public Tucuxi::GuiCore::Entity, public ReportBuilderModule
{
    Q_OBJECT

public:
    TreatmentBuilderModule(Tucuxi::GuiCore::DrugTreatment* treatment = 0, QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);

    const Tucuxi::GuiCore::DrugTreatment* treatment() const;
    TreatmentBuilderModule &setTreatment(Tucuxi::GuiCore::DrugTreatment* treatment);

private:
    Tucuxi::GuiCore::DrugTreatment* _treatment;
};

#endif // TREATMENTBUILDERMODULE_H
