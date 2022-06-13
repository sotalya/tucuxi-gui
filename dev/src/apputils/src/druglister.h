#ifndef DRUGLISTER_H
#define DRUGLISTER_H

#include <QDir>
#include <QMap>
#include <QCoreApplication>
#include <QXmlStreamReader>

namespace Tucuxi {
    namespace Gui {
    namespace Core {

        class DrugXmlDescriptor;
    }
    }
}

/** \defgroup drugxmlfiles Drug XML Files
 *  \ingroup core
 *
 * EzeCHieL uses drug XML files in order to define the drugs used in the software. At
 * startup, EzeCHieL will search the default and preferred drugs directories and build
 * a drug for each valid XML file found.
 *
 * EzeCHieL provides an XML editor in order to edit the existing drug XML files, or
 * create new ones. The drugs XML files provided by EzeCHieL are certified (signed)
 * by the EzeCHieL group.
 *
 * \sa DrugLister, DrugXmlDescriptor, DrugManager, Drug
 */

/** \ingroup drugxmlfiles
 *  \brief A light parser used to list the drugs available.
 *
 * This class is used to search the default drugs directories and scan the drug XML files
 * they contain. For each drug XML file, it will construct an adequate DrugXmlDescriptor
 * object that can then be used to display basic information about the drug and retrieve
 * the corresponding file. To be considered valid, a drug XML file must be validated against
 * the EzeCHieL drug XSD schema, and its verion must match the current drug version.
 *
 * The drug lister is using a QXmlStreamReader in order to extract only what it needs (the
 * drug ID, names and descriptions, as well as the drug version). It can therefore parse
 * a lot of drug files in a minimal amount of time. It is also possible to add or remove
 * directories from the default list.
 *
 * \sa DrugXmlDescriptor
 */


namespace Tucuxi {
namespace Gui {
namespace AppUtils {

class DrugLister
{
    Q_DECLARE_TR_FUNCTIONS(DrugLister)

public:

    //! Constructor.
    DrugLister();

    //! Destructor.
    /** Destructs the drug XML descriptors previous built by the lister.
     */
    ~DrugLister();

    //! Scan the drugs.
    /** Scan the drugs in the specified directories. The previous drug XML descriptors will
     * be deleted.
     */
    void scanDrugs();

    //! Scan a drug XML file and return the corresponding drug XML descriptor.
    /** Scan a drug XML file and return the corresponding drug XML descriptor, or an null pointer
     * if an error occured. The error message can be recovered by calling the errorMessage()
     * function. This function will also validate the XML file in accordance with the drugs XML
     * schema.
     * @param filePath The absolute path to the drug XML file.
     * @return A pointer to the corresponding XML descriptor.
     */
    const Tucuxi::Gui::Core::DrugXmlDescriptor *scanDrug(const QString &filePath);

    /** \brief Return the XML descriptor of a drug.
     * @param drugModelId The drug ID.
     * @return The drug XML descriptor.
     */
    const Tucuxi::Gui::Core::DrugXmlDescriptor *drug(const QString &drugModelId) const;

    /** \brief Return the list of the drugs' XML descriptors.
     * @return The list of the drugs' XML descriptors.
     */
    QList<const Tucuxi::Gui::Core::DrugXmlDescriptor *> drugs() const;

    /** \brief Remove a drug from the lister.
     * @param drugModelId The drug ID.
     */
    void removeDrug(const QString &drugModelId);

    //! Return the list of directories.
    /** Return the list of directories in which the drugs editor will search for drugs XML
     * files. By default, the list consists of only the drugs and custom drugs directories,
     * as well as the one saved in the user's config, if any.
     * @return The list of directories
     */
    QList<QDir> directories() const;

    //! Clear the directories list.
    void clearDirectories();

    /** \brief Add a directory to the scan list.
     * @param directory Directory to add.
     * @return True if successfully added, otherwise false.
     */
    bool addDirectory(const QDir &directory);

    /** \brief Remove a directory from the scan list.
     * @param directory Directory to remove.
     * @return True if successfully removed, otherwise false.
     */
    bool removeDirectory(const QDir &directory);

    /** \brief Returns the last error message.
     * @return The last error message.
     */
    QString errorMessage() const;

private:
    //The error messages
    static const char *const _DIRECTORY_UNREADABLE;
    static const char *const _FILE_BAD_EXTENSION;
    static const char *const _FILE_UNREADABLE;
    static const char *const _FILE_INVALID_XML;
    static const char *const _FILE_CANNOT_OPEN;
    static const char *const _DRUG_PARSING_ERROR;
    static const char *const _DRUG_INVALID_DESC;
    static const char *const _DRUG_DUPLICATED;

    //The shared strings
    static const char *const _IGNORING;

    /* Function used to scan the directories and search for each drug xml file.
     * The function scanDirectory() will scan each file it finds and register
     * each drug that were successfully parsed. */
    void scanDirectory(const QDir &directory);

    /* Scans a <model> element, get the version attribute and check if it matches
     * the current drug version. If not, raises an XML stream reader error. */
    void validateVersion();

    //Scans a <drug> element and create the corresponding drug xml descriptor
    Tucuxi::Gui::Core::DrugXmlDescriptor *buildDescriptor(const QString &filePath);

    //Deletes the existing drugs XML descriptors
    void resetDrugs();

    //The last error message
    QString _error;

    //The XML stream read
    QXmlStreamReader _parser;

    //The directories to look into
    QList<QDir> _directories;

    //QMap of <Drug model ID, Drug XML descriptor>
    QMap<QString, const Tucuxi::Gui::Core::DrugXmlDescriptor *> _drugs;
};

} //namespace AppUtils
} //namespace Gui
} //namespace Tucuxi
#endif // DRUGLISTER_H
