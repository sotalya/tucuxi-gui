#include "sentencepalette.h"
#include "core/settings.h"
#include "tucucommon/xmlnode.h"
#include "tucucommon/xmliterator.h"
#include <sstream>
#include <fstream>

#include <stdio.h>
#ifdef WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
#include<iostream>

using namespace Tucuxi::Gui::Admin;

QML_POINTERLIST_CLASS_IMPL(SectionList, Section)

void DrugSentences::removeSentence(int _listIndex){
    _sentences.removeAt(_listIndex);
}

void DrugSentences::addSentence(QString _sentence){
    _sentences.append(_sentence);
}

Section::Section(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent){
    _repository = repository;
}

void Section::addSentenceToGlobal(QString _sentence){
    _globalSentences.push_back(_sentence);
}

void Section::removeSentenceFromGlobal(int _listIndex){
    _globalSentences.removeAt(_listIndex);
}


QStringList Section::getSpecificSentencesList(QString _drugId){

    for(const auto &sentencesList : _specificSentences){
        if(sentencesList->getDrugId() == _drugId){
            return sentencesList->getSentences();
        }
    }
    addDrugSentences(_drugId);
    return _specificSentences.last()->getSentences();
}

void Section::addSentenceToDrugSentencesList(QString _drugId, QString _sentence){
    for(const auto &sentencesList : _specificSentences){
        if(sentencesList->getDrugId() == _drugId){
            sentencesList->addSentence(_sentence);
            return;
        }
    }
    //SHOULD NOT GO THERE
    addDrugSentences(_drugId);
}

void Section::removeSentenceFromDrugSentencesList(QString _drugId, int _listIndex){
    for(const auto &sentencesList : _specificSentences){
        if(sentencesList->getDrugId() == _drugId){
            sentencesList->removeSentence(_listIndex);
            return;
        }
    }
    //SHOULD NOT GO THERE
    addDrugSentences(_drugId);
}

void Section::addDrugSentences(QString _drugId){
    auto newSentence = Tucuxi::Gui::Core::CoreFactory::createEntity<DrugSentences>(_repository);
    newSentence->setDrugId(_drugId);
    _specificSentences.push_back(newSentence);
}

void Section::addSentenceToDrugSentences(QString _drugId, QString _sentence){
    bool same = false;
    for (const auto &sentence : getSpecificSentences()){
        if (_drugId == sentence->getDrugId()){
            same = true;
            sentence->addSentence(_sentence);
        }
    }
    if(!same){
        addDrugSentences(_drugId);
        _specificSentences.last()->addSentence(_sentence);
    }

}


QString Section::getSentencePerKey(int key)
{
    std::cout << key << std::endl;
    int index = 0;
    for(const auto &sentencesList : _specificSentences){
        for (const auto &sentence: sentencesList->getSentences()) {
            if (key - Qt::Key_0 == index) {
                return sentence;
            }
            index ++;
        }
    }
    for(const auto &sentence : _globalSentences){
        if (key - Qt::Key_0 == index) {
            return sentence;
        }
        index ++;
    }
    return "";
}

SentencesPalettes::SentencesPalettes(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent){
    _sectionsList = Tucuxi::Gui::Core::CoreFactory::createEntity<SectionList>(repository);
    for (int i=0; i < VALIDATION_SECTIONS; i++){
        auto newSection = Tucuxi::Gui::Core::CoreFactory::createEntity<Section>(repository);
        newSection->setSectionId(QString::number(i));
        _sectionsList->append(newSection);
    }
    _filename = loadXMLPath();
    SentencesPalettesImporter s;
    m_sentencesPalettesImporter = &s;
    m_sentencesPalettesImporter->importXml(this, _filename);
}

void SentencesPalettes::manualImport(QString _filename) {
    m_sentencesPalettesImporter->importXml(this, _filename);
}


void SentencesPalettes::addSentenceToGlobal(QString _sentence){
    _globalSentences.push_back(_sentence);
}

