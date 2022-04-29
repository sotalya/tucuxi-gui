//@@license@@

#include "genericdbobjects.h"
//#include "curvemanager.h"

namespace ezechiel {
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
