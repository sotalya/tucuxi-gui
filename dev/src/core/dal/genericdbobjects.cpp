/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
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


#include "genericdbobjects.h"
//#include "curvemanager.h"

namespace Tucuxi {
////Set the Location default values
//Location::Location()
//{
//    setId(-1);
//}

//Set the DBPhone default values
//Phone::Phone() : m_id(-1)
//{

//}

//Set the DBPhone default values and provided phone number and type
//Phone::Phone(QString type, QString number) : id(-1) , type(type), number(number)
//{

//}

bool Phone::operator==(const Phone &rhv)
{
    return m_number == rhv.number() && m_type == rhv.type();
}

//Set the DBInstitute default values
Institute::Institute() : m_id(-1), m_location(-1)
{

}

//Set the DBPerson default values
Person::Person() : m_id(-1), m_location(-1)
{

}

//Set the DBPatient default values
Patient::Patient() : m_id(-1), m_person(-1), m_stat_ok(false)
{

}


//Set the DBPractician default values
Practician::Practician() : m_id(-1), m_person(-1), m_institute(-1)
{

}

//Set the DBReport default values
Report::Report() : m_id(-1), m_curve(-1), m_analyst(-1), m_applicant(-1),
    m_recipient(-1), m_distributed(false), m_crtDate(QDateTime::currentDateTime()), m_refDate(QDateTime::currentDateTime())
{
    for (int i = 0; i < LAST_SECTION; i++)
        m_layout.append(sections(i));
}

//Set the DBCurve default values
Curve::Curve() : m_id(-1), m_patient(-1), /*m_cType(CurveManager::Continous)*/, m_pType(APOSTERIORI)
{

}

}
