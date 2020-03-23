#include "teambuildermodule.h"
#include "reportdata.h"
#include "identifiers.h"
#include "dbobjects.h"

AUTO_PROPERTY_IMPL(TeamBuilderModule, Practician*, practician, Practician)

TeamBuilderModule::TeamBuilderModule(Practician* practician, QObject *parent) :
    _practician(practician)
{

}

bool TeamBuilderModule::setData(ReportData *data)
{
    if (_practician == 0) {return true;}

    data->setValue(QString(ID::Team::Member::Id).arg(0), QString::number(_practician->id()));
    data->setValue(QString(ID::Team::Member::External).arg(0), _practician->externalId());
    data->setValue(QString(ID::Team::Member::Title).arg(0), _practician->title());
    data->setValue(QString(ID::Team::Member::Firstname).arg(0), _practician->person()->firstname());
    data->setValue(QString(ID::Team::Member::Lastname).arg(0), _practician->person()->name());
    data->setValue(QString(ID::Team::Member::Gender).arg(0), _practician->person()->gender() == Male ? "male" : "female");
    data->setValue(QString(ID::Team::Member::Birthdate).arg(0), _practician->person()->birthday().toString(Qt::ISODate));
    data->setValue(QString(ID::Team::Member::Institute).arg(0), _practician->institute()->name());
    data->setValue(QString(ID::Team::Member::Address).arg(0), _practician->person()->location()->address());
    data->setValue(QString(ID::Team::Member::Postcode).arg(0), _practician->person()->location()->postcode());
    data->setValue(QString(ID::Team::Member::City).arg(0), _practician->person()->location()->city());
    data->setValue(QString(ID::Team::Member::Country).arg(0), _practician->person()->location()->country());

    // ToDo ///////////////////////////////////////////////////////////////////////////////////
    // When refactoring, find a better way to retrieve phones and emails through their type.
    // ////////////////////////////////////////////////////////////////////////////////////////

    if (_practician->person()->phones().size() > 0)
        data->setValue(QString(ID::Team::Member::Phone).arg(0), _practician->person()->phones().at(0)->number());
    if (_practician->person()->phones().size() > 1)
        data->setValue(QString(ID::Team::Member::Fax).arg(0), _practician->person()->phones().at(1)->number());
    if (_practician->person()->emails().size() > 0)
        data->setValue(QString(ID::Team::Member::Email).arg(0), _practician->person()->emails().at(0)->getEmail());

    data->setValue(ID::Team::Count, QString::number(0));

    return true;
}

void TeamBuilderModule::unsetData(ReportData *data)
{
    data->unsetValue(QString(ID::Team::Member::Id).arg(0));
    data->unsetValue(QString(ID::Team::Member::External).arg(0));
    data->unsetValue(QString(ID::Team::Member::Title).arg(0));
    data->unsetValue(QString(ID::Team::Member::Firstname).arg(0));
    data->unsetValue(QString(ID::Team::Member::Lastname).arg(0));
    data->unsetValue(QString(ID::Team::Member::Gender).arg(0));
    data->unsetValue(QString(ID::Team::Member::Birthdate).arg(0));
    data->unsetValue(QString(ID::Team::Member::Institute).arg(0));
    data->unsetValue(QString(ID::Team::Member::Address).arg(0));
    data->unsetValue(QString(ID::Team::Member::Postcode).arg(0));
    data->unsetValue(QString(ID::Team::Member::City).arg(0));
    data->unsetValue(QString(ID::Team::Member::Country).arg(0));
    data->unsetValue(QString(ID::Team::Member::Phone).arg(0));
    data->unsetValue(QString(ID::Team::Member::Fax).arg(0));
    data->unsetValue(QString(ID::Team::Member::Email).arg(0));
    data->unsetValue(ID::Team::Count);
}
