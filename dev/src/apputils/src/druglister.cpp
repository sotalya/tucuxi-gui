//#include <QDebug>

#include "druglister.h"
//#include "coremessagehandler.h"
#include "core/utils/xmltranslator.h"
#include "core/drugxmldescriptor.h"
#include "core/core.h"
#include "core/utils/xmlvalidator.h"
//#include "core_errors.h"
#include "core/utils/version.h"
#include "current_version.h"
#include "core/utils/tag.h"
#include "core/utils/logging.h"
#include "errors_apputils.h"


using namespace Tucuxi::Gui::Core;

namespace Tucuxi {
namespace Gui {
namespace AppUtils {

//The error messages
const char *const DrugLister::_DIRECTORY_UNREADABLE = QT_TR_NOOP_UTF8("The directory '%1' is unreadable");
const char *const DrugLister::_FILE_BAD_EXTENSION = QT_TR_NOOP_UTF8("The file '%1' is not an XML file");
const char *const DrugLister::_FILE_UNREADABLE = QT_TR_NOOP_UTF8("The file '%1' is unreadable");
const char *const DrugLister::_FILE_INVALID_XML = QT_TR_NOOP_UTF8("The XML file '%1' is invalid. At line %2: %3");
const char *const DrugLister::_FILE_CANNOT_OPEN = QT_TR_NOOP_UTF8("The XML file '%1' cannot be opened");
const char *const DrugLister::_DRUG_PARSING_ERROR = QT_TR_NOOP_UTF8("The XML file '%1' is invalid: %2 (at line %3, column %4)");
const char *const DrugLister::_DRUG_INVALID_DESC = QT_TR_NOOP_UTF8("The XML file '%1' description is incomplete or corrupted");
const char *const DrugLister::_DRUG_DUPLICATED = QT_TR_NOOP_UTF8("The XML file '%1' contains a duplicate of the drug ID '%2'");

//The shared strings
const char *const DrugLister::_IGNORING = QT_TR_NOOP_UTF8(", ignoring...");

//Constructor
DrugLister::DrugLister()
{
    // ToDo /////////////////////////////////////////////////////////
    // Add the user directories from the config when available.
    // //////////////////////////////////////////////////////////////

    // Define DRUGS_AS_RESOURCES for only proposing drugs in resource files
    // instead of in standard directories
#define DRUGS_AS_RESOURCES
#ifdef DRUGS_AS_RESOURCES
    // Modify the file core/resources/core.qrc for adding new drugs
    _directories.append(QDir(":/drugs"));
#endif // DRUGS_AS_RESOURCES
    //Set the default directories
    _directories.append(QDir(CORE->path(Core::Core::Drugs)));
    _directories.append(QDir(CORE->path(Core::Core::CustomDrugs)));

}

//Destructor
DrugLister::~DrugLister()
{
    resetDrugs();
}

//Scans all drug XML files
void DrugLister::scanDrugs()
{
    //Clear the drug XML descriptors
    resetDrugs();

    //Scan each directories
    foreach (QDir directory, _directories) {

        //Ignore unreadable directories
        if (!directory.isReadable()) {
            LOG(QtDebugMsg, NOEZERROR, tr(_DIRECTORY_UNREADABLE).arg(directory.absolutePath()) + tr(_IGNORING));
            continue;
        }

        //Scan the directory
        scanDirectory(directory);
    }
}

//Scans a drug XML file
const DrugXmlDescriptor *DrugLister::scanDrug(const QString &filePath)
{
    LOG(QtDebugMsg, NOEZERROR, tr("Scanning drug file '%1'").arg(filePath));

    //Validate the XML structure
    QScopedPointer<DrugXmlDescriptor> drug;

    if (!XMLVALIDATOR->validate(filePath, XmlValidator::Drug)) {
        LOG(QtWarningMsg, NOEZERROR, _error = tr(_FILE_INVALID_XML).arg(filePath, QString::number(XMLVALIDATOR->errorLine()), XMLVALIDATOR->errorMessage()) + tr(_IGNORING));
        return nullptr;
    }

    //Set up and open the file
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        LOG(QtWarningMsg, NOEZERROR, tr(_FILE_CANNOT_OPEN).arg(filePath) + tr(_IGNORING));
        return nullptr;
    }

