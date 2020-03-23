#include "patientbuildermodule.h"
#include "reportdata.h"
#include "identifiers.h"
#include "dbobjects.h"

AUTO_PROPERTY_IMPL(PatientBuilderModule, Patient*, patient, Patient)

PatientBuilderModule::PatientBuilderModule(Patient* patient, QObject *parent) :
    _patient(patient)
{
}

bool PatientBuilderModule::setData(ReportData *data)
{
    data->setValue(ID::Patient::Id, QString::number(_patient->id()));
    data->setValue(ID::Patient::External, _patient->externalId());
    data->setValue(ID::Patient::Room, _patient->stayNumber());
    data->setValue(ID::Patient::Approval, _patient->statOk() ? "yes" : "no");
    data->setValue(ID::Patient::Firstname, _patient->person()->firstname());
    data->setValue(ID::Patient::Lastname, _patient->person()->name());
    data->setValue(ID::Patient::Gender, _patient->person()->gender() == Male ? "male" : "female");
    data->setValue(ID::Patient::Birthdate, _patient->person()->birthday().toString(Qt::ISODate));
    data->setValue(ID::Patient::Address, _patient->person()->location()->address());
    data->setValue(ID::Patient::Postcode, _patient->person()->location()->postcode());
    data->setValue(ID::Patient::City, _patient->person()->location()->city());
    data->setValue(ID::Patient::Country, _patient->person()->location()->country());

    // ToDo ///////////////////////////////////////////////////////////////////////////////////////
    // When refactoring, find a better way to retrieve phones and emails through their type.
    // ////////////////////////////////////////////////////////////////////////////////////////////

    if (_patient->person()->phones().size() > 0)
        data->setValue(ID::Patient::Phone, _patient->person()->phones().at(0)->number());
    if (_patient->person()->phones().size() > 1)
        data->setValue(ID::Patient::Fax, _patient->person()->phones().at(1)->number());
    if (_patient->person()->emails().size() > 0)
        data->setValue(ID::Patient::Email, _patient->person()->emails().at(0)->getEmail());

    return true;
}

void PatientBuilderModule::unsetData(ReportData *data)
{
    data->unsetValue(ID::Patient::Id);
    data->unsetValue(ID::Patient::External);
    data->unsetValue(ID::Patient::Room);
    data->unsetValue(ID::Patient::Approval);
    data->unsetValue(ID::Patient::Firstname);
    data->unsetValue(ID::Patient::Lastname);
    data->unsetValue(ID::Patient::Gender);
    data->unsetValue(ID::Patient::Birthdate);
    data->unsetValue(ID::Patient::Address);
    data->unsetValue(ID::Patient::Postcode);
    data->unsetValue(ID::Patient::City);
    data->unsetValue(ID::Patient::Country);
    data->unsetValue(ID::Patient::Phone);
    data->unsetValue(ID::Patient::Fax);
    data->unsetValue(ID::Patient::Email);
}
