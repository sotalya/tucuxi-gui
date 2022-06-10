#ifndef DOSAGEBUILDERMODULE_H
#define DOSAGEBUILDERMODULE_H

#include "core/dal/entity.h"
#include "core/dal/dosage.h"
#include "core/corefactory.h"
#include "core/dal/dosage.h"
#include "reportbuildermodule.h"

class DosageBuilderModule : public Tucuxi::GuiCore::Entity, public ReportBuilderModule
{
    Q_OBJECT

public:
    DosageBuilderModule(Tucuxi::GuiCore::Dosage *dosage, QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);

    Tucuxi::GuiCore::Dosage* dosage() const;
    DosageBuilderModule &setDosage(Tucuxi::GuiCore::Dosage *&dosage);

private:
    Tucuxi::GuiCore::Dosage* _dosage;
};

#endif // DOSAGEBUILDERMODULE_H
