#include "sentencepalette.h"
#include "core/corerepository.h"
#include "core/settings.h"
#include "qkeysequence.h"
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

Sentence::Sentence(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent)
{
}

Sentence::Sentence(int key, int modifier, QString text) :
    _key(key),
    _modifier(modifier),
    _text(text)
{
}

bool Sentence::operator==(const Sentence& _other) const
{
    return (this->getText() == _other.getText());
}

void DrugSentences::removeSentence(int _listIndex){
    _sentences.removeAt(_listIndex);
}

void DrugSentences::editSentence(int _listIndex, Sentence* _sentence){
    _sentences.removeAt(_listIndex);
    _sentences.insert(_listIndex, _sentence);
}

void DrugSentences::addSentence(Sentence* _sentence){
    _sentences.append(_sentence);
}

Section::Section(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent){
    _repository = repository;
}

void Section::addSentenceToGlobal(int key, int modifier, QString text){
    auto _sentence = Tucuxi::Gui::Core::CoreFactory::createEntity<Sentence>(_repository);
    _sentence->setKey(key);
    _sentence->setModifier(modifier);
    _sentence->setText(text);
    _globalSentences.push_back(_sentence);
}

void Section::editSentenceOfGlobal(int key, int modifier, QString text, int _listIndex){
    auto _sentence = Tucuxi::Gui::Core::CoreFactory::createEntity<Sentence>(_repository);
    _sentence->setKey(key);
    _sentence->setModifier(modifier);
    _sentence->setText(text);
    _globalSentences.removeAt(_listIndex);
    _globalSentences.insert(_listIndex, _sentence);
}

void Section::removeSentenceFromGlobal(int _listIndex){
    _globalSentences.removeAt(_listIndex);
}


QStringList Section::getSpecificSentencesList(QString _drugId){

    QStringList ret;

    for(const auto &sentencesList : _specificSentences){
        if(sentencesList->getDrugId() == _drugId){
            for(const auto &sentence : sentencesList->getSentences()){
                ret.append(sentence->getText());
            }
            return ret;
        }
    }
    addDrugSentences(_drugId);
    for(const auto &sentence : _specificSentences.last()->getSentences()){
        ret.append(sentence->getText());
    }
    return ret;
}

QList<Sentence*> Section::getSentenceFromDrugId(QString _drugId){

    for(const auto &DrugSentences : _specificSentences){
        if(DrugSentences->getDrugId() == _drugId){
            return DrugSentences->getSentences();
        }
    }
}

QStringList Section::getGlobalSentencesTextsList(){
    QStringList ret;

    for(auto sentence : _globalSentences){
        auto a = sentence->getText();
        ret.append(sentence->getText());
    }
    return ret;
}

void Section::addSentenceToDrugSentencesList(QString _drugId, int key, int modifier, QString text){
    auto _sentence = Tucuxi::Gui::Core::CoreFactory::createEntity<Sentence>(_repository);
    _sentence->setKey(key);
    _sentence->setModifier(modifier);
    _sentence->setText(text);
        for(const auto &sentencesList : _specificSentences){
        if(sentencesList->getDrugId() == _drugId){
            sentencesList->addSentence(_sentence);
            return;
        }
    }
    //SHOULD NOT GO THERE
    addDrugSentences(_drugId);
}

