#include "analystbuildermodule.h"
#include "reportdata.h"
#include "identifiers.h"
#include "dbobjects.h"

AUTO_PROPERTY_IMPL(AnalystBuilderModule, Practician*, practician, Practician)

AnalystBuilderModule::AnalystBuilderModule(QObject *parent)
{
}

bool AnalystBuilderModule::setData(ReportData *data)
{
    data->setValue(ID::Analyst::Id, QString::number(_practician->id()));
    data->setValue(ID::Analyst::External, _practician->externalId());
    data->setValue(ID::Analyst::Title, _practician->title());
    data->setValue(ID::Analyst::Firstname, _practician->person()->firstname());
    data->setValue(ID::Analyst::Lastname, _practician->person()->name());
    data->setValue(ID::Analyst::Gender, _practician->person()->gender() == Male ? "male" : "female");
    data->setValue(ID::Analyst::Birthdate, _practician->person()->birthday().toString(Qt::ISODate));
    data->setValue(ID::Analyst::Institute, _practician->institute()->name());
    data->setValue(ID::Analyst::Address, _practician->person()->location()->address());
    data->setValue(ID::Analyst::Postcode, _practician->person()->location()->postcode());
    data->setValue(ID::Analyst::City, _practician->person()->location()->city());
    data->setValue(ID::Analyst::Country, _practician->person()->location()->country());

    // ToDo ///////////////////////////////////////////////////////////////////////////////////////
    // When refactoring, find a better way to retrieve phones and emails through their type.
    // ////////////////////////////////////////////////////////////////////////////////////////////

    if (_practician->person()->phones().size() > 0)
        data->setValue(ID::Analyst::Phone, _practician->person()->phones().at(0)->number());
    if (_practician->person()->phones().size() > 1)
        data->setValue(ID::Analyst::Fax, _practician->person()->phones().at(1)->number());
    if (_practician->person()->emails().size() > 0)
        data->setValue(ID::Analyst::Email, _practician->person()->emails().at(0)->getEmail());

    Institute* _institute = _practician->institute();
    Location* location = _practician->person()->location();

    data->setValue(ID::Institute::Id, QString::number(_institute->id()));
    data->setValue(ID::Institute::External, _institute->externalId());
    data->setValue(ID::Institute::Name, _institute->name());
    data->setValue(ID::Institute::Address, location->address());
    data->setValue(ID::Institute::Postcode, location->postcode());
    data->setValue(ID::Institute::City, location->city());
    data->setValue(ID::Institute::Country, location->country());
    data->setImage(ID::Institute::Logo, _institute->logo());

    return true;
}

void AnalystBuilderModule::unsetData(ReportData *data)
{
    data->unsetValue(ID::Analyst::Id);
    data->unsetValue(ID::Analyst::External);
    data->unsetValue(ID::Analyst::Title);
    data->unsetValue(ID::Analyst::Firstname);
    data->unsetValue(ID::Analyst::Lastname);
    data->unsetValue(ID::Analyst::Gender);
    data->unsetValue(ID::Analyst::Birthdate);
    data->unsetValue(ID::Analyst::Institute);
    data->unsetValue(ID::Analyst::Address);
    data->unsetValue(ID::Analyst::Postcode);
    data->unsetValue(ID::Analyst::City);
    data->unsetValue(ID::Analyst::Country);
    data->unsetValue(ID::Analyst::Phone);
    data->unsetValue(ID::Analyst::Fax);
    data->unsetValue(ID::Analyst::Email);

    data->unsetValue(ID::Institute::Id);
    data->unsetValue(ID::Institute::External);
    data->unsetValue(ID::Institute::Name);
    data->unsetValue(ID::Institute::Address);
    data->unsetValue(ID::Institute::Postcode);
    data->unsetValue(ID::Institute::City);
    data->unsetValue(ID::Institute::Country);

    data->unsetImage(ID::Institute::Logo);
}
