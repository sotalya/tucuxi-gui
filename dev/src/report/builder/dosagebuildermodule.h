#ifndef DOSAGEBUILDERMODULE_H
#define DOSAGEBUILDERMODULE_H

#include "core/dal/entity.h"
#include "core/dal/dosage.h"
#include "core/corefactory.h"
#include "core/dal/dosage.h"
#include "reportbuildermodule.h"

class DosageBuilderModule : public Tucuxi::Gui::Core::Entity, public ReportBuilderModule
{
    Q_OBJECT

public:
    DosageBuilderModule(Tucuxi::Gui::Core::Dosage *dosage, QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);

    Tucuxi::Gui::Core::Dosage* dosage() const;
    DosageBuilderModule &setDosage(Tucuxi::Gui::Core::Dosage *&dosage);

private:
    Tucuxi::Gui::Core::Dosage* _dosage;
};

#endif // DOSAGEBUILDERMODULE_H
