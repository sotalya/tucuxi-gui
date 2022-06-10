#ifndef COVARIATESBUILDERMODULE_H
#define COVARIATESBUILDERMODULE_H

#include "core/dal/entity.h"
#include "core/dal/covariate.h"
#include "core/dal/drug/drugvariate.h"
#include "admin/src/adminfactory.h"
#include "databases/ephemeraldb/ephemeraldb.h"
#include "admin/src/stdadminrepository.h"
#include "reportbuildermodule.h"


class CovariatesBuilderModule : public Tucuxi::Gui::Core::Entity, public ReportBuilderModule
{
    Q_OBJECT
    AUTO_PROPERTY_DECL(Tucuxi::Gui::Core::DrugVariateList*, drugvariates, Drugvariates)
    AUTO_PROPERTY_DECL(Tucuxi::Gui::Core::PatientVariateList*, patientvariates, Patientvariates)

public:
    CovariatesBuilderModule(Tucuxi::Gui::Core::DrugVariateList *drugvariates = 0, Tucuxi::Gui::Core::PatientVariateList *covariates = 0, QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);
};

#endif // COVARIATESBUILDERMODULE_H
