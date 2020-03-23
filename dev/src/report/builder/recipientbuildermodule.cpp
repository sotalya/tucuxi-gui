#include "recipientbuildermodule.h"
#include "reportdata.h"
#include "identifiers.h"
#include "dbobjects.h"

AUTO_PROPERTY_IMPL(RecipientBuilderModule, Practician*, practician, Practician)
AUTO_PROPERTY_IMPL(RecipientBuilderModule, Institute*, institute, Institute)

RecipientBuilderModule::RecipientBuilderModule(QObject *parent)
{
}

bool RecipientBuilderModule::setData(ReportData *data)
{
    data->setValue(ID::Recipient::Id, QString::number(_practician->id()));
    data->setValue(ID::Recipient::External, _practician->externalId());
    data->setValue(ID::Recipient::Title, _practician->title());
    data->setValue(ID::Recipient::Firstname, _practician->person()->firstname());
    data->setValue(ID::Recipient::Lastname, _practician->person()->name());
    data->setValue(ID::Recipient::Gender, _practician->person()->gender() == Male ? "male" : "female");
    data->setValue(ID::Recipient::Birthdate, _practician->person()->birthday().toString(Qt::ISODate));
    data->setValue(ID::Recipient::Institute, _practician->institute()->name());
    data->setValue(ID::Recipient::Address, _practician->person()->location()->address());
    data->setValue(ID::Recipient::Postcode, _practician->person()->location()->postcode());
    data->setValue(ID::Recipient::City, _practician->person()->location()->city());
    data->setValue(ID::Recipient::Country, _practician->person()->location()->country());

    // ToDo ///////////////////////////////////////////////////////////////////////////////////////
    // When refactoring, find a better way to retrieve phones and emails through their type.
    // ////////////////////////////////////////////////////////////////////////////////////////////

    if (_practician->person()->phones().size() > 0)
        data->setValue(ID::Recipient::Phone, _practician->person()->phones().at(0)->number());
    if (_practician->person()->phones().size() > 1)
        data->setValue(ID::Recipient::Fax, _practician->person()->phones().at(1)->number());
    if (_practician->person()->emails().size() > 0)
        data->setValue(ID::Recipient::Email, _practician->person()->emails().at(0)->getEmail());

    return true;
}

void RecipientBuilderModule::unsetData(ReportData *data)
{
    data->unsetValue(ID::Recipient::Id);
    data->unsetValue(ID::Recipient::External);
    data->unsetValue(ID::Recipient::Title);
    data->unsetValue(ID::Recipient::Firstname);
    data->unsetValue(ID::Recipient::Lastname);
    data->unsetValue(ID::Recipient::Gender);
    data->unsetValue(ID::Recipient::Birthdate);
    data->unsetValue(ID::Recipient::Institute);
    data->unsetValue(ID::Recipient::Address);
    data->unsetValue(ID::Recipient::Postcode);
    data->unsetValue(ID::Recipient::City);
    data->unsetValue(ID::Recipient::Country);
    data->unsetValue(ID::Recipient::Phone);
    data->unsetValue(ID::Recipient::Fax);
    data->unsetValue(ID::Recipient::Email);
}
