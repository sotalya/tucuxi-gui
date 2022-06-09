#include "dosagebuildermodule.h"
#include "reportdata.h"
#include "identifiers.h"

using namespace ezechiel::GuiCore;

DosageBuilderModule::DosageBuilderModule(Dosage *dosage, QObject *parent) :
    _dosage(dosage)
{

}

bool DosageBuilderModule::setData(ReportData *data)
{
    if (!_dosage->isValid())
        return true;

    data->setValue(ID::Dosage::Id, QString::number(_dosage->id()));
    data->setValue(ID::Dosage::Take::First, _dosage->getApplied().toString(Qt::ISODate));
    data->setValue(ID::Dosage::Dose::Value, QString::number(_dosage->getQuantity()->value()));
    data->setValue(ID::Dosage::Dose::Unit, _dosage->getQuantity()->unit().toString());
    data->setValue(ID::Dosage::Interval::Value, QString::number(_dosage->getInterval().toHours()));
    data->setValue(ID::Dosage::Interval::Unit, "h");

    // ToDo ///////////////////////////////////////////////////////////////////////////////////////
    // When refactoring, find a generic way to deal with infusion time and other exta data.
    // ////////////////////////////////////////////////////////////////////////////////////////////

    if (!_dosage->getTinf().isEmpty() || !_dosage->getTinf().isNegative()) {
        data->setValue(ID::Dosage::Infusion::Value, QString::number(_dosage->getTinf().toHours()));
        data->setValue(ID::Dosage::Infusion::Unit, "h");
    }

    return true;
}

void DosageBuilderModule::unsetData(ReportData *data)
{
    data->unsetValue(ID::Dosage::Id);
    data->unsetValue(ID::Dosage::Take::First);
    data->unsetValue(ID::Dosage::Dose::Value);
    data->unsetValue(ID::Dosage::Dose::Unit);
    data->unsetValue(ID::Dosage::Interval::Value);
    data->unsetValue(ID::Dosage::Interval::Unit);
    data->unsetValue(ID::Dosage::Infusion::Value);
    data->unsetValue(ID::Dosage::Infusion::Unit);
}

Dosage* DosageBuilderModule::dosage() const
{
    return _dosage;
}

DosageBuilderModule &DosageBuilderModule::setDosage(Dosage* &dosage)
{
    _dosage = dosage;
    return *this;
}