    //Set the XML parser's device
    _parser.setDevice(&file);

    while (!_parser.hasError() && _parser.readNextStartElement()) {

        //Parse the <model> element
        if (_parser.name() == Tag::name(DrugTag::Root) && _parser.isStartElement())
            validateVersion();

        //Parse the <drug> element
        else if (_parser.name() == Tag::name(DrugTag::Drug) && _parser.isStartElement())
            drug.reset(buildDescriptor(filePath));

        //Skip other elements
        else
            _parser.skipCurrentElement();
    }

    //Position the parser at the end of the file for control purpose
    if (!_parser.hasError() && _parser.name() == Tag::name(DrugTag::Root) && !_parser.isStartElement())
        _parser.readNext();

    //Check the end of the document was reached, raise an error otherwise
    if (!_parser.hasError() && !_parser.atEnd())
        _parser.raiseError("did not reach the end of the document");

    //Check if an error occured
    if (_parser.hasError()) {
        LOG(QtWarningMsg, NOEZERROR, tr(_DRUG_PARSING_ERROR).arg(filePath).arg(_parser.errorString()).arg(_parser.lineNumber()).arg(_parser.columnNumber()) + tr(_IGNORING));
        return nullptr;
    }

    //Check if the descriptor is valid
    if (!drug->isValid()) {
        LOG(QtWarningMsg, NOEZERROR, tr(_DRUG_INVALID_DESC).arg(filePath) + tr(_IGNORING));
        return nullptr;
    }

    //Clear the XML parser's device
    _parser.clear();

