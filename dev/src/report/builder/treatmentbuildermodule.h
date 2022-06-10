#ifndef TREATMENTBUILDERMODULE_H
#define TREATMENTBUILDERMODULE_H

#include "core/dal/entity.h"
#include "core/dal/drugtreatment.h"
#include "reportbuildermodule.h"

class TreatmentBuilderModule : public Tucuxi::Gui::Core::Entity, public ReportBuilderModule
{
    Q_OBJECT

public:
    TreatmentBuilderModule(Tucuxi::Gui::Core::DrugTreatment* treatment = 0, QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);

    const Tucuxi::Gui::Core::DrugTreatment* treatment() const;
    TreatmentBuilderModule &setTreatment(Tucuxi::Gui::Core::DrugTreatment* treatment);

private:
    Tucuxi::Gui::Core::DrugTreatment* _treatment;
};

#endif // TREATMENTBUILDERMODULE_H
