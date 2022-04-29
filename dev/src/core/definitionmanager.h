//@@license@@

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <QCoreApplication>
#include <QList>
#include <QString>
#include <QMetaType>

#include "core/utils/ezutils.h"
#include "core/utils/duration.h"
#include "core/utils/ezxmlreader.h"

/** \ingroup core
 *  \brief Provides an interface to the software's global medical definitions.
 *
 * This object is an interface to the software's global medical definitions.
 * The data is described in a definitions.xml file located in the 'Core::ApplicationData' directory, and allow the user to retrieve:
 * \li The intakes type's id, name and description
 * \li The targets type's id, name and description
 * \li The global covariates and their default value (see \ref Covariate).
 * \li The global (default) intervale for the reverse engines (see \ref reengines).
 * \li The possible drug intakes with their name and description.
 *
 * \section struct XML Structure
 * The structure of the XML file is as follows:
 * \code
  <definitions version='0.1'>

   <intakes>
      <intake>
         <id>extra</id>
         <names>
           <name lang='en'>Extravascular</name>
           <name lang='fr'>Extravasculaire</name>
         </names>
         <descriptions>
           <description lang='en'>Administration of a solution occurring outside a blood or lymph vessel.</description>
           <description lang='fr'>Administration d'une solution en dehors du sang.</description>
         </descriptions>
      </intake>
   </intakes>

   <targets>
     <target>
         <id>peak</id>
         <names>
           <name lang='en'>Concentration peak</name>
           <name lang='fr'>Pic de concentration</name>
         </names>
         <descriptions>
           <description lang='en'>Target for the drug concentration peak.</description>
           <description lang='fr'>Cible de pic de concentration du médicament.</description>
         </descriptions>
     </target>
   </targets>

   <covariates>
     <covariate>
       <id>weight</id>
       <names>
         <name lang='en'>Weight</name>
         <name lang='fr'>Poid</name>
       </names>
       <descriptions>
         <description lang='en'>Weight of the patient</description>
         <description lang='en'>Poids du patient</description>
       </descriptions>
       <unit>kg</unit>
       <type>double</type>
       <defaultvalue>70</defaultvalue>
     </covariate>
   </covariates>

   <intervals>
      <interval>4</interval> <!-- Times always in hours -->
      <interval>8</interval> <!-- Times always in hours -->
   </intervals>

</definitions>

 * \endcode
 * \section trans Note on translation
 * The definitions xml file might contain names and definitions in more than one language.
 * By default, the definition manager will transmit the language currently used by the software to the \ref EzXMLReader, which will load the corresponding definitions.
 * It is also possible to set another language at any time, resulting in a reset and new scan of the definitions.
 * \sa Covariate, reengines, EzXMLReader
 */

namespace ezechiel {
namespace core {

class DefinitionManager
{
    Q_DECLARE_TR_FUNCTIONS(Definitions)

public:
    static const char *const _dateTimeFormat;
    static const char *const _xmlDateTimeFormat;
    //! Read the XML file and stored the data contained within.
    DefinitionManager();

    /** \brief Returns the list of intakes.
     * @return The list of intakes descriptors.
     */
    QList<Descriptor> intakes() const;

    /** \brief Returns the descriptor of a specific intake.
     * @param id The intake ID.
     * @return The intake descriptor.
     */
    Descriptor intake(const QString id) const;

    /** \brief Returns true if the specified intake is valide.
     * @param id The intake ID.
     * @return True if the intake exists.
     */
    bool validateIntake(const QString id) const;

    /** \brief Returns true if the specified intake is of an infusion type.
     * @param id The intake ID.
     * @return True if the intake is of an infusion type.
     */
    bool isInfusion(const QString id) const;

    /** \brief Returns the list of targets.
     * @return The list of targets descriptors.
     */
    QList<Descriptor> targets() const;

    // ToDo ///////////////////////////////////////////////////////////////////////////////////////
    // Two possible ways for the targets:
    // 1. Use the enum and add functions to make the correspondance between enum value and target
    //    descriptor
    // 2. Remove the enum from the software and use the IDs instead
    // ////////////////////////////////////////////////////////////////////////////////////////////

    /** \brief Returns the descriptor of a specific target type.
     * @param id The target ID.
     * @return The target descriptor.
     */
    //Descriptor target(const QString id) const;

    /** \brief Returns the target type given its ID.
     * @param id The target ID.
     * @return The target type, or UnkownTarget if the ID is invalid.
     */
    int target(const QString &id) const;

    /** \brief Returns true if the specified target type is valide.
     * @param id The target ID.
     * @return True is the target exists.
     */
    bool validateTarget(const QString &id) const;

    /** \brief Returns the list of global covariates.
     * @return The list of global covariates.
     */
    //QList<Covariate> covariates() const;

    /** \brief Returns the global covariate corresponding to this ID.
     * @param id The covariate ID.
     * @return The global covariate.
     */
    //Covariate covariate(const QString id) const;

    /** \brief Returns true if a global covariate corresponding to this ID exists.
     * @param id The covariate ID.
     * @return True is the global covariate exists.
     */
    //bool validateCovariate(const QString id) const;

    // ToDo: removed when refactored ////////////////////////////
    // Remove the covariates specific function below and adapt
    // the registerCovariates functions. In addition, check the
    // default global covariates in setCovariate of the Drug obj.
    /////////////////////////////////////////////////////////////

