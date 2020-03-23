#ifndef COVARIATESBUILDERMODULE_H
#define COVARIATESBUILDERMODULE_H

#include "core/dal/entity.h"
#include "core/dal/covariate.h"
#include "core/dal/drug/drugvariate.h"
#include "admin/src/adminfactory.h"
#include "databases/ephemeraldb/ephemeraldb.h"
#include "admin/src/stdadminrepository.h"
#include "reportbuildermodule.h"


class CovariatesBuilderModule : public ezechiel::core::Entity, public ReportBuilderModule
{
    Q_OBJECT
    AUTO_PROPERTY_DECL(ezechiel::core::DrugVariateList*, drugvariates, Drugvariates)
    AUTO_PROPERTY_DECL(ezechiel::core::PatientVariateList*, patientvariates, Patientvariates)

public:
    CovariatesBuilderModule(ezechiel::core::DrugVariateList *drugvariates = 0, ezechiel::core::PatientVariateList *covariates = 0, QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);
};

#endif // COVARIATESBUILDERMODULE_H
