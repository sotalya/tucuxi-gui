//@@license@@

#ifndef DRUGXMLDESCRIPTOR_H
#define DRUGXMLDESCRIPTOR_H

#include <QString>
#include <QMetaType>


namespace ezechiel {
namespace core {

/** \ingroup drugxmlfiles
 *  \brief The partial description of a drug XML file.
 *
 * This class is used to store partial data about a spcific drug. It is mostly used by
 * the drug lister to extract the basic information of a drug XML file. It can then be
 * used to display general information about the drug, such as its ID, ATC, name, etc.
 * It can also be used to retrieve the corresponding XML file.
 *
 * \sa DrugLister
 */
class DrugXmlDescriptor
{

public:

    /** \brief Constructor.
     * @param drugId The ID of the drug.
     */
    DrugXmlDescriptor(const QString &drugId);

    //! Checks if the drug descriptor is valid.
    /** Checks if the drug descriptor is valid. In order to be considered valid, the
     * drug ID, name, domain name, study name and description must be non-empty. In
     * addition, the drug's file path must point to an existing file and the model ID
     * must be non-empty.
     * @return True is valid, otherwise false.
     */
    bool isValid() const;

    /** \brief Returns the drug ID.
     * @return The drug ID.
     */
    QString drugId() const;

    /** \brief Returns the drug's file path.
     * @return The drug's file path.
     */
    QString file() const;

    /** \brief Sets the drug's file path.
     * @param filePath The file path.
     */
    void setFile(const QString &filePath);

    /** \brief Returns the pk model ID.
     * @return The model ID.
     */
    QString pkModelId() const;

    /** \brief Sets the pk model ID.
     * @param modelId The model ID.
     */
    void setPkModelId(const QString &pkModelId);

    /** \brief Returns the drug's model ID.
     * @return The model ID.
     */
    QString drugModelId() const;

    /** \brief Sets the drug's model ID.
     * @param modelId The model ID.
     */
    void setDrugModelId(const QString &pkModelId);

    /** \brief Returns the drug ATC.
     * @return The drug ATC.
     */
    QString atc() const;

    /** \brief Sets the drug ATC.
     * @param atc The drug ATC.
     */
    void setAtc(const QString &atc);

    /** \brief Returns the drug name.
     * @return The drug name.
     */
    QString name() const;

    /** \brief Sets the drug name.
     * @param name The drug name.
     */
    void setName(const QString &name);

    /** \brief Returns the domain name.
     * @return The domain name.
     */
    QString domainName() const;

    /** \brief Sets the domain name.
     * @param domainName The domain name.
     */
    void setDomainName(const QString &domainName);

    /** \brief Returns the study name.
     * @return The study name.
     */
    QString studyName() const;

    /** \brief Sets the study name.
     * @param studyName The study name.
     */
    void setStudyName(const QString &studyName);

    /** \brief Returns the drug description.
     * @return The drug description.
     */
    QString description() const;

    /** \brief Sets the drug description.
     * @param description The drug description.
     */
    void setDescription(const QString &description);

private:
    //The drug ID
    QString _drugId;

    //The file path
    QString _filePath;

    //The drug model ID
    QString _drugModelId;

    //The Pk model ID
    QString _pkModelId;

    //The drug ATC
    QString _atc;

    //The drug name
    QString _name;

    //The domain name
    QString _domain;

    //The study name
    QString _study;

    //The drug description
    QString _description;
};

} // namespace core
} // namespace ezechiel

#endif // DRUGXMLDESCRIPTOR_H
