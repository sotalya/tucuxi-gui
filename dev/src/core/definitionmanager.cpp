//@@license@@

#include <QMultiMap>
#include <QDebug>
#include <QDir>


#include "core/definitionmanager.h"

#include "core/utils/ezxmlreader.h"
#include "core/utils/ezutils.h"
#include "core/core.h"
//#include "coremessagehandler.h"
#include "core/utils/units.h"
#include "core/utils/version.h"
#include "current_version.h"
#include "core/utils/logging.h"
#include "core/errors_core.h"

namespace Tucuxi {
namespace GuiCore {

const char *const DefinitionManager::_dateTimeFormat    = "dd/MM/yyyy-HH:mm";
const char *const DefinitionManager::_xmlDateTimeFormat = "yyyy'-'MM'-'dd'T'HH:mm:ss";


//Read the XML file and fill the internal variables
DefinitionManager::DefinitionManager()
{
    //Set XML structure, attributes and tags
    setXMLStructure();
    setAttributes();
    setIgnoredTags();

    //Set the default language
    _language = CORE->language();

    //Load the definitions
    loadDefinitions();
}

//Returns the list of intakes descriptors
QList<Descriptor> DefinitionManager::intakes() const
{
    return _intakes.values();
}

//Returns a specific intake descriptor
Descriptor DefinitionManager::intake(const QString id) const
{
    return _intakes.value(id);
}

//Returns true if the intake exists
bool DefinitionManager::validateIntake(const QString id) const
{
    return _intakes.contains(id);
}

//Returns true if the intake if of infusion type
bool DefinitionManager::isInfusion(const QString id) const
{
    return id == "infu";
}

//Returns the list of targets descriptors
QList<Descriptor> DefinitionManager::targets() const
{
    return _targets.values();
}

//Returns a specific target descriptor
//Descriptor DefinitionManager::target(const QString id) const
//{
//    return _targets.value(id);
//}

//Returns a target type
int DefinitionManager::target(const QString &id) const
{
    if (id == "peak")
        return 1;
    else if (id == "mean")
        return 2;
    else if (id == "residual")
        return 0;
    else
        return 3;
}

//Returns true if the target exists
bool DefinitionManager::validateTarget(const QString &id) const
{
    return _targets.contains(id);
}

bool DefinitionManager::registerCovariate(const QString &drugId, const QString &covariateId, const QString &covariateName, const QString &covariateDesc, const Unit &unit, const QMetaType::Type type, double value)
{
    //Check if the drug ID exists
    if (drugId.isEmpty())
        return false;

    //Check if the covariate already exists
    if (covariateExists(covariateName) && !isDuplicate(covariateId, unit, type))
        return false;

    //Check if the name exists
    if (covariateName.isEmpty())
        return false;

    //Check if the unit is valid
    if (!unit.isValid())
        return false;

    //Check if the type is valid
    if (type == QMetaType::Void)
        return false;

    //Create the covariate description
    Descriptor desc;
    desc.id = covariateId;
    desc.name = covariateName;
    desc.description = covariateDesc;    

    //Register the current covariate
    _covariatesDesc.insert(desc.name, desc);
    _covariatesUnit.insert(desc.name, unit);
    _covariatesType.insert(desc.name, type);
    _covariatesValue.insert(desc.name, value);

    //Register the covariate for this drug
    _covariatesGroup[drugId].append(desc);

    return true;
}

// ToDo: remove when refactored ////////////////////////////////////
QList<Descriptor> DefinitionManager::covariatesDesc(QString drugId) const
{
    return _covariatesGroup.value(drugId);
}

QList<Descriptor> DefinitionManager::clinicals() const
{
    return _clinicalsType.values();
}
Descriptor DefinitionManager::covariateDesc(const QString &id) const
{
    return _covariatesDesc.value(id);
}
Descriptor DefinitionManager::clinical(const QString &id) const
{
    return _clinicalsType.value(id);
}
bool DefinitionManager::covariateExists(const QString &id) const
{
    return _covariatesDesc.contains(id);
}
bool DefinitionManager::isDuplicate(const QString &covariateId, const Unit &unit, const QMetaType::Type type)
{
    if (!covariateExists(covariateId))
        return false;

    return _covariatesUnit.value(covariateId) == unit && _covariatesType.value(covariateId) == type;
}
bool DefinitionManager::clinicalExists(const QString &id) const
{
    return _clinicalsType.contains(id);
}
Unit DefinitionManager::covariateUnit(const QString &id) const
{
    return _covariatesUnit.value(id);
}
QMetaType::Type DefinitionManager::covariateType(const QString &id) const
{
    return _covariatesType.value(id);
}

double DefinitionManager::covariateValue(const QString &id) const
{
    return _covariatesValue.value(id, -1.0);
}

QString DefinitionManager::covariateDefaultValue(const QString &id) const
{
    // ToDo ///////////////////////////////////////////////////////////////////////////////////////
    // Below, we need to make a match between the covariate default value and the covariate possi-
    // ble values, if any. This is not possible currently because of the sex, whose default value
    // is 0.5 and therefore not usable as an index in the possible values list. Remove the two
    // line of code below when this issue is addressed.
    //
    // It might also be a good idea to rename this function to covariateDisplayValue and to add a
    // value parameter. We would then return a QString using the same algorithm as below.
    // ////////////////////////////////////////////////////////////////////////////////////////////

    if (id.split(".", QString::SkipEmptyParts).last() == "sex")
        return tr("Undefined");

    //Get the covariate list of values
    QStringList values = covariateValuesList(id);

    //Get the covariate default value
    double defaultValue = covariateValue(id);

    if (values.isEmpty() || (int)defaultValue >= values.size() || (int)defaultValue < 0)
        return QString::number(defaultValue);
    else
        return values.at((int)defaultValue);
}

QStringList DefinitionManager::covariateValuesList(const QString &id) const
{
    QStringList values;

    // ToDo ///////////////////////////////////////////////////////////////////////////////////////
    // Below, we only hardcoded the GIST and gender cases. In the future, the values for all cova-
    // riates will need to be extracted from the definition.xml and drug xml files.
    // ////////////////////////////////////////////////////////////////////////////////////////////

    if (id != "") {
        if (id.split(".", QString::SkipEmptyParts).last() == "sex")
            return values << tr("Female") << tr("Male");
        if (id.split(".", QString::SkipEmptyParts).last() == "gist")
            return values << tr("No") << tr("Yes");

    }

    return values;
}
////////////////////////////////////////////////////////////////////

//Returns the list of default intervals
QList<Tucuxi::GuiCore::Duration> DefinitionManager::intervals() const
{
    return _intervals;
}

//Returns the currently used language
QString DefinitionManager::language() const
{
    return _language;
}

//Set a new language
void DefinitionManager::setLanguage(const QString language)
{
    //Set the new current language
    _language = language;

    //Reload the definitions
    loadDefinitions();
}

//Sets drugs XML files structure
void DefinitionManager::setXMLStructure()
{
    //Structure
    QMultiMap<QString,QString> structure;
    structure.insert("definitions", "root");
        structure.insert("intakes", "definitions");
            structure.insert("intake", "intakes");
                structure.insert("id", "intake");
                structure.insert("names", "intake");
                    structure.insert("name", "names");
                structure.insert("descriptions", "intake");
                    structure.insert("description", "descriptions");
        structure.insert("targets", "definitions");
            structure.insert("target", "targets");
                structure.insert("id", "target");
                structure.insert("names", "target");
                    structure.insert("name", "names");
                structure.insert("descriptions", "target");
                    structure.insert("description", "descriptions");
        structure.insert("covariates", "definitions");
            structure.insert("covariate", "covariates");
                structure.insert("id", "covariate");
                structure.insert("names", "covariate");
                    structure.insert("name", "names");
                structure.insert("descriptions", "covariate");
                    structure.insert("description", "description");
                structure.insert("unit", "covariate");
                structure.insert("type", "covariate");
                structure.insert("defaultvalue", "covariate");
        structure.insert("clinicals", "definitions");
            structure.insert("clinical", "clinicals");
                structure.insert("id", "clinical");
                structure.insert("names", "clinical");
                    structure.insert("name", "names");
                structure.insert("descriptions", "clinical");
                    structure.insert("description", "description");
        structure.insert("intervals", "definitions");
            structure.insert("interval", "intervals");
            structure.insert("all", "intervals");

    //Set the structure in the XML reader
    _xmlReader.setStructure(structure);
}

//Sets tags attributes
void DefinitionManager::setAttributes()
{
    //Attributes
    QMultiMap<QString,QString> attributes;
    attributes.insert("definitions", "version");
    attributes.insert("name", "lang");
    attributes.insert("description", "lang");

    //Set the attributes
    _xmlReader.setTagAttributes(attributes);
}

//Sets ignored tags
void DefinitionManager::setIgnoredTags()
{

}

//Load the definitions
bool DefinitionManager::loadDefinitions()
{
//    qDebug() << "Scanning " + CORE->path(Core::ApplicationData) + "/definitions.xml";

    //Remove the existing data
    _intakes.clear();
    _intervals.clear();
    _covariatesDesc.clear();
    _covariatesUnit.clear();
    _covariatesType.clear();
    _covariatesValue.clear();
    _clinicalsType.clear();

    //Set the language to use
    _xmlReader.setLanguage(_language);

    //Set the definitions file in the XML reader
    QString fileName = CORE->path(Core::ApplicationData) + "/definitions.xml";
    if (!_xmlReader.setFile(fileName))
    {
        LOG(QtCriticalMsg, XMLERROR, tr("Error opening definitions.xml, aborting..."));
        return false;
    }

    //Scan the file and abort if an error occured
    if (!_xmlReader.readXML())
    {
        LOG(QtCriticalMsg, XMLERROR, tr("Error while scanning definitions.xml, aborting..."));
        return false;
    }

    //Set up the necessery child tags
    EzTag *root = _xmlReader.result()->childs.value("definitions");
    EzTag *intakes = root->childs.value("intakes");
    EzTag *targets = root->childs.value("targets");
    EzTag *covariates = root->childs.value("covariates");
    EzTag *intervals = root->childs.value("intervals");
    EzTag *clinicals = root->childs.value("clinicals");

    //Verify existence of the intakes tag and abort if missing
    if (!intakes)
    {
        LOG(QtCriticalMsg, XMLFORMATERROR, tr("Error in definitions file: missing <intakes> tag, aborting..."));
        return false;
    }

    //Verify existence of the targets tag and abort if missing
    if (!targets)
    {
        LOG(QtCriticalMsg, XMLFORMATERROR, tr("Error in definitions file: missing <targets> tag, aborting..."));
        return false;
    }

    //Verify existence of the covariates tag and abort if missing
    if (!covariates)
    {
        LOG(QtCriticalMsg, XMLFORMATERROR, tr("Error in definitions file: missing <covariates> tag, aborting..."));
        return false;
    }

    //Verify existence of the intervals tag and abort if missing
    if (!intervals)
    {
        LOG(QtCriticalMsg, XMLFORMATERROR, tr("Error in definitions file: missing <intervals> tag, aborting..."));
        return false;
    }

    //Test the definitions file version
    if (Version(root->attributes.value("version")) != Version(DEFINITIONS_VERSION))
    {
        LOG(QtCriticalMsg, VERSIONERROR, tr("The definitions.xml file has an invalid version ('%2', waited '%3')").arg(root->attributes.value("version"), DEFINITIONS_VERSION));
        return false;
    }

    //Register the intakes
    if (!registerIntakes(intakes))
    {
        LOG(QtCriticalMsg, XMLFORMATERROR, tr("Error in definitions file: invalid intake, aborting..."));
        return false;
    }

    //Register the targets
    if (!registerTargets(targets))
    {
        LOG(QtCriticalMsg, XMLFORMATERROR, tr("Error in definitions file: invalid target, aborting..."));
        return false;
    }

    //Register the covariates
    if (!registerCovariates(covariates))
    {
        LOG(QtCriticalMsg, XMLFORMATERROR, tr("Error in definitions file: invalid covariate, aborting..."));
        return false;
    }

    //Register the intervals
    if (!registerIntervals(intervals))
    {
        LOG(QtCriticalMsg, XMLFORMATERROR, tr("Error in definitions file: invalid interval, aborting..."));
        return false;
    }

    if(!registerClinicals(clinicals))
    {
        LOG(QtCriticalMsg, XMLFORMATERROR, tr("Error in definitions file: invalid clinical data, aborting..."));
        return false;
    }

    //Delete the XML reader tree
    _xmlReader.deleteTree();

    return true;
}

//Register the intakes
bool DefinitionManager::registerIntakes(const EzTag *root)
{
    //Temporary tag used for controls
    EzTag *current;

    //Temporary intake descriptor
    Descriptor desc;

    //For each available intakes
    foreach(EzTag *tag, root->childs.values()) {

        //Fetch the current intake ID
        if (!(current = tag->childs.value("id")))
            return false;
        desc.id = current->content;

        //Fetch the current intake name
        if (!(current = tag->childs.value("names")) || !(current = current->childs.value("name")))
            return false;
        desc.name = current->content;

        //Fetch the current intake description
        if (!(current = tag->childs.value("descriptions")) || !(current = current->childs.value("description")))
            return false;
        desc.description = current->content;

        //Register the current intake
        _intakes.insert(desc.id, desc);
    }

    return true;
}

//Register the targets
bool DefinitionManager::registerTargets(const EzTag *root)
{
    //Temporary tag used for controls
    EzTag *current;

    //Temporary target descriptor
    Descriptor desc;

    //For each available targets
    foreach(EzTag *tag, root->childs.values()) {

        //Fetch the current target ID
        if (!(current = tag->childs.value("id")))
            return false;
        desc.id = current->content;

        //Fetch the current target name
        if (!(current = tag->childs.value("names")) || !(current = current->childs.value("name")))
            return false;
        desc.name = current->content;

        //Fetch the current target description
        if (!(current = tag->childs.value("descriptions")) || !(current = current->childs.value("description")))
            return false;
        desc.description = current->content;

        //Register the current target
        _targets.insert(desc.id, desc);
    }

    return true;
}

//Register the covariates
bool DefinitionManager::registerCovariates(const EzTag *root)
{
    // ToDo ////////////////////////////////////
    // Verify the info as in registerCovariate()
    ////////////////////////////////////////////

    //Temporary tag used for controls
    EzTag *current;

    //Temporary covariate data
    Descriptor desc;

    //For each available covariate
    foreach(EzTag *tag, root->childs.values()) {

        //Fetch the current covariate ID
        if (!(current = tag->childs.value("id")))
            return false;
        desc.id = current->content;

        //Fetch the current covariate name
        if (!(current = tag->childs.value("names")) || !(current = current->childs.value("name")))
            return false;
        desc.name = current->content;

        //Fetch the current covariate description
        if (!(current = tag->childs.value("descriptions")) || !(current = current->childs.value("description")))
            return false;
        desc.description = current->content;

        //Fetch the current covariate unit
        if (!(current = tag->childs.value("unit")))
            return false;
        Unit unit(current->content);

        //Fetch the current covariate type
        if (!(current = tag->childs.value("type")))
            return false;
        QMetaType::Type type = QMetaType::Type(QMetaType::type(qPrintable(current->content)));

        //Fetch the current covariate default value
        if (!(current = tag->childs.value("defaultvalue")))
            return false;
        double value = current->content.toDouble();

        //Register the current covariate
        _covariatesDesc.insert(desc.name, desc);
        _covariatesUnit.insert(desc.name, unit);
        _covariatesType.insert(desc.name, type);
        _covariatesValue.insert(desc.name, value);

        //Mark the covariate as global
        _covariatesGroup[""].append(desc);
    }
    return true;
}

//Register the clinicals
bool DefinitionManager::registerClinicals(const EzTag *root)
{
    //Temporary tag used for controls
    EzTag *current;

    //Temporary clinical data
    Descriptor desc;

    //For each available clinical
    foreach(EzTag *tag, root->childs.values()) {

        //Fetch the current clinical ID
        if (!(current = tag->childs.value("id")))
            return false;
        desc.id = current->content;

        //Fetch the current clinical name
        if (!(current = tag->childs.value("names")) || !(current = current->childs.value("name")))
            return false;
        desc.name = current->content;

        //Fetch the current clinical description
        if (!(current = tag->childs.value("descriptions")) || !(current = current->childs.value("description")))
            return false;
        desc.description = current->content;


        //Register the current covariate
        _clinicalsType.insert(desc.id, desc);
    }

    return true;
}

//Register the intervals
bool DefinitionManager::registerIntervals(const EzTag *root)
{
    //For each available interval
    foreach(EzTag *tag, root->childs.values("interval"))
        _intervals.append(Duration(tag->content));

    return true;
}

} // namespace core
} // namespace Tucuxi

