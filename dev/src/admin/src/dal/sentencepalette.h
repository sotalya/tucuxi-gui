#ifndef SENTENCEPALETTE_H
#define SENTENCEPALETTE_H
#define VALIDATION_SECTIONS 5

#include <QObject>
#include "core/dal/entity.h"
#include "tucucommon/xmldocument.h"


//class SentencesPalette : public ezechiel::core::Entity
//{
//    Q_OBJECT

//    ENTITY_UTILS(SentencesPalette)

//    AUTO_PROPERTY_DECL(QStringList, drugSpecificSentences, DrugSpecificSentences)
//    AUTO_PROPERTY_DECL(QList<QStringList>, drugList, DrugList)

//    protected:

//        Q_INVOKABLE explicit SentencesPalette(ezechiel::core::AbstractRepository *repository, QObject *parent = nullptr){
////        loadSentencesSettings();
//        }

//    public:
//        Q_INVOKABLE void addSentenceToSpecific(QString _sentence);
//        Q_INVOKABLE void removeSentenceFromSpecific(int _listIndex);
//        Q_INVOKABLE void saveSentencesSettings(int _index, QString _drugId);

//        ~SentencesPalette() {}

//        Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
//        {
//            return true;
//        }

//private:
//    Tucuxi::Common::XmlDocument m_doc;
//    void loadSentencesSettings();

//};

class Sentences : public ezechiel::core::Entity
{
    Q_OBJECT

    ENTITY_UTILS(Sentences)

    AUTO_PROPERTY_DECL(QString, drugId, DrugId)
    AUTO_PROPERTY_DECL(QStringList, sentences, Sentences)

public:

    Q_INVOKABLE explicit Sentences(ezechiel::core::AbstractRepository *repository, QObject *parent = nullptr){

    }
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
//        setSectionId(_sectionId);
        _repository = repository;
    }
//    Q_INVOKABLE QStringList getSpecificSentenceFromMap(QString _drugId);
//    Q_INVOKABLE void addSpecificSentenceToMap(QString _drugId, QStringList _specificSentence);
//    Q_INVOKABLE void addSentenceToSpecificSentencesList(QString _drugId, QString _sentence);
//    Q_INVOKABLE void removeSentenceFromSpecificSentencesList(QString _drugId, int _listIndex);

public:

    Q_INVOKABLE QStringList getSentencesList(QString _drugId);
    Q_INVOKABLE void addSentenceToSentencesList(QString _drugId, QString _sentence);
    Q_INVOKABLE void removeSentenceFromSentencesList(QString _drugId, int _listIndex);

private :

    void addSentences(QString _drugId);

private:
    QMap<QString, QStringList> _specificSentencesMap;
    ezechiel::core::AbstractRepository *_repository;
};

class SentencesPalettes : public ezechiel::core::Entity
{

    Q_OBJECT

    ENTITY_UTILS(SentencesPalettes)

    AUTO_PROPERTY_DECL(QStringList, globalSentences, GlobalSentences)
    AUTO_PROPERTY_DECL(QList<Section*>, sectionsList, SectionsList)

    public:

    Q_INVOKABLE explicit SentencesPalettes(ezechiel::core::AbstractRepository *repository, QObject *parent = nullptr){
        for (int i=0; i < VALIDATION_SECTIONS; i++){
            _sectionsList.push_back(ezechiel::core::CoreFactory::createEntity<Section>(repository));
        }
    }

    Q_INVOKABLE void addSentenceToGlobal(QString _sentence);
    Q_INVOKABLE void removeSentenceFromGlobal(int _listIndex);
};


//class SentencesPalettesS : public ezechiel::core::Entity
//{

//    Q_OBJECT

//    ENTITY_UTILS(SentencesPalettesS)

//    AUTO_PROPERTY_DECL(QStringList, globalSentences, GlobalSentences)
//    AUTO_PROPERTY_DECL(QList<QObject*>, listPalettes, ListPalettes)

//    signals:
//        Q_SIGNAL void objListChanged (QList<QObject*>* objList);

//    public:

//        Q_INVOKABLE explicit SentencesPalettesS(ezechiel::core::AbstractRepository *repository, QObject *parent = nullptr){
//            // expectedness, suitability, etc ...
//            for (int i=0; i < VALIDATION_SECTIONS; i++){
//                SentencesPalette *_sp = ezechiel::core::CoreFactory::createEntity<SentencesPalette>(repository);
//                _listPalettes.push_back(_sp);
//            }
//        }

//    Q_INVOKABLE void addSentenceToGlobal(QString _sentence);
//    Q_INVOKABLE void removeSentenceFromGlobal(int _listIndex);
//    Q_INVOKABLE QObject* getPalette(const int _i){
//        return _listPalettes[_i];
//    }

//        void loadSentencesSettings();

//private:
//        Tucuxi::Common::XmlDocument m_doc;
////        QList<SentencesPalette*> m_sp;
//};

//QML_POINTERLIST_CLASS_DECL(SentencesPalettes2, SentencesPalette)

#endif // SENTENCEPALETTE_H
