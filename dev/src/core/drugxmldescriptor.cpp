//@@license@@

#include "core/drugxmldescriptor.h"

#include <QFile>

namespace ezechiel {
namespace GuiCore {


//Default contructor
DrugXmlDescriptor::DrugXmlDescriptor(const QString &drugId) : _drugId(drugId)
{

}

//Check if the drug descriptor is valid
bool DrugXmlDescriptor::isValid() const
{
    return !_drugId.isEmpty()          &&
           !_atc.isEmpty()         &&
           !_name.isEmpty()        &&
           !_domain.isEmpty()      &&
           !_study.isEmpty()       &&
           !_description.isEmpty() &&
           !_pkModelId.isEmpty()     &&
           QFile::exists(_filePath);
}

//Returns the drug ID
QString DrugXmlDescriptor::drugId() const
{
    return _drugId;
}

//Returns the drug file path
QString DrugXmlDescriptor::file() const
{
    return _filePath;
}

//Sets the drug file path
void DrugXmlDescriptor::setFile(const QString &filePath)
{
    _filePath = filePath;
}

//Returns the drug model ID
QString DrugXmlDescriptor::pkModelId() const
{
    return _pkModelId;
}

//Sets the drug model ID
void DrugXmlDescriptor::setDrugModelId(const QString &modelId)
{
    _drugModelId = modelId;
}

//Returns the Pk model ID
QString DrugXmlDescriptor::drugModelId() const
{
    return _drugModelId;
}

//Sets the Pk model ID
void DrugXmlDescriptor::setPkModelId(const QString &modelId)
{
    _pkModelId = modelId;
}

//Returns the drug ATC
QString DrugXmlDescriptor::atc() const
{
    return _atc;
}

//Sets the drug ATC
void DrugXmlDescriptor::setAtc(const QString &atc)
{
    _atc = atc;
}

//Returns the drug name
QString DrugXmlDescriptor::name() const
{
    return _name;
}

//Sets the drug name
void DrugXmlDescriptor::setName(const QString &name)
{
    _name = name;
}

//Returns the domain name
QString DrugXmlDescriptor::domainName() const
{
    return _domain;
}

//Sets the domain name
void DrugXmlDescriptor::setDomainName(const QString &domainName)
{
    _domain = domainName;
}

//Returns the study name
QString DrugXmlDescriptor::studyName() const
{
    return _study;
}

//Sets the study name
void DrugXmlDescriptor::setStudyName(const QString &studyName)
{
    _study = studyName;
}

//Returns the drug description
QString DrugXmlDescriptor::description() const
{
    return _description;
}

//Sets the drug description
void DrugXmlDescriptor::setDescription(const QString &description)
{
    _description = description;
}

} // namespace core
} // namespace ezechiel