    //! Register a drug-specific covariate.
    /** If the covariate ID already exists, or if the name, unit or type is invalid, the function returns false.
     * @param drugId The drug ID.
     * @param covariateId The covariate ID.
     * @param covariateName The covariate name.
     * @param covariateDesc The covariate description.
     * @param unit The covariate unit.
     * @param type The covariate type.
     * @param value The covariate default value.
     * @return Returns true if successfully registered, otherwise return false.
     * \sa Covariate
     */
    bool registerCovariate(const QString &drugId, const QString &covariateId, const QString &covariateName,
                           const QString &covariateDesc, const Unit &unit, const QMetaType::Type type, double value);

    //! Returns the list of descriptions of a specific group of covariates.
    /** A group of covariate is either specific to a drug, or the group of global covariates.
     * To retrieve the list of global covariates descriptions, simply ignore the drugId paramater.
     * @param drugId The drug ID of the covariate group, or the default value for the global covariates.
     * @return The list of covariates ID, name and description, translated.
     * \sa Covariate
     */
    QList<Descriptor> covariatesDesc(QString drugId = QString()) const;

    /** List of clinicals description.
     * @return The list of clinicals ID, name and description, translated.
     * \sa Clinicals
     */
    QList<Descriptor> clinicals () const;

    /** Descriptor for a specific covariate.
     * @param id The covariate ID
     * @return The corresponding descriptor if found, and an empty one otherwise
     * \sa Covariate
     */
    Descriptor covariateDesc(const QString &id) const;

    Descriptor covariate (const QString &id) const;
    /** Descriptor for a specific clinical.
     * @param id The covariate type
     * @return The corresponding descriptor if found, and an empty one otherwise
     * \sa Clinical
     */
    Descriptor clinical (const QString &id) const;
    /** Does this covariate exists?
     * @param id The covariate ID to check
     * @return True if it exists, false otherwise
     * \sa Covariate
     */
    bool covariateExists (const QString &id) const;
    /** Is this covariate a duplicate of another covariate ?
     * @param id The covariate ID.
     * @param unit The covariate unit.
     * @param type The covariate type.
     * @return True if it exists with the same ID, unit and type, false otherwise.
     */
    bool isDuplicate (const QString &covariateId, const Unit &unit, const QMetaType::Type type);
    /** Does this clinical exist?
     * @param type The covariate type to check
     * @return True if it exists, false otherwise
     * \sa Covariate
     */
    bool clinicalExists (const QString &id) const;
    /** A covariate's unit.
     * @param id The covariate ID
     * @return The unit for the given covariate, or an invalid one otherwise
     * \sa Covariate
     */
    Unit covariateUnit (const QString &id) const;
    /** A covariate's data type.
     * A covariate may contains different kind of data (boolean, double, integer) and this function returns the corresponding data type given a covariate ID.
     * @param id The covariate ID
     * @return The given covariate type, or 0 if not found
     * \sa Covariate
     */
    QMetaType::Type covariateType(const QString &id) const;

    /** A covariate's value.
     * @param id The covariate ID
     * @return The default value of the covariate, or -1.0 if not found
     * \sa Covariate
     */
    double covariateValue(const QString &id) const;

    //! Returns a covariate default value.
    /** Returns a covariate default value for display puprose. The default value is either one of the
     * covariate possible values, if there is a list available, or a double value if there is none.
     * @param id The covariate ID.
     * @return The covariate default value.
     */
    QString covariateDefaultValue(const QString &id) const;

    //! Returns a covariate list of possible values.
    /** Returns a covariate list of possible values. Those values are used to present the different
     * values a covariate can take to the end user. If the list is empty, the covariate type will be
     * used instead.
     * @param id The covariate ID.
     * @return The list of possible values, or an empty list if none are given.
     * \sa Covariate
     */
    QStringList covariateValuesList(const QString &id) const;

    /////////////////////////////////////////////////////////////

    /** \brief Return the list of the default intervals durations.
     * @return The list of default intervals.
     */
    QList<ezechiel::core::Duration> intervals() const;

    /** \brief Returns the language of the definitions.
     * @return The current language.
     */
    QString language() const;

    //! Sets a new language.
    /** When calling this function, the definition manager will reload the definitions using the appropriate language.
     * @param language The new language.
     */
    void setLanguage(const QString language);

private:
    //Functions used to sets the xml files structure, attributes and ignored flags
    void setXMLStructure();
    void setAttributes();
    void setIgnoredTags();

    //Load the definitions
    bool loadDefinitions();

    //Functions used to register the intakes, targets, covariates and intervals
    bool registerIntakes(const EzTag *root);
    bool registerTargets(const EzTag *root);
    bool registerCovariates(const EzTag *root);
    bool registerIntervals(const EzTag *root);
    bool registerClinicals(const EzTag *root);

    //Current language
    QString _language;

    //XML reader object
    EzXMLReader _xmlReader;

    //Map of <Intake ID, Intake descriptor>
    QMap<QString, Descriptor> _intakes;

    //Map of <Target ID, Target descriptor>
    QMap<QString, Descriptor> _targets;

    //List of default intervals duration
    QList<ezechiel::core::Duration> _intervals;

    // ToDo: remove when refactored //////////////
    //Hash of <Drug ID, QList<Covariate ID> >
    QHash<QString, QList<Descriptor> > _covariatesGroup;

    //Map of <Covariate ID, Covariate descriptor>
    QMap<QString, Descriptor> _covariatesDesc;

    //Map of <Covariate ID, Covariate>
    QMap<QString, double> _covariatesValue;
    //Map of covariates descriptors
    QMap<QString,Descriptor> _covariates;
    //Map of covariates unit
    QMap<QString,Unit> _covariatesUnit;
    //Map of covariates type
    QMap<QString,QMetaType::Type> _covariatesType;
    //Map of clinicals types
    QMap<QString, Descriptor> _clinicalsType;
    //////////////////////////////////////////////
};

} // namespace core
} // namespace ezechiel

#endif // DEFINITIONS_H
