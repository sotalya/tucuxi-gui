#include "parametersbuildermodule.h"
#include "reportdata.h"
#include "identifiers.h"
#include "bsv.h"

using namespace ezechiel::GuiCore;

ParametersBuilderModule::ParametersBuilderModule(ParameterSet* parameters, QObject *parent) :
    _parameters(parameters)
{

}

bool ParametersBuilderModule::setData(ReportData *data)
{
    if (_parameters->size() == 0)
        return true;

    data->setValue(ID::Parameters::Count, QString::number(_parameters->size()));
    // YTA: These two fields are not in ParameterSet but in Drug now
//    data->setValue(ID::Parameters::Additive, QString::number(_parameters->globalAdditive()));
//    data->setValue(ID::Parameters::Proportional, QString::number(_parameters->globalProportional()));

    // ToDo ///////////////////////////////////////////////////////////////////////////////////////
    // When refactoring, find a better way to iterate a parameters set, if still used.
    // ////////////////////////////////////////////////////////////////////////////////////////////

    QStringList pids = _parameters->pids();
    pids.sort();

    for (int i = 0; i < _parameters->size(); ++i) {
        SharedParameter parameter = _parameters->get(pids.at(i));

        data->setValue(QString(ID::Parameters::Parameter::Identifier).arg(i), parameter->getName());
        data->setValue(QString(ID::Parameters::Parameter::Unit).arg(i), parameter->getQuantity()->unit().name());
        data->setValue(QString(ID::Parameters::Parameter::Additive).arg(i), QString::number(parameter->getBsv()->getStandard()));
        data->setValue(QString(ID::Parameters::Parameter::Proportional).arg(i), QString::number(parameter->getBsv()->getProportional()));

        // ToDo ///////////////////////////////////////////////////////////////////////////////////
        // When refactoring, find a better way to store all three values in the parameters.
        // ////////////////////////////////////////////////////////////////////////////////////////

        if (_parameters->type() == POPULATION)
            data->setValue(QString(ID::Parameters::Parameter::Population).arg(i), QString::number(parameter->getQuantity()->value()));
        if (_parameters->type() == APRIORI)
            data->setValue(QString(ID::Parameters::Parameter::Reference).arg(i), QString::number(parameter->getQuantity()->value()));
        if (_parameters->type() == APOSTERIORI)
            data->setValue(QString(ID::Parameters::Parameter::Patient).arg(i), QString::number(parameter->getQuantity()->value()));
    }

    return true;
}

void ParametersBuilderModule::unsetData(ReportData *data)
{
    int parametersCount = data->value(ID::Parameters::Count).toInt();

    for (int i = 0; i < parametersCount; ++i) {
        data->unsetValue(QString(ID::Parameters::Parameter::Identifier).arg(i));
        data->unsetValue(QString(ID::Parameters::Parameter::Unit).arg(i));
        data->unsetValue(QString(ID::Parameters::Parameter::Additive).arg(i));
        data->unsetValue(QString(ID::Parameters::Parameter::Proportional).arg(i));
        data->unsetValue(QString(ID::Parameters::Parameter::Population).arg(i));
        data->unsetValue(QString(ID::Parameters::Parameter::Reference).arg(i));
        data->unsetValue(QString(ID::Parameters::Parameter::Patient).arg(i));
    }

    data->unsetValue(ID::Parameters::Count);
    data->unsetValue(ID::Parameters::Additive);
    data->unsetValue(ID::Parameters::Proportional);
}

ParameterSet* ParametersBuilderModule::parameters() const
{
    return _parameters;
}

ParametersBuilderModule &ParametersBuilderModule::setParameters(ParameterSet* parameters)
{
    _parameters = parameters;
    return *this;
}
