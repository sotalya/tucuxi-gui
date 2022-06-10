#include "concentrationsbuildermodule.h"
#include "reportdata.h"
#include "identifiers.h"

using namespace Tucuxi::Gui::Core::Core;

ConcentrationsBuilderModule::ConcentrationsBuilderModule(const QList<QPair<Amount, QDateTime> > &concentrations, QObject *parent) :
    _concentrations(concentrations)
{

}

bool ConcentrationsBuilderModule::setData(ReportData *data)
{
    if (_concentrations.size() == 0)
        return true;

    data->setValue(ID::Concentrations::Count, QString::number(_concentrations.size()));

    for (int i = 0; i < _concentrations.size(); ++i) {
        Amount concentration = _concentrations.at(i).first;
        QDateTime date       = _concentrations.at(i).second;

        data->setValue(QString(ID::Concentrations::Concentration::Value).arg(i), QString::number(concentration.value()));
        data->setValue(QString(ID::Concentrations::Concentration::Unit).arg(i), concentration.unit().toString());
        data->setValue(QString(ID::Concentrations::Concentration::Date).arg(i), date.toString(Qt::ISODate));
    }

    return true;
}

void ConcentrationsBuilderModule::unsetData(ReportData *data)
{
    int concentrationsCount = data->value(ID::Concentrations::Count).toInt();

    for (int i = 0; i < concentrationsCount; ++i) {
        data->unsetValue(QString(ID::Concentrations::Concentration::Value).arg(i));
        data->unsetValue(QString(ID::Concentrations::Concentration::Unit).arg(i));
        data->unsetValue(QString(ID::Concentrations::Concentration::Date).arg(i));
    }
    data->unsetValue(ID::Concentrations::Count);
}

QList<QPair<Amount, QDateTime> > ConcentrationsBuilderModule::concentrations() const
{
    return _concentrations;
}

ConcentrationsBuilderModule &ConcentrationsBuilderModule::setConcentrations(const QList<QPair<Amount, QDateTime> > &concentrations)
{
    _concentrations = concentrations;
    return *this;
}
