#ifndef DRUGMANAGER_H
#define DRUGMANAGER_H

//class Drug;
namespace ezechiel {namespace GuiCore{class ParameterSet;}}
//class DrugXmlDescriptor;

#include <QCoreApplication>
#include <QDomElement>
#include <QMap>
#include <QDir>

#include "druglister.h"
#include "core/utils/ezutils.h"

/** \ingroup core
 *  \brief Provides an interface to the drugs for the rest of the software.
 *
 * The drug manager is in charge to list the existing drugs and provide an interface to the corresponding
 * drug objects. When instantiated, the drug manager will search in the default drug folders for every
 * drug xml files. It will then scan each one of them and store only the necessary data.
 *
 * The drug objects are build on demand. The first time a drug is requested, the drug manager will parse
 * and build the corresponding Drug object, and store it for further usage. Those objects can then be
 * retrieved at any time by their respective ID.
 *
 * WARNING: The drug manager might need to re-parse and rebuild the drugs at some point during runtime, if
 * for example the language is changed. For this reason, no class should stores the drugs pointers. Instead,
 * they need to access the drug manager everytime they need data about a drug.
 *
 * The drug manager will also fill several maps that can be used to obtain:
 * \li the \ref Descriptor of a given drug
 * \li the \ref Descriptor of all the drugs
 * \li the \ref Descriptor of all the drugs matching a given \ref Model ID
 * \li the list of models IDs that each drug can be used with
 *
 * In addition, it is possible to add or remove a folder from the scan path, resulting in a reset of the
 * current drugs and a new scan.
 *
 * \section trans Note on translation
 * The drug xml files might contain definitions in more than one language. By default, the drug manager
 * will use the application default language when parsing the XML files. When this language is changed,
 * the languageChanged() slot must be called in order to re-parse the drugs using the correct language.
 *
 * \sa Drug, DrugLister, DrugXmlDescriptor
 */

namespace ezechiel {

namespace GuiCore {
    class DrugModel;
}

namespace apputils {
class DrugManager
{
    Q_DECLARE_TR_FUNCTIONS(DrugManager)

public:

    //! Constructor.
    /** Constructor. The drugs descriptions will be build automatically.
     */
    DrugManager();

    //! Destructor.
    ~DrugManager();

    /** \brief Checks if a drug exits.
     * @param drugId The ID of the drug.
     * @return True if it exists, otherwise false.
     */
    bool exists(const QString &drugId);

    //! Scan a drug XML file.
    /** Scan a drug XML file, returning the corresponding drug XML descriptor. If an error occurred,
     * the function returns null and the error message can be retrieved using the errorMessage()
     * function. The caller takes the ownership of the object and must delete it when appropriate.
     *
     * Note: A drug XML descriptor can be used to retrieve basic information about the drug, or sent
     * to the buildDrug() function in order to build the corresponding drug object.
     *
     * @param filePath The path to the drug XML file.
     * @return A drug XML descriptor.
     */
    const ezechiel::GuiCore::DrugXmlDescriptor *scanDrug(const QString &filePath);

    //! Build a drug.
    /** Build a drug given its XML descriptor. If an error occurred, the function returns null and the
     * error message can be retrieved using the errorMessage() function. The caller takes the ownership
     * of the object and must delete it when appropriate.
     *
     * Note: If no error occurred, the drug is built but might contain inconsistent data. To ensure
     * its data is valid, use the validateDrug() function.
     *
     * @param xmlDesc The drug XML descriptor.
     * @return The drug, or null if an error occured.
     */
    ezechiel::GuiCore::DrugModel* buildDrug(const ezechiel::GuiCore::DrugXmlDescriptor *xmlDesc);

    //! Validate a drug.
    /** Validate a drug. If an error occurred, the function returns false and an error message can be
     * retrieved using the errorMessage() function. The caller keeps the ownership of the object.
     *
     * Note: The validation will ensure that the drug's data is valid and compatible with its model.
     *
     * @param drug A drug object.
     * @return True if the drug is valid.
     */
    bool validateDrug(const ezechiel::GuiCore::DrugModel* drug);

    /** \brief Returns the descriptor of the given drug.
     * @param drugId The ID of the drug.
     * @return The corresponding descriptor.
     */
    ezechiel::GuiCore::Descriptor description(const QString &drugId);

    /** \brief Returns the descriptors of all the drugs.
     * @return A list of the descriptors.
     */
    QList<ezechiel::GuiCore::Descriptor> descriptions();

    //! Returns the models IDs compatible with the given drug.
    /** Returns the models IDs compatible with the given drug. A model can be used by a drug if it is
     * defined with all the necessary information in the corresponding drug XML file.
     * @param drugId The drug ID.
     * @return The list of the corresponding model IDs.
     */
    QList<QString> models(const QString &drugId);

    //! Returns the drugs descriptions compatible with the given model ID.
    /** Returns the drugs descriptions compatible with the given model ID. A drug is compatible with a
     * model if the corresponding XML files contains all the necessary information required by the model.
     * @param modelId The ID of the model.
     * @return The list of the corresponding drug descriptors.
     */
    QList<ezechiel::GuiCore::Descriptor> descriptionsForModel(const QString &modelId);

    /** \brief Returns the ATC of the given drug.
     * @param drugId The ID of the drug.
     * @return The corresponding ATC.
     */
    QString atc(const QString &drugId);

