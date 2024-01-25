#ifndef SENTENCEPALETTE_H
#define SENTENCEPALETTE_H
#define VALIDATION_SECTIONS 5

#include <QObject>
#include "core/dal/entity.h"
#include "tucucommon/xmldocument.h"
#include "tucucommon/xmlimporter.h"

namespace Tucuxi {
namespace Gui {
namespace Admin {

///
/// \brief The Sentence class
/// \brief This object contains the sentence text, modifier and key
class Sentence : public Tucuxi::Gui::Core::Entity
{
    Q_OBJECT

    ENTITY_UTILS(Sentence)

public:
    AUTO_PROPERTY_DECL(int, modifier, Modifier)
    AUTO_PROPERTY_DECL(int, key, Key)
    AUTO_PROPERTY_DECL(QString, text, Text)

public:
    Q_INVOKABLE explicit Sentence(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent = nullptr);
    Q_INVOKABLE Sentence(int key, int modifier, QString text);

    bool operator==(const Sentence& _other) const;
};

///
/// \brief The DrugSentences class
/// \brief This object contains the drug specific sentences list and the drug id
class DrugSentences : public Tucuxi::Gui::Core::Entity
{
    Q_OBJECT

    ENTITY_UTILS(DrugSentences)

    AUTO_PROPERTY_DECL(QString, drugId, DrugId)
    AUTO_PROPERTY_DECL(QList<Sentence*>, sentences, Sentences)

public:

    Q_INVOKABLE explicit DrugSentences(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent = nullptr){}
    void addSentence(Sentence* _sentence);
    void editSentence(int _listIndex, Sentence* _sentence);
    void removeSentence(int _listIndex);
};

///
/// \brief The Section class
/// \brief This object contains the global sentences list and the DrugSentences list
class Section : public Tucuxi::Gui::Core::Entity
{
    Q_OBJECT

    ENTITY_UTILS(Section)

    AUTO_PROPERTY_DECL(QString, sectionId, SectionId)
    AUTO_PROPERTY_DECL(QList<Sentence*>, globalSentences, GlobalSentences)
    AUTO_PROPERTY_DECL(QList<DrugSentences*>, specificSentences, SpecificSentences)

    Q_INVOKABLE explicit Section(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent = nullptr);

    void addDrugSentences(QString _drugId);

    Tucuxi::Gui::Core::AbstractRepository *_repository;

public:

    Q_INVOKABLE void addSentenceToGlobal(int key, int modifier, QString text);
    Q_INVOKABLE void editSentenceOfGlobal(int key, int modifier, QString text, int _listIndex);
    Q_INVOKABLE void removeSentenceFromGlobal(int _listIndex);

    Q_INVOKABLE void addSentenceToDrugSentencesList(QString drugId, int key, int modifier, QString text);
    Q_INVOKABLE void editSentenceOfDrugSentencesList(QString _drugId, int key, int modifier, QString text, int _listIndex);
    Q_INVOKABLE void removeSentenceFromDrugSentencesList(QString _drugId, int _listIndex);

    ///
    /// \brief getSpecificSentencesList : return the list of sentences texts according to the drugId
    /// \param _drugId : the drug id
    /// \return QStringList :
    ///
    Q_INVOKABLE QStringList getSpecificSentencesList(QString _drugId);

    Q_INVOKABLE QList<Sentence*> getSentenceFromDrugId(QString _drugId);

    ///
    /// \brief getGlobalSentencesTextsList : return the list of sentences texts
    /// \brief Used in SentencePaletteDialog.qml to display the content
    /// \return QStringList :
    ///
    Q_INVOKABLE QStringList getGlobalSentencesTextsList();

    void addSentenceToDrugSentences(QString _drugId, Sentence* _sentence);

    QString getSentencePerKey(int key, int modifiers, QString drugId);
};

QML_POINTERLIST_CLASS_DECL(SectionList, Section)

///
/// \brief The SentencesPalettes class
/// \brief This object is the top of the sentences palettes architecture
///
class SentencesPalettes : public Tucuxi::Gui::Core::Entity
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

        bool isXMLSentenceExisting(QList<Sentence*> _sentence, Sentence* _xmlSentence);

    public:
        void importXml(SentencesPalettes *_sentencesPalettes, QString _fileName);

        int stringToKey(QString keyString);
        int stringToModifier(QString modifierString);
    };

    Q_OBJECT

    ENTITY_UTILS(SentencesPalettes)

    AUTO_PROPERTY_DECL(QList<Sentence*>, globalSentences, GlobalSentences)
    AUTO_PROPERTY_DECL(SectionList*, sectionsList, SectionsList)
    AUTO_PROPERTY_DECL(QString, filename, Filename)

    QString getDefaultPath();

    Tucuxi::Common::XmlDocument m_doc;

    SentencesPalettesImporter *m_sentencesPalettesImporter;

public:

    Q_INVOKABLE explicit SentencesPalettes(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent = nullptr);

    // Used in QML to get the reference to the section.
    // Especially used to write the sentence from list to the TextArea
    Q_INVOKABLE Section* getSection(int i) { return _sectionsList->at(i);}

    Q_INVOKABLE void exportToXml();
    Q_INVOKABLE void saveXMLPath();
    Q_INVOKABLE QString loadXMLPath();

    // Manual import, from the settingsDialog button^
    Q_INVOKABLE void manualImport(QString _filename);

    // Check if the saving path already exist.
    Q_INVOKABLE bool isPathExisting(QString _name);

    // Convert modifier and key to string and vice versa
    std::string modifierToString(int modifier);
    std::string keyToString(int key);
};

}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::Section*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Admin::Section*>)
Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::SectionList*)

#endif // SENTENCEPALETTE_H
