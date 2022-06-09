#include "covariatesbuildermodule.h"
#include "reportdata.h"
#include "identifiers.h"
#include "stdadminrepository.h"
#include "adminfactory.h"

using namespace ezechiel::GuiCore;

AUTO_PROPERTY_IMPL(CovariatesBuilderModule, ezechiel::GuiCore::DrugVariateList*, drugvariates, Drugvariates)
AUTO_PROPERTY_IMPL(CovariatesBuilderModule, ezechiel::GuiCore::PatientVariateList*, patientvariates, Patientvariates)

CovariatesBuilderModule::CovariatesBuilderModule(DrugVariateList * drugvariates, PatientVariateList *patientvariates, QObject *parent) :
    _patientvariates(patientvariates), _drugvariates(drugvariates)
{

}

bool CovariatesBuilderModule::setData(ReportData *data)
{
    if (_drugvariates->size() == 0)
        return true;

    data->setValue(ID::Covariates::Count, QString::number(_drugvariates->size()));

    for (int i = 0; i < _drugvariates->size(); ++i) {
        DrugVariate* drugvariate = _drugvariates->at(i);

        data->setValue(QString(ID::DrugVariates::DrugVariate::Id).arg(i), QString::number(drugvariate->id()));
        data->setValue(QString(ID::DrugVariates::DrugVariate::Identifier).arg(i), drugvariate->getName());
        data->setValue(QString(ID::DrugVariates::DrugVariate::Name).arg(i), drugvariate->getVisualNameTranslation()->value("en"));
        data->setValue(QString(ID::DrugVariates::DrugVariate::Description).arg(i), drugvariate->getDescription());
        data->setValue(QString(ID::DrugVariates::DrugVariate::Value).arg(i), QString::number(drugvariate->getQuantity()->value()));
        data->setValue(QString(ID::DrugVariates::DrugVariate::Unit).arg(i), drugvariate->getQuantity()->unit().name());
        data->setValue(QString(ID::DrugVariates::DrugVariate::Type).arg(i), QMetaType::typeName(drugvariate->getType()));
    }

    if (_patientvariates->size() == 0)
        return true;

    data->setValue(ID::Covariates::Count, QString::number(_patientvariates->size()));

    for (int i = 0; i < _patientvariates->size(); ++i) {
        PatientVariate* covariate = _patientvariates->at(i);

        data->setValue(QString(ID::Covariates::Covariate::Id).arg(i), QString::number(covariate->id()));
        data->setValue(QString(ID::Covariates::Covariate::Identifier).arg(i), covariate->getName());
        data->setValue(QString(ID::Covariates::Covariate::Name).arg(i), covariate->getName());
        data->setValue(QString(ID::Covariates::Covariate::Description).arg(i), covariate->getDescription());
        data->setValue(QString(ID::Covariates::Covariate::Value).arg(i), QString::number(covariate->getQuantity()->value()));
        data->setValue(QString(ID::Covariates::Covariate::Unit).arg(i), covariate->getQuantity()->unit().name());
        data->setValue(QString(ID::Covariates::Covariate::Type).arg(i), QMetaType::typeName(covariate->getType()));
    }

    return true;
}

void CovariatesBuilderModule::unsetData(ReportData *data)
{
    int covariatesCount = data->value(ID::Covariates::Count).toInt();

    for (int i = 0; i < covariatesCount; ++i) {
        data->unsetValue(QString(ID::Covariates::Covariate::Id).arg(i));
        data->unsetValue(QString(ID::Covariates::Covariate::Identifier).arg(i));
        data->unsetValue(QString(ID::Covariates::Covariate::Name).arg(i));
        data->unsetValue(QString(ID::Covariates::Covariate::Description).arg(i));
        data->unsetValue(QString(ID::Covariates::Covariate::Value).arg(i));
        data->unsetValue(QString(ID::Covariates::Covariate::Unit).arg(i));
        data->unsetValue(QString(ID::Covariates::Covariate::Type).arg(i));
    }
    data->unsetValue(ID::Covariates::Count);
}
