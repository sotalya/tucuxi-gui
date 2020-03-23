#ifndef DOSAGEBUILDERMODULE_H
#define DOSAGEBUILDERMODULE_H

#include "core/dal/entity.h"
#include "core/dal/dosage.h"
#include "core/corefactory.h"
#include "core/dal/dosage.h"
#include "reportbuildermodule.h"

class DosageBuilderModule : public ezechiel::core::Entity, public ReportBuilderModule
{
    Q_OBJECT

public:
    DosageBuilderModule(ezechiel::core::Dosage *dosage, QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);

    ezechiel::core::Dosage* dosage() const;
    DosageBuilderModule &setDosage(ezechiel::core::Dosage *&dosage);

private:
    ezechiel::core::Dosage* _dosage;
};

#endif // DOSAGEBUILDERMODULE_H