void Section::editSentenceOfDrugSentencesList(QString _drugId, int key, int modifier, QString text, int _listIndex){
    auto _sentence = Tucuxi::Gui::Core::CoreFactory::createEntity<Sentence>(_repository);
    _sentence->setKey(key);
    _sentence->setModifier(modifier);
    _sentence->setText(text);
    for(const auto &sentencesList : _specificSentences){
        if(sentencesList->getDrugId() == _drugId){
            sentencesList->editSentence(_listIndex, _sentence);
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

void Section::addSentenceToDrugSentences(QString _drugId, Sentence* _sentence){
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

QString Section::getSentencePerKey(int key, int modifiers, QString drugId)
{
    // Global Sentences
    for(auto const &sentences : _globalSentences){
        if(modifiers == sentences->getModifier() && key == sentences->getKey()){
            return sentences->getText();
        }
    }
    // Specific Sentences
    if(drugId != ""){
        for(auto const &drug : _specificSentences){
            if(drugId == drug->getDrugId()){
                for(auto const &sentences : drug->getSentences()){
                    if(modifiers == sentences->getModifier() && key == sentences->getKey()){
                        return sentences->getText();
                    }
                }
            }
        }
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

            Tucuxi::Common::XmlNode sentences = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
                                                                 "sentences", "sentences");
            palette.addChild(sentences);

            for (const auto &sentence : section->getGlobalSentences()){
                Tucuxi::Common::XmlNode sentenceNode = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
                                                                        "sentence", "sentence");
                sentences.addChild(sentenceNode);

                Tucuxi::Common::XmlNode modifierNode = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
                                                                        "modifier", modifierToString(sentence->getModifier()));
                sentenceNode.addChild(modifierNode);

                Tucuxi::Common::XmlNode keyNode = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
                                                                   "key", keyToString(sentence->getKey()));
                sentenceNode.addChild(keyNode);

                Tucuxi::Common::XmlNode textNode = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
                                                                    "text", sentence->getText().toStdString());
                sentenceNode.addChild(textNode);
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

                Tucuxi::Common::XmlNode sentences = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
                                                                     "sentences", "sentences");
                palette.addChild(sentences);

                for (const auto &sentence : drug->getSentences()){
                    Tucuxi::Common::XmlNode sentenceNode = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
                                                                            "sentence", "sentence");
                    sentences.addChild(sentenceNode);

                    Tucuxi::Common::XmlNode modifierNode = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
                                                                            "modifier", modifierToString(sentence->getModifier()));
                    sentenceNode.addChild(modifierNode);

                    Tucuxi::Common::XmlNode keyNode = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
                                                                       "key", keyToString(sentence->getKey()));
                    sentenceNode.addChild(keyNode);

                    Tucuxi::Common::XmlNode textNode = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
                                                                        "text", sentence->getText().toStdString());
                    sentenceNode.addChild(textNode);
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

std::string SentencesPalettes::modifierToString(int modifier){
    std::string ret = "";

    if(modifier & Qt::ControlModifier){
        ret = "CTRL";
    }
    if (modifier & Qt::ShiftModifier){
        if(ret == ""){
            ret = "SHIFT";
        }
        else{
            ret = ret + " + SHIFT";
        }
    }
#ifdef MACOS
    if (modifier & Qt::MetaModifier){
        if(ret == ""){
            ret = "ALT";
        }
        else{
            ret = ret + " + ALT";
        }
    }
#else
    if (modifier & Qt::AltModifier){
        if(ret == ""){
            ret = "ALT";
        }
        else{
            ret = ret + " + ALT";
        }
    }
#endif

    if (modifier & Qt::NoModifier){
        ret = "";
    }

    return ret;
}

