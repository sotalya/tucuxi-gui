#include "treatmentbuildermodule.h"
#include "targetbuildermodule.h"
#include "patientbuildermodule.h"
#include "reportdata.h"
#include "identifiers.h"
#include "drugresponseanalysis.h"
#include "drugtreatment.h"
#include "drug.h"
#include "translatablestring.h"

using namespace Tucuxi::GuiCore;

TreatmentBuilderModule::TreatmentBuilderModule(DrugTreatment* treatment, QObject *parent) :
    _treatment(treatment)
{

}

bool TreatmentBuilderModule::setData(ReportData *data)
{
    if (_treatment == 0)
        return true;

    if (!TargetBuilderModule(&_treatment->getTargets()->getList()).setData(data)) {
        unsetData(data);
        return false;
    }

    data->setValue(ID::Treatment::Id, QString::number(_treatment->id()));
    data->setValue(ID::Treatment::Name, _treatment->getName());
    data->setValue(ID::Treatment::Drug, _treatment->getDrug()->getName()->value());

    return true;
}

void TreatmentBuilderModule::unsetData(ReportData *data)
{
//    TargetBuilderModule().unsetData(data);

    data->unsetValue(ID::Treatment::Id);
    data->unsetValue(ID::Treatment::Name);
    data->unsetValue(ID::Treatment::Drug);
}

const DrugTreatment* TreatmentBuilderModule::treatment() const
{
    return _treatment;
}

TreatmentBuilderModule &TreatmentBuilderModule::setTreatment(DrugTreatment* treatment)
{
    _treatment = treatment;
    return *this;
}
