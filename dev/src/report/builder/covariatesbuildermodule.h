#ifndef COVARIATESBUILDERMODULE_H
#define COVARIATESBUILDERMODULE_H

#include "core/dal/entity.h"
#include "core/dal/covariate.h"
#include "core/dal/drug/drugvariate.h"
#include "admin/src/adminfactory.h"
#include "databases/ephemeraldb/ephemeraldb.h"
#include "admin/src/stdadminrepository.h"
#include "reportbuildermodule.h"


class CovariatesBuilderModule : public ezechiel::GuiCore::Entity, public ReportBuilderModule
{
    Q_OBJECT
    AUTO_PROPERTY_DECL(ezechiel::GuiCore::DrugVariateList*, drugvariates, Drugvariates)
    AUTO_PROPERTY_DECL(ezechiel::GuiCore::PatientVariateList*, patientvariates, Patientvariates)

public:
    CovariatesBuilderModule(ezechiel::GuiCore::DrugVariateList *drugvariates = 0, ezechiel::GuiCore::PatientVariateList *covariates = 0, QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);
};

#endif // COVARIATESBUILDERMODULE_H