std::string SentencesPalettes::keyToString(int key){
    return QKeySequence(key).toString().toStdString();
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
    static const std::string SENTENCES_NODE                 = "sentences";
    static const std::string SENTENCE_NODE                  = "sentence";
    static const std::string MODIFIER_NODE                  = "modifier";
    static const std::string KEY_NODE                       = "key";
    static const std::string TEXT_NODE                      = "text";

    bool foundDrugInSpecificList = false;

    Tucuxi::Common::XmlNode root = document.getRoot();

    Tucuxi::Common::XmlNodeIterator sentencesPaletteIterator = root.getChildren(SENTENCES_PALETTE_NODE);

    while(sentencesPaletteIterator != Tucuxi::Common::XmlNodeIterator::none()){
        QString sectionId = QString::fromStdString(getChildString(sentencesPaletteIterator, SECTION_ID_NODE));
        QString drugId = QString::fromStdString(getChildString(sentencesPaletteIterator, DRUG_ID_NODE));

        Tucuxi::Common::XmlNodeIterator sentencesIterator = sentencesPaletteIterator->getChildren(SENTENCES_NODE);
        Tucuxi::Common::XmlNodeIterator sentenceIterator = sentencesIterator->getChildren(SENTENCE_NODE);

        for(int i = 0; i< _sentencesPalettes->getSectionsList()->size(); i++) {
            auto section = _sentencesPalettes->getSectionsList()->at(i);
            if(sectionId == section->getSectionId()){
                for(const auto &sentences : section->getSpecificSentences()){
                    if (sentences->getDrugId() == drugId){
                        while(sentenceIterator != Tucuxi::Common::XmlNodeIterator::none()){
                            QString modifier = QString::fromStdString(getChildString(sentenceIterator, MODIFIER_NODE));
                            QString key = QString::fromStdString(getChildString(sentenceIterator, KEY_NODE));
                            QString text = QString::fromStdString(getChildString(sentenceIterator, TEXT_NODE));
                            auto xmlSentence = Tucuxi::Gui::Core::CoreFactory::createEntity<Sentence>(REPO);
                            xmlSentence->setKey(stringToKey(key));
                            xmlSentence->setModifier(stringToModifier(modifier));
                            xmlSentence->setText(text);
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
                        QString modifier = QString::fromStdString(getChildString(sentenceIterator, MODIFIER_NODE));
                        QString key = QString::fromStdString(getChildString(sentenceIterator, KEY_NODE));
                        QString text = QString::fromStdString(getChildString(sentenceIterator, TEXT_NODE));
                        Sentence xmlSentence(stringToKey(key), stringToModifier(modifier), text);
                        if (isXMLSentenceExisting(section->getGlobalSentences(), &xmlSentence)){
                            section->addSentenceToGlobal(stringToKey(key), stringToModifier(modifier), text);
                        }
                        sentenceIterator++;
                    }
                }
                // Drug not yet registered
                else if (!foundDrugInSpecificList){
                    while(sentenceIterator != Tucuxi::Common::XmlNodeIterator::none()){
                        QString modifier = QString::fromStdString(getChildString(sentenceIterator, MODIFIER_NODE));
                        QString key = QString::fromStdString(getChildString(sentenceIterator, KEY_NODE));
                        QString text = QString::fromStdString(getChildString(sentenceIterator, TEXT_NODE));
                        auto xmlSentence = Tucuxi::Gui::Core::CoreFactory::createEntity<Sentence>(REPO);
                        xmlSentence->setKey(stringToKey(key));
                        xmlSentence->setModifier(stringToModifier(modifier));
                        xmlSentence->setText(text);
                        section->addSentenceToDrugSentences(drugId, xmlSentence);
                        sentenceIterator++;
                    }
                }
                foundDrugInSpecificList = false;
            }
        }
        sentencesPaletteIterator++;
    }
}

bool SentencesPalettes::SentencesPalettesImporter::isXMLSentenceExisting(QList<Sentence*> _sentence, Sentence* _xmlSentence){
    for (auto const &sentence : std::list(_sentence.begin(), _sentence.end())){
        if(sentence == _xmlSentence){
            return 0;
        }
    }
    return 1;
}

int SentencesPalettes::SentencesPalettesImporter::stringToModifier(QString modifierString){
    int ret = Qt::NoModifier;

    if(modifierString.contains("CTRL", Qt::CaseInsensitive)){
        ret = ret | Qt::ControlModifier;
    }
    if(modifierString.contains("SHIFT", Qt::CaseInsensitive)){
        ret = ret | Qt::ShiftModifier;
    }
#ifdef MACOS
    if(modifierString.contains("ALT", Qt::CaseInsensitive)){
        ret = ret | Qt::MetaModifier;
    }
#else
    if(modifierString.contains("ALT", Qt::CaseInsensitive)){
        ret = ret | Qt::AltModifier;
    }
#endif
    if (modifierString == ""){
        ret = Qt::NoModifier;
    }

    return ret;
}

int SentencesPalettes::SentencesPalettesImporter::stringToKey(QString keyString){
    return QKeySequence(keyString)[0];
}

AUTO_PROPERTY_IMPL(DrugSentences, QString, drugId, DrugId)
AUTO_PROPERTY_IMPL(DrugSentences, QList<Sentence*>, sentences, Sentences)
AUTO_PROPERTY_IMPL(Section, QString, sectionId, SectionId)
AUTO_PROPERTY_IMPL(Section, QList<Sentence*>, globalSentences, GlobalSentences)
AUTO_PROPERTY_IMPL(Section, QList<DrugSentences*>, specificSentences, SpecificSentences)
AUTO_PROPERTY_IMPL(SentencesPalettes, QList<Sentence*>, globalSentences, GlobalSentences)
AUTO_PROPERTY_IMPL(SentencesPalettes, SectionList*, sectionsList, SectionsList)
AUTO_PROPERTY_IMPL(SentencesPalettes, QString, filename, Filename)

AUTO_PROPERTY_IMPL(Sentence, int, modifier, Modifier)
AUTO_PROPERTY_IMPL(Sentence, int, key, Key)
AUTO_PROPERTY_IMPL(Sentence, QString, text, Text)
