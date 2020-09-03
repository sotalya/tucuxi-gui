#include "sentencepalette.h"
#include "core/settings.h"
#include "tucucommon/xmlnode.h"
#include "tucucommon/xmliterator.h"
#include <sstream>
#include <fstream>

void Sentences::removeSentenceFromSpecific(int _listIndex){
    _sentences.removeAt(_listIndex);
}

void Sentences::addSentenceToSpecific(QString _sentence){
    _sentences.append(_sentence);
}

void Section::addSentenceToGlobal(QString _sentence){
    _globalSentences.push_back(_sentence);
}

void Section::removeSentenceFromGlobal(int _listIndex){
    _globalSentences.removeAt(_listIndex);
}

QStringList Section::getSentencesList(QString _drugId){

    for(const auto &sentencesList : _specificSentences){
        if(sentencesList->getDrugId() == _drugId){
            return sentencesList->getSentences();
        }
    }
    addSentences(_drugId);
    return _specificSentences.last()->getSentences();
}

void Section::addSentenceToSentencesList(QString _drugId, QString _sentence){
    for(const auto &sentencesList : _specificSentences){
        if(sentencesList->getDrugId() == _drugId){
            sentencesList->addSentenceToSpecific(_sentence);
            return;
        }
    }
    //SHOULD NOT GO THERE
    addSentences(_drugId);
}

void Section::removeSentenceFromSentencesList(QString _drugId, int _listIndex){
    for(const auto &sentencesList : _specificSentences){
        if(sentencesList->getDrugId() == _drugId){
            sentencesList->removeSentenceFromSpecific(_listIndex);
            return;
        }
    }
    //SHOULD NOT GO THERE
    addSentences(_drugId);
}

void Section::addSentences(QString _drugId){
    auto newSentence = ezechiel::core::CoreFactory::createEntity<Sentences>(_repository);
    newSentence->setDrugId(_drugId);
    _specificSentences.push_back(newSentence);
}

void Section::addSentenceToSentences(QString _drugId, QString _sentence){
    bool same = false;
    for (const auto &sentence : getSpecificSentences()){
        if (_drugId == sentence->getDrugId()){
            same = true;
            sentence->addSentenceToSpecific(_sentence);
        }
    }
    if(!same){
        auto newSentence = ezechiel::core::CoreFactory::createEntity<Sentences>(_repository);
        newSentence->setDrugId(_drugId);
        newSentence->addSentenceToSpecific(_sentence);
        _specificSentences.push_back(newSentence);
    }

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

    for(const auto &section : _sectionsList){
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
    file.open(m_filename);
    if ((file.rdstate() & std::ostream::failbit) != 0) {
    }
    file << xmlString;
    file.close();
}

void SentencesPalettes::SentencesPalettesImporter::importXml(SentencesPalettes *_sentencesPalettes, std::string _filename){
    Tucuxi::Common::XmlDocument document;
    if (!document.open(_filename)) {
        //not good
    }

    static const std::string SENTENCES_PALETTE_NODE         = "sentencesPalette";
    static const std::string SECTION_ID_NODE                = "sectionId";
    static const std::string DRUG_ID_NODE                   = "drugId";
    static const std::string TRIGGER_NODE                   = "trigger";
    static const std::string SENTENCES_NODE                 = "sentences";
    static const std::string SENTENCE_NODE                  = "sentence";

    bool foundDrugInSpecificList = false;
    bool globalSentences = false;

    Tucuxi::Common::XmlNode root = document.getRoot();

    Tucuxi::Common::XmlNodeIterator sentencesPaletteIterator = root.getChildren(SENTENCES_PALETTE_NODE);

    while(sentencesPaletteIterator != Tucuxi::Common::XmlNodeIterator::none()){
        QString sectionId = QString::fromStdString(getChildString(sentencesPaletteIterator, SECTION_ID_NODE));
        QString drugId = QString::fromStdString(getChildString(sentencesPaletteIterator, DRUG_ID_NODE));
        QString trigger = QString::fromStdString(getChildString(sentencesPaletteIterator, TRIGGER_NODE));

        Tucuxi::Common::XmlNodeIterator sentencesIterator = sentencesPaletteIterator->getChildren(SENTENCES_NODE);
        Tucuxi::Common::XmlNodeIterator sentenceIterator = sentencesIterator->getChildren(SENTENCE_NODE);

        for(const auto &section : _sentencesPalettes->getSectionsList()){
            if(sectionId == section->getSectionId()){
                for(const auto &sentences : section->getSpecificSentences()){
                    if (sentences->getDrugId() == drugId){
                        while(sentenceIterator != Tucuxi::Common::XmlNodeIterator::none()){
                            QString sentence = QString::fromStdString(sentenceIterator->getValue());
                            sentences->addSentenceToSpecific(sentence);
                            sentenceIterator++;
                        }
                        foundDrugInSpecificList = true;
                    }
                }
                if(drugId == "all"){
                    while(sentenceIterator != Tucuxi::Common::XmlNodeIterator::none()){
                        QString sentence = QString::fromStdString(sentenceIterator->getValue());
                        section->addSentenceToGlobal(sentence);
                        sentenceIterator++;
                    }
                }
                // Drug not yet registered
                else if (!foundDrugInSpecificList){
                    while(sentenceIterator != Tucuxi::Common::XmlNodeIterator::none()){
                        QString sentence = QString::fromStdString(sentenceIterator->getValue());
                        section->addSentenceToSentences(drugId, sentence);
                        sentenceIterator++;
                    }
                }
            }
        }
        sentencesPaletteIterator++;
    }
}


AUTO_PROPERTY_IMPL(Sentences, QString, drugId, DrugId)
AUTO_PROPERTY_IMPL(Sentences, QStringList, sentences, Sentences)
AUTO_PROPERTY_IMPL(Section, QString, sectionid, SectionId)
AUTO_PROPERTY_IMPL(Section, QStringList, globalSentences, GlobalSentences)
AUTO_PROPERTY_IMPL(Section, QList<Sentences*>, specificSentences, SpecificSentences)
AUTO_PROPERTY_IMPL(SentencesPalettes, QStringList, globalSentences, GlobalSentences)
AUTO_PROPERTY_IMPL(SentencesPalettes, QList<Section*>, sectionsList, SectionsList)

