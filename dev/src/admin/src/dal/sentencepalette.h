#ifndef SENTENCEPALETTE_H
#define SENTENCEPALETTE_H
#define VALIDATION_SECTIONS 5

#include <QObject>
#include "core/dal/entity.h"
#include "tucucommon/xmldocument.h"
#include "tucucommon/xmlimporter.h"

///
/// \brief The DrugSentences class
/// \brief This object contains the drug specific sentences list and the drug id
class DrugSentences : public ezechiel::core::Entity
{
    Q_OBJECT

    ENTITY_UTILS(DrugSentences)

    AUTO_PROPERTY_DECL(QString, drugId, DrugId)
    AUTO_PROPERTY_DECL(QStringList, sentences, Sentences)

public:

    Q_INVOKABLE explicit DrugSentences(ezechiel::core::AbstractRepository *repository, QObject *parent = nullptr){}
    Q_INVOKABLE void addSentence(QString _sentence);
    Q_INVOKABLE void removeSentence(int _listIndex);

};

///
/// \brief The Section class
/// \brief This object contains the global sentences list and the DrugSentences list
class Section : public ezechiel::core::Entity
{
    Q_OBJECT

    ENTITY_UTILS(Section)

    AUTO_PROPERTY_DECL(QString, sectionid, SectionId)
    AUTO_PROPERTY_DECL(QStringList, globalSentences, GlobalSentences)
    AUTO_PROPERTY_DECL(QList<DrugSentences*>, specificSentences, SpecificSentences)

    Q_INVOKABLE explicit Section(ezechiel::core::AbstractRepository *repository, QObject *parent = nullptr){
        _repository = repository;
    }

public:

    Q_INVOKABLE void addSentenceToGlobal(QString _sentence);
    Q_INVOKABLE void removeSentenceFromGlobal(int _listIndex);

    Q_INVOKABLE void addSentenceToDrugSentencesList(QString _drugId, QString _sentence);
    Q_INVOKABLE void removeSentenceFromDrugSentencesList(QString _drugId, int _listIndex);

    ///
    /// \brief getSpecificSentencesList : return the list of sentences according to the drugId
    /// \brief Used in SentencePaletteDialog.qml to display the content
    /// \param _drugId : the drug ident
    /// \return QStringList :
    ///
    Q_INVOKABLE QStringList getSpecificSentencesList(QString _drugId);

    void addSentenceToDrugSentences(QString _drugId, QString _sentence);

private:
    void addDrugSentences(QString _drugId);
//    void addDrugSentences(QString _drugId, QString _sentence);
    ezechiel::core::AbstractRepository *_repository;
};

QML_POINTERLIST_CLASS_DECL(SectionList, Section)

///
/// \brief The SentencesPalettes class
/// \brief This object is the top of the sentences palettes architecture
///
class SentencesPalettes : public ezechiel::core::Entity
{
    ///
    /// \brief The SentencesPalettesImporter class
    /// \brief This object is used to convert the xml file into SentencesPalettes
    class SentencesPalettesImporter : public Tucuxi::Common::XMLImporter
    {
        const std::vector<std::string> &ignoredTags() const override{
            static std::vector<std::string> ignored;
            return ignored;
        }

        public:
        void importXml(SentencesPalettes *_sentencesPalettes, QString _fileName);

    };

    Q_OBJECT

    ENTITY_UTILS(SentencesPalettes)

    AUTO_PROPERTY_DECL(QStringList, globalSentences, GlobalSentences)
    AUTO_PROPERTY_DECL(SectionList*, sectionsList, SectionsList)
    AUTO_PROPERTY_DECL(QString, filename, Filename)

    public:

    Q_INVOKABLE explicit SentencesPalettes(ezechiel::core::AbstractRepository *repository, QObject *parent = nullptr)
    {
        _sectionsList = ezechiel::core::CoreFactory::createEntity<SectionList>(repository);
        for (int i=0; i < VALIDATION_SECTIONS; i++){
            auto newSection = ezechiel::core::CoreFactory::createEntity<Section>(repository);
            newSection->setSectionId(QString::number(i));
            _sectionsList->append(newSection);
        }
        _filename = loadXMLPath();
        SentencesPalettesImporter s;
        s.importXml(this, _filename);
    }

    Q_INVOKABLE Section* getSection(int i) { return _sectionsList->at(i);}

    Q_INVOKABLE void addSentenceToGlobal(QString _sentence);
    Q_INVOKABLE void removeSentenceFromGlobal(int _listIndex);

    Q_INVOKABLE void exportToXml();
    Q_INVOKABLE void saveXMLPath();
    Q_INVOKABLE QString loadXMLPath();

private:
    QString getDefaultPath();

    Tucuxi::Common::XmlDocument m_doc;
};

Q_DECLARE_METATYPE(Section*)
Q_DECLARE_METATYPE(QList<Section*>)
Q_DECLARE_METATYPE(SectionList*)

#endif // SENTENCEPALETTE_H