void SentencesPalettes::removeSentenceFromGlobal(int _listIndex){
    _globalSentences.removeAt(_listIndex);
}

void SentencesPalettes::exportToXml(){


    std::string xmlString;

    Tucuxi::Common::XmlNode root = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element, "sentencesPalettes");

    m_doc.setRoot(root);

    for(int i = 0; i< _sectionsList->size(); i++) {
        auto section = _sectionsList->at(i);
        if(section->getGlobalSentences().size() != 0){
            //Global
            Tucuxi::Common::XmlNode palette = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element, "sentencesPalette");

            root.addChild(palette);

            Tucuxi::Common::XmlNode sectionId = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
                                                               "sectionId", section->getSectionId().toStdString());
            palette.addChild(sectionId);

            Tucuxi::Common::XmlNode drugId = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
                                                               "drugId", "all");
            palette.addChild(drugId);

            Tucuxi::Common::XmlNode trigger = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
                                                               "trigger", "trigger");
            palette.addChild(trigger);

            Tucuxi::Common::XmlNode sentences = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
                                                               "sentences", "sentences");
            palette.addChild(sentences);

            for (const auto &sentence : section->getGlobalSentences()){
                Tucuxi::Common::XmlNode sentenceNode = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
                                                                        "sentence", sentence.toStdString());
                sentences.addChild(sentenceNode);
            }
        }

        // Specific
        for(const auto &drug : section->getSpecificSentences()){

            if(drug->getSentences().size() != 0){
                Tucuxi::Common::XmlNode palette = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element, "sentencesPalette");

                root.addChild(palette);

                Tucuxi::Common::XmlNode sectionId = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
                                                                   "sectionId", section->getSectionId().toStdString());
                palette.addChild(sectionId);

                Tucuxi::Common::XmlNode drugId = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
                                                                   "drugId", drug->getDrugId().toStdString());
                palette.addChild(drugId);

                Tucuxi::Common::XmlNode trigger = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
                                                                   "trigger", "trigger");
                palette.addChild(trigger);

                Tucuxi::Common::XmlNode sentences = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
                                                                   "sentences", "sentences");
                palette.addChild(sentences);

                for (const auto &sentence : drug->getSentences()){

                    Tucuxi::Common::XmlNode sentenceNode = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
                                                                       "sentence", sentence.toStdString());
                    sentences.addChild(sentenceNode);
                }
            }
        }
    }

    m_doc.toString(xmlString, true);

    std::ofstream file;
    file.open(_filename.toStdString());
    if ((file.rdstate() & std::ostream::failbit) != 0) {
    }
    file << xmlString;
    file.close();
}

void SentencesPalettes::saveXMLPath(){

    SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "sentencesPalettesXML" , _filename);
}

QString SentencesPalettes::loadXMLPath(){
    QString xmlPath = SETTINGS.get(Tucuxi::Gui::Core::Module::GUI, "sentencesPalettesXML").toString();

    return (xmlPath.isEmpty()) ? getDefaultPath() : xmlPath;
}

QString SentencesPalettes::getDefaultPath(){
    char buff[FILENAME_MAX];
    GetCurrentDir( buff, FILENAME_MAX ); // dist folder
    std::string current_working_dir(buff);
    return QString::fromStdString(current_working_dir + "/sentences.xml");
}

bool SentencesPalettes::isPathExisting(QString _name){
      struct stat buffer;
      return (stat (_name.toStdString().c_str(), &buffer) == 0);
}

