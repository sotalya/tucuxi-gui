/* 
 * Tucuxi - Tucuxi-gui software. 
 * This software is able to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "dbobjects.h"
//#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"

namespace Tucuxi {
namespace Gui {
namespace Core {



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

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

