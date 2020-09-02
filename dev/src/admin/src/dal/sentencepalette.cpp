#include "sentencepalette.h"
#include "core/settings.h"
#include "tucucommon/xmlnode.h"
#include <sstream>
#include <fstream>

//void SentencesPalette::loadSentencesSettings(){
//    // i correspond to validationSection index (expectedness, suitability, etc ...)
//    // Theses sections are appened to the list in interpretationController
//    static uint8_t i = 0;

//    QList<QVariant> sentencePalettes = SETTINGS.get(ezechiel::core::Module::GUI, "sentencesPalette").toList();

//    if (sentencePalettes.size() != 2){
//        _drugSpecificSentences = sentencePalettes[0].toMap()[QString::fromStdString(std::to_string(i))].toStringList();
//        _globalSentences = sentencePalettes[1].toStringList();
//    }




//    QMap<QString, QVariant> sentencesPalettes = SETTINGS.get(ezechiel::core::Module::GUI, "sentencesPalettes").toMap();
//    QList<QVariant> sentencesList = sentencesPalettes[QString::fromStdString(std::to_string(i))].toList();

//    if (sentencesList.size() == 2){
//        _globalSentences = sentencesList.at(0).toStringList();
//        _drugSpecificSentences = sentencesList.at(1).toStringList();
//    }
//    i++;
//}

//void SentencesPalette::saveSentencesSettings(int _index, QString _drugId){
//    QList<QVariant> sentencesList;

//    sentencesList.append(_globalSentences);
//    sentencesList.append(_drugSpecificSentences);

//    This map correspond to SentencesPalettes. Must be static
//    static QMap<QString, QVariant> _sentencesPaletteMap;

//    _sentencesPaletteMap[QString::fromStdString(std::to_string(_index))] = _drugSpecificSentences;
//    sentencesList.append(_sentencesPaletteMap);
//    sentencesList.append(_globalSentences);

//    SETTINGS.set(ezechiel::core::Module::GUI, "sentencesPalette", sentencesList);

//    std::string fileName = "/home/johan/Bureau/sentence.xml";
//    std::string xmlString;

//    Tucuxi::Common::XmlNode root = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element, "SentencesPalettes");

//    m_doc.setRoot(root);

//    Tucuxi::Common::XmlNode globalList = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
//                                                       "gloablSentenceList", "gloablSentenceList");
//    root.addChild(globalList);

//    Tucuxi::Common::XmlNode trigger = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
//                                                       "trigger", "");
//    globalList.addChild(trigger);

//    for(auto const &sp : getListPalette()){
//        Tucuxi::Common::XmlNode section = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
//                                                           "expectedness", "expectedness");
//        root.addChild(section);

//        for
//    }

//    for (const auto &sentences : _globalSentences){
//        Tucuxi::Common::XmlNode globalSentence = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
//                                                           "sentence", sentences.toStdString());
//        globalList.addChild(globalSentence);

//    }

//    Tucuxi::Common::XmlNode specificList = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
//                                                       "specificSentenceList", "specificSentenceList");
//    root.addChild(specificList);

//    Tucuxi::Common::XmlNode drugId = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
//                                                       "drugId", _drugId.toStdString());
//    specificList.addChild(drugId);

//    Tucuxi::Common::XmlNode trigger2 = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
//                                                       "trigger", "");
//    specificList.addChild(trigger2);

//    for (const auto &sentences : _drugSpecificSentences){
//        Tucuxi::Common::XmlNode specificSentence = m_doc.createNode(Tucuxi::Common::EXmlNodeType::Element,
//                                                           "sentence", sentences.toStdString());
//        specificList.addChild(specificSentence);

//    }

//    m_doc.toString(xmlString, true);

//    std::ofstream file;
//    file.open(fileName);
//    if ((file.rdstate() & std::ostream::failbit) != 0) {
//    }
//    file << xmlString;
//    file.close();
//}

//void SentencesPalette::addSentenceToSpecific(QString _sentence){
//    _drugSpecificSentences.push_back(_sentence);
//}