void SentencesPalettes::SentencesPalettesImporter::importXml(SentencesPalettes *_sentencesPalettes, QString _filename){
    Tucuxi::Common::XmlDocument document;
    if (!document.open(_filename.toStdString())) {
        //no document
        return;
    }

    static const std::string SENTENCES_PALETTE_NODE         = "sentencesPalette";
    static const std::string SECTION_ID_NODE                = "sectionId";
    static const std::string DRUG_ID_NODE                   = "drugId";
    static const std::string TRIGGER_NODE                   = "trigger";
    static const std::string SENTENCES_NODE                 = "sentences";
    static const std::string SENTENCE_NODE                  = "sentence";

    bool foundDrugInSpecificList = false;

    Tucuxi::Common::XmlNode root = document.getRoot();

    Tucuxi::Common::XmlNodeIterator sentencesPaletteIterator = root.getChildren(SENTENCES_PALETTE_NODE);

    while(sentencesPaletteIterator != Tucuxi::Common::XmlNodeIterator::none()){
        QString sectionId = QString::fromStdString(getChildString(sentencesPaletteIterator, SECTION_ID_NODE));
        QString drugId = QString::fromStdString(getChildString(sentencesPaletteIterator, DRUG_ID_NODE));
        QString trigger = QString::fromStdString(getChildString(sentencesPaletteIterator, TRIGGER_NODE));

        Tucuxi::Common::XmlNodeIterator sentencesIterator = sentencesPaletteIterator->getChildren(SENTENCES_NODE);
        Tucuxi::Common::XmlNodeIterator sentenceIterator = sentencesIterator->getChildren(SENTENCE_NODE);


        for(int i = 0; i< _sentencesPalettes->getSectionsList()->size(); i++) {
            auto section = _sentencesPalettes->getSectionsList()->at(i);
            if(sectionId == section->getSectionId()){
                for(const auto &sentences : section->getSpecificSentences()){
                    if (sentences->getDrugId() == drugId){
                        while(sentenceIterator != Tucuxi::Common::XmlNodeIterator::none()){
                            QString xmlSentence = QString::fromStdString(sentenceIterator->getValue());
                            if (isXMLSentenceExisting(sentences->getSentences(), xmlSentence)){
                                sentences->addSentence(xmlSentence);
                            }
                            sentenceIterator++;
                        }
                        foundDrugInSpecificList = true;
                    }
                }
                if(drugId == "all"){
                    while(sentenceIterator != Tucuxi::Common::XmlNodeIterator::none()){
                        QString xmlSentence = QString::fromStdString(sentenceIterator->getValue());
                        if (isXMLSentenceExisting(section->getGlobalSentences(), xmlSentence)){
                            section->addSentenceToGlobal(xmlSentence);
                        }
                        sentenceIterator++;
                    }
                }
                // Drug not yet registered
                else if (!foundDrugInSpecificList){
                    while(sentenceIterator != Tucuxi::Common::XmlNodeIterator::none()){
                        QString sentence = QString::fromStdString(sentenceIterator->getValue());
                        section->addSentenceToDrugSentences(drugId, sentence);
                        sentenceIterator++;
                    }
                }
                foundDrugInSpecificList = false;
            }
        }
        sentencesPaletteIterator++;
    }
}

bool SentencesPalettes::SentencesPalettesImporter::isXMLSentenceExisting(QStringList _sentence, QString _xmlSentence){
    for (auto const &sentence : std::list(_sentence.begin(), _sentence.end())){
        if(sentence == _xmlSentence){
            return 0;
        }
    }
    return 1;
}

AUTO_PROPERTY_IMPL(DrugSentences, QString, drugId, DrugId)
AUTO_PROPERTY_IMPL(DrugSentences, QStringList, sentences, Sentences)
AUTO_PROPERTY_IMPL(Section, QString, sectionId, SectionId)
AUTO_PROPERTY_IMPL(Section, QStringList, globalSentences, GlobalSentences)
AUTO_PROPERTY_IMPL(Section, QList<DrugSentences*>, specificSentences, SpecificSentences)
AUTO_PROPERTY_IMPL(SentencesPalettes, QStringList, globalSentences, GlobalSentences)
AUTO_PROPERTY_IMPL(SentencesPalettes, SectionList*, sectionsList, SectionsList)
AUTO_PROPERTY_IMPL(SentencesPalettes, QString, filename, Filename)