    /** \brief Returns the drugs descriptions matching the given ATC.
     * @param atc The drug ATC.
     * @return The list of the corresponding drug descriptors.
     */
    QList<ezechiel::GuiCore::Descriptor> descriptionsForAtc(const QString &atc);

    /** \brief Returns the domain of the given drug.
     * @param drugId The ID of the drug.
     * @return The corresponding domain.
     */
    QString domain(const QString &drugId);

    /** \brief Returns the study of the given drug.
     * @param drugId The ID of the drug.
     * @return The corresponding study.
     */
    QString study(const QString &drugId);

    //! Returns a drug.
    /** Returns a drug. The drug returned is const in order to prevent any class to modify the drug's
     * information. The drug might need to be parsed and build if it is the first time it is requested.
     * It will then be stored in the drug manager and kept as long as the language or drugs directories
     * are not changed.
     *
     * WARNING: As a drug might be deleted, for example when changing the language, the caller SHOULD
     * NOT store the pointer to the drug. Instead, it must call the drug manager each time it needs to
     * access data about this drug.
     * @param drugId The drug ID.
     * @return The const pointer to the drug, or null if an error occured.
     */
    ezechiel::GuiCore::DrugModel* drug(const QString &drugId);

    //! Remove a drug.
    /** Remove a drug from the drug manager. The drug will also be removed in the drug lister internal
     * object.
     * @param drugId The drug ID.
     */
    void removeDrug(const QString &drugId);

    /** \brief Returns the list of the directories in which the drug manager will search for the drug xml
     * files.
     * @return The list of the directories.
     */
    QList<QDir> directories() const;

    //! Adds a directory.
    /** Adds a directory to the search path. The drug manager will reset the current drugs descriptions
     * and will perform a new scan. The existing drugs will be deleted and rebuild only when needed.
     * @param directory The directory to add.
     * @return Returns true if successfully added, otherwise returns false.
     */
    bool addDirectory(const QDir &directory);

    //! Removes a directory.
    /** Removes a directory from the search path. The drug manager will reset the current drugs descriptions
     * and will perform a new scan. The existing drugs will be deleted and rebuild only when needed.
     * @param directory The directory to remove.
     * @return Returns true if successfully removed, otherwise returns false.
     */
    bool removeDirectory(const QDir &directory);

    /** \brief Returns the last error message.
     * @return The last error message.
     */
    QString errorMessage() const;

    /**
     * @brief Analyzes the covariates to find near duplicates
     * Displays the result on stdout
     */
    void analyzeCovariates();

    /**
     * @brief Clears the error message
     */
    void clearErrorMessage();

    /**
     * @brief tryToAddDrugModelToRepo Adds a new drug model to the repository
     * @param drugModel
     * @return true if it was added, false if the drug model Id is already in the repo.
     *  It allows to avoid duplicates.
     */
    bool tryToAddDrugModelToRepo(ezechiel::GuiCore::DrugModel *drugModel);

public slots:

    //! Reload the drugs.
    /** Reload the drugs using the new language. The drug manager will reset the current drugs descriptions
     * and will perform a new scan. The existing drugs will be deleted and rebuild only when needed.
     */
    void languageChanged();

private:
    //The shared strings
    static const char *const _IGNORING;

    //Scans all drug files and build the descriptions and global maps
    void scanDrugs();

    //Functions that parse data related to the population parameters
    bool parseErrorModel(QDomElement node, ezechiel::GuiCore::ParameterSet* parameters);
    bool parseCorrelations(QDomElement node, ezechiel::GuiCore::ParameterSet* parameters);

    //Functions that validate data extracted from the drug file
    bool validateModel(const ezechiel::GuiCore::DrugModel* drug);
    bool validateParameters(const ezechiel::GuiCore::DrugModel* drug);

    // bool validateScripts(const ezechiel::GuiCore::DrugModel* drug);

    //Deletes the existing drugs
    void resetDrugs();

    //The last error message
    QString _error;

    //The drug lister
    ezechiel::apputils::DrugLister _lister;

    //The map of <Drug ID, Drug domain>
    QMap<QString, QString> _drugIdToDrugDomain;

    //The map of <Drug ID, Drug study>
    QMap<QString, QString> _drugIdToDrugStudy;

    //The map of <Drug ID, Drug object>
    QMap<QString, ezechiel::GuiCore::DrugModel*> _drugIdToDrugObj;

    //The map of <Drug ID, Drug descriptor>
    QMap<QString, ezechiel::GuiCore::Descriptor> _drugIdToDrugDesc;

    //The map of <Drug ID, Drug ATC>
    QMap<QString, QString> _drugIdToDrugAtc;

    //The map of <Drug ID, Model ID>
    QMultiMap<QString, QString> _drugIdToModelId;

    //The map of <Model ID, Drug descriptor>
    QMultiMap<QString, ezechiel::GuiCore::Descriptor> _modelIdToDrugDesc;

    //The map of <Drug ATC, Drug descriptor>
    QMultiMap<QString, ezechiel::GuiCore::Descriptor> _atcToDrugDesc;

    template < typename error >
    inline void log(QtMsgType type, QString msg, const char * file, int line, const char * function)
    {
        _error = QString("%1: %2\n").arg(error::report(), msg);
    }

};

} //namespace apputils
} //namespace ezechiel
#endif // DRUGMANAGER_H
