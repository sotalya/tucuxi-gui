#include "samplebuildermodule.h"
#include "reportdata.h"
#include "identifiers.h"

SampleBuilderModule::SampleBuilderModule(Measure * sample, QObject *parent) :
    _sample(sample)
{
}

bool SampleBuilderModule::setData(ReportData *data)
{

//    if (!_sample->isValid())
//        return true;
    Measure* measure = static_cast<Measure*>(_sample);
    data->setValue(ID::Sample::Id, QString::number(measure->id()));
    data->setValue(ID::Sample::External, measure->sampleID());
    data->setValue(ID::Sample::Value, QString::number(measure->getConcentration()->value()));
    data->setValue(ID::Sample::Unit, measure->getConcentration()->unit().toString());
    data->setValue(ID::Sample::Drug, measure->getSdrug());
    data->setValue(ID::Sample::Date, measure->getMoment().toString(Qt::ISODate));
    data->setValue(ID::Sample::Arrival, measure->arrivalDate().toString(Qt::ISODate));

    return true;
}

void SampleBuilderModule::unsetData(ReportData *data)
{
    data->unsetValue(ID::Sample::Id);
    data->unsetValue(ID::Sample::External);
    data->unsetValue(ID::Sample::Value);
    data->unsetValue(ID::Sample::Unit);
    data->unsetValue(ID::Sample::Drug);
    data->unsetValue(ID::Sample::Date);
    data->unsetValue(ID::Sample::Arrival);
}

Measure * SampleBuilderModule::sample() const
{
    return _sample;
}

SampleBuilderModule &SampleBuilderModule::setSample(Measure * sample)
{
    _sample = sample;
    return *this;
}
