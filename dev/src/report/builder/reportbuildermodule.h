#ifndef REPORTBUILDERMODULE_H
#define REPORTBUILDERMODULE_H

class ReportData;

#include <QObject>
#include "core/interfaces/genericdatabase.h"
#include "core/utils/errorenums.h"
#include "admin/src/dal/institute.h"
#include "admin/src/dal/practician.h"
#include "admin/src/dal/patient.h"
#include "reportbuildermodule.h"

class ReportBuilderModule
{

public:
    virtual bool setData(ReportData *data) = 0;
    virtual void unsetData(ReportData *data) = 0;
};

#endif // REPORTBUILDERMODULE_H
