#ifndef SENTENCEPALETTE_H
#define SENTENCEPALETTE_H
#define VALIDATION_SECTIONS 5

#include <QObject>
#include "core/dal/entity.h"
#include "tucucommon/xmldocument.h"
#include "tucucommon/xmlimporter.h"
//#include "sentencepaletteimporter.h"


class Sentences : public ezechiel::core::Entity
{
    Q_OBJECT

    ENTITY_UTILS(Sentences)

    AUTO_PROPERTY_DECL(QString, drugId, DrugId)
    AUTO_PROPERTY_DECL(QStringList, sentences, Sentences)

public:

    Q_INVOKABLE explicit Sentences(ezechiel::core::AbstractRepository *repository, QObject *parent = nullptr){}
    Q_INVOKABLE void addSentenceToSpecific(QString _sentence);
    Q_INVOKABLE void removeSentenceFromSpecific(int _listIndex);

};


class Section : public ezechiel::core::Entity
{
    Q_OBJECT

    ENTITY_UTILS(Section)

    AUTO_PROPERTY_DECL(QString, sectionid, SectionId)
    AUTO_PROPERTY_DECL(QStringList, globalSentences, GlobalSentences)
    AUTO_PROPERTY_DECL(QList<Sentences*>, specificSentences, SpecificSentences)

    Q_INVOKABLE explicit Section(ezechiel::core::AbstractRepository *repository, QObject *parent = nullptr){
        _repository = repository;
    }

public:

    Q_INVOKABLE void addSentenceToGlobal(QString _sentence);
    Q_INVOKABLE void removeSentenceFromGlobal(int _listIndex);
    Q_INVOKABLE QStringList getSentencesList(QString _drugId);
    Q_INVOKABLE void addSentenceToSentencesList(QString _drugId, QString _sentence);
    Q_INVOKABLE void removeSentenceFromSentencesList(QString _drugId, int _listIndex);

    void addSentenceToSentences(QString _drugId, QString _sentence);

private:
    void addSentences(QString _drugId);

    QMap<QString, QStringList> _specificSentencesMap;
    ezechiel::core::AbstractRepository *_repository;
};

class SentencesPalettes : public ezechiel::core::Entity
{
    class SentencesPalettesImporter : public Tucuxi::Common::XMLImporter
    {
        const std::vector<std::string> &ignoredTags() const override{
            static std::vector<std::string> ignored;
            return ignored;
        }

        public:
        void importXml(SentencesPalettes *_sentencesPalettes, std::string _fileName);

    };

    Q_OBJECT

    ENTITY_UTILS(SentencesPalettes)

    AUTO_PROPERTY_DECL(QStringList, globalSentences, GlobalSentences)
    AUTO_PROPERTY_DECL(QList<Section*>, sectionsList, SectionsList)

    public:

    Q_INVOKABLE explicit SentencesPalettes(ezechiel::core::AbstractRepository *repository, QObject *parent = nullptr){
        for (int i=0; i < VALIDATION_SECTIONS; i++){
            auto newSection = ezechiel::core::CoreFactory::createEntity<Section>(repository);
            newSection->setSectionId(QString::number(i));
            _sectionsList.push_back(newSection);
        }
        m_filename = "/home/johan/Bureau/sentence.xml";
        SentencesPalettesImporter s;
        s.importXml(this, m_filename);
    }

    Q_INVOKABLE void addSentenceToGlobal(QString _sentence);
    Q_INVOKABLE void removeSentenceFromGlobal(int _listIndex);
    Q_INVOKABLE void exportToXml();

private:
    Tucuxi::Common::XmlDocument m_doc;
    std::string m_filename;
};

#endif // SENTENCEPALETTE_H