//void SentencesPalette::removeSentenceFromSpecific(int _listIndex){
//    _drugSpecificSentences.removeAt(_listIndex);
//}


//void SentencesPalettesS::loadSentencesSettings(){
//     i correspond to validationSection index (expectedness, suitability, etc ...)
//     Theses sections are appened to the list in interpretationController

//    QList<QVariant> sentencePalettes = SETTINGS.get(ezechiel::core::Module::GUI, "sentencesPalette").toList();



//    _globalSentences =




//    QMap<QString, QVariant> sentencesPalettes = SETTINGS.get(ezechiel::core::Module::GUI, "sentencesPalettes").toMap();
//    QList<QVariant> sentencesList = sentencesPalettes[QString::fromStdString(std::to_string(i))].toList();

//    if (sentencesList.size() == 2){
//        _globalSentences = sentencesList.at(0).toStringList();
//        _drugSpecificSentences = sentencesList.at(1).toStringList();
//    }
//}

//void SentencesPalettesS::addSentenceToGlobal(QString _sentence){
//    _globalSentences.push_back(_sentence);
//}

//void SentencesPalettesS::removeSentenceFromGlobal(int _listIndex){
//    _globalSentences.removeAt(_listIndex);
//}

void Sentences::removeSentenceFromSpecific(int _listIndex){
    _sentences.removeAt(_listIndex);
}

void Sentences::addSentenceToSpecific(QString _sentence){
    _sentences.append(_sentence);
}

//QStringList Section::getSpecificSentenceFromMap(QString _drugId){
//    if(_specificSentencesMap[_drugId].size() != 0){
//        return _specificSentencesMap[_drugId];
//    }
//    else{
//        QStringList sl;
//        addSpecificSentenceToMap(_drugId, sl);
//        return sl;
//    }
//}

//void Section::addSpecificSentenceToMap(QString _drugId, QStringList _specificSentence){
//    _specificSentencesMap[_drugId] = _specificSentence;
//}

//void Section::addSentenceToSpecificSentencesList(QString _drugId, QString _sentence){
//    getSpecificSentenceFromMap(_drugId).append(_sentence);
//}

//void Section::removeSentenceFromSpecificSentencesList(QString _drugId, int _listIndex){
//    getSpecificSentenceFromMap(_drugId).removeAt(_listIndex);
//}

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

void SentencesPalettes::addSentenceToGlobal(QString _sentence){
    _globalSentences.push_back(_sentence);
}

void SentencesPalettes::removeSentenceFromGlobal(int _listIndex){
    _globalSentences.removeAt(_listIndex);
}




//AUTO_PROPERTY_IMPL(SentencesPalette, QStringList, drugSpecificSentences, DrugSpecificSentences)
//AUTO_PROPERTY_IMPL(SentencesPalette, QList<QStringList>, drugList, DrugList)
//AUTO_PROPERTY_IMPL(SentencesPalettesS, QStringList, globalSentences, GlobalSentences)
//AUTO_PROPERTY_IMPL(SentencesPalettesS, QList<QObject*>, listPalettes, ListPalettes)
//QML_POINTERLIST_CLASS_IMPL(SentencesPalettes2, SentencesPalette)


AUTO_PROPERTY_IMPL(Sentences, QString, drugId, DrugId)
AUTO_PROPERTY_IMPL(Sentences, QStringList, sentences, Sentences)
AUTO_PROPERTY_IMPL(Section, QString, sectionid, SectionId)
AUTO_PROPERTY_IMPL(Section, QStringList, globalSentences, GlobalSentences)
AUTO_PROPERTY_IMPL(Section, QList<Sentences*>, specificSentences, SpecificSentences)
AUTO_PROPERTY_IMPL(SentencesPalettes, QStringList, globalSentences, GlobalSentences)
AUTO_PROPERTY_IMPL(SentencesPalettes, QList<Section*>, sectionsList, SectionsList)