    return drug.take();
}

//Returns a drug XML descriptor
const DrugXmlDescriptor *DrugLister::drug(const QString &drugModelId) const
{
    return _drugs.value(drugModelId);
}

//Returns the list of drug XML descriptors
QList<const DrugXmlDescriptor *> DrugLister::drugs() const
{
    return _drugs.values();
}

//Removes a drug from the lister
void DrugLister::removeDrug(const QString &drugModelId)
{
    QScopedPointer<const DrugXmlDescriptor> drug(_drugs.take(drugModelId));
}

//Returns the directories list
QList<QDir> DrugLister::directories() const
{
    return _directories;
}

//Resets the directories
void DrugLister::clearDirectories()
{
    //Clear the directories list
    _directories.clear();
}

//Adds a directory to the list
bool DrugLister::addDirectory(const QDir &directory)
{
    //Check if the directory is not already included
    if (_directories.contains(directory))
        return true;

    //Add the directory to the list
    _directories.append(directory);

    return true;
}

//Removes a directory from the list
bool DrugLister::removeDirectory(const QDir &directory)
{
    //Get the directory index
    int index = _directories.indexOf(directory);

    //Abord if not found
    if (index == -1)
        return false;

    //Remove the directory
    _directories.removeAt(index);

    return true;
}

//Returns the last error message
QString DrugLister::errorMessage() const
{
    return _error;
}

//Scans a directory recursively
void DrugLister::scanDirectory(const QDir &directory)
{
    LOG(QtDebugMsg, NOEZERROR, tr("Scanning directory '%1'").arg(directory.absolutePath()));

    //For each files in the directory
    foreach (QFileInfo entry, directory.entryInfoList(QDir::Files)) {
        const DrugXmlDescriptor *drug;

        //Ignore non-XML files
        if (entry.suffix().toLower() != "xml") {
            LOG(QtWarningMsg, NOEZERROR, tr(_FILE_BAD_EXTENSION).arg(entry.absoluteFilePath()) + tr(_IGNORING));
            continue;
        }

        //Ignore unreadable files
        if (!entry.isReadable()) {
            LOG(QtWarningMsg, NOEZERROR, tr(_FILE_UNREADABLE).arg(entry.absoluteFilePath()) + tr(_IGNORING));
            continue;
        }

        //Scan the current drug XML file
        if (!(drug = scanDrug(entry.filePath())))
            continue;

        //Check if the drug does not already exists
        if (_drugs.contains(drug->drugModelId())) {
            LOG(QtWarningMsg, NOEZERROR, tr(_DRUG_DUPLICATED).arg(entry.filePath(), drug->drugId()) + tr(_IGNORING));
            continue;
        }

        //Register the drug XML descriptor
        _drugs.insert(drug->drugModelId(), drug);
    }

    //Recursively scan each subdirectories
    foreach (QFileInfo subdir, directory.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot))
        scanDirectory(QDir(subdir.absoluteFilePath()));
}

//Validate thes current file version
void DrugLister::validateVersion()
{
    //Get the current drug file's version
    QString version(_parser.attributes().value(Tag::attribute(DrugTag::Version)).toString());

    //Raise an error if it doesn't match
    if (Version(version) != Version(DRUG_VERSION))
        _parser.raiseError(tr("invalid drug XML file version '%1', was expecting '%2')").arg(version, DRUG_VERSION));
}

//Registers the current file drug
DrugXmlDescriptor *DrugLister::buildDescriptor(const QString &filePath)
{
    //The data to be extracted
    QString atc, drugId, drugModelId, pkModelId, name, domain, study, desc;

    //Create a XML translator
    XmlTranslator translator;

    /* Parse the <drug> element's childs. We will leave the while as soon as an error is
     * raised, or when the readNextStartElement() function encounter the </drug> tag. In
     * this case, the new current element will be the <drug> element's parent, <models>. */
    while (!_parser.hasError() && _parser.readNextStartElement()) {
        if (_parser.name() == Tag::name(DrugTag::Head)) {

            /* Parse the <head> element's childs. We will leave the while as soon as an error is
             * raised, or when the readNextStartElement() function encounter the </head> tag. In
             * this case, the new current element will be the <head> element's parent, <drug>. */
            while (!_parser.hasError() && _parser.readNextStartElement()) {
                if (_parser.name() == Tag::name(DrugTag::ATC)) {
                    atc = _parser.readElementText();
                } else if (_parser.name() == Tag::name(DrugTag::DrugId)) {
                    drugId = _parser.readElementText();
                } else if (_parser.name() == Tag::name(DrugTag::DrugModelId)) {
                    drugModelId = _parser.readElementText();
                } else if (_parser.name() == Tag::name(DrugTag::PkModelId)) {
                    pkModelId = _parser.readElementText();
                } else if (_parser.name() == Tag::name(DrugTag::DrugNames)) {
                    name = translator.translate(_parser, Tag::attribute(DrugTag::Language));
                } else if (_parser.name() == Tag::name(DrugTag::DomainNames)) {
                    domain = translator.translate(_parser, Tag::attribute(DrugTag::Language));
                } else if (_parser.name() == Tag::name(DrugTag::StudyNames)) {
                    study = translator.translate(_parser, Tag::attribute(DrugTag::Language));
                } else if (_parser.name() == Tag::name(DrugTag::Descriptions)) {
                    desc = translator.translate(_parser, Tag::attribute(DrugTag::Language));
                } else {
                    _parser.skipCurrentElement();
                }
            }

        } else {
            _parser.skipCurrentElement();
        }
    }

    //If the text is still empty, raise an error
    if (name.isEmpty()) {
        _parser.raiseError(tr("empty drug name"));
        return nullptr;
    }

    //Create a new drug XML descriptor
    DrugXmlDescriptor *drug = new DrugXmlDescriptor(drugId);
    drug->setFile(filePath);
    drug->setDrugModelId(drugModelId);
    drug->setPkModelId(pkModelId);
    drug->setAtc(atc);
    drug->setName(name);
    drug->setDomainName(domain);
    drug->setStudyName(study);
    drug->setDescription(desc);

    //Return the new drug XML descriptor
    return drug;
}

//Deletes the existing drug XML descriptors
void DrugLister::resetDrugs()
{
    QMap<QString, const DrugXmlDescriptor *>::iterator i = _drugs.begin();

    //Delete all the XML descriptors
    while(i != _drugs.end()){
        delete i.value();
        ++i;
    }

    //Clear the map
    _drugs.clear();
}

} //namespace AppUtils
} //namespace Gui
} //namespace Tucuxi
