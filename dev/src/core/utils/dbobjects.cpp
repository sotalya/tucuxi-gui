//@@license@@

#include "dbobjects.h"
//#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"

namespace ezechiel {
namespace core {


//Set the Location default values
//Location::Location() : _id(invalid_ident)
//{

//}

//Set the DBPhone default values
/*DBPhone::DBPhone() : id(invalid_ident)
{

}

//Set the DBPhone default values and provided phone number and type
DBPhone::DBPhone(QString type, QString number) : id(invalid_ident) , type(type), number(number)
{

}

bool DBPhone::operator==(const DBPhone &rhv)
{
    return number == rhv.number && type == rhv.type;
}

//Set the DBInstitute default values
DBInstitute::DBInstitute() : id(invalid_ident), location(invalid_ident)
{

}

//Set the DBPerson default values
DBPerson::DBPerson() : _id(invalid_ident), _locationId(invalid_ident)
{

}

//Set the DBPatient default values
//DBPatient::DBPatient() : _id(invalid_ident), _personId(invalid_ident), _stat_ok(false)
//{

//}

//Set the DBPractician default values
//DBPractician::DBPractician() : _id(invalid_ident), _personId(invalid_ident), _institute(invalid_ident)
//{

//}
*/
//Set the DBReport default values
DBReport::DBReport() : id(invalid_ident), curve(invalid_ident), analyst(invalid_ident), applicant(invalid_ident),
    recipient(invalid_ident), distributed(false), crtDate(QDateTime::currentDateTime()), refDate(QDateTime::currentDateTime())
{
    for (int i = 0; i < LAST_SECTION; i++)
        layout.append(sections(i));
}

//Set the DBCurve default values
DBCurve::DBCurve() : id(invalid_ident), patient(invalid_ident), cType(0), pType(0)
{

}

} // namespace core
} // namespace ezechiel

