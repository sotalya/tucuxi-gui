#include "sentencepalette.h"
#include "core/settings.h"

namespace ezechiel {
namespace core {

SentencesPalette::SentencesPalette(ezechiel::core::AbstractRepository *repository, QObject *parent) :
    Entity(repository, parent)
{
    loadSentencesSettings();
}

void SentencesPalette::loadSentencesSettings(){
    // i correspond to validationSection index (expectedness, suitability, etc ...)
    // Theses sections are appened to the list in interpretationController
    static uint8_t i = 0;

    QMap<QString, QVariant> sentencesPalettes = SETTINGS.get(ezechiel::core::Module::GUI, "sentencesPalettes").toMap();
    QList<QVariant> sentencesList = sentencesPalettes[QString::fromStdString(std::to_string(i))].toList();

    if (sentencesList.size() == 2){
        _globalSentences = sentencesList.at(0).toStringList();
        _drugSpecificSentences = sentencesList.at(1).toStringList();
    }
    i++;
}

void SentencesPalette::saveSentencesSettings(int _index){
    QList<QVariant> sentencesList;

    sentencesList.append(_globalSentences);
    sentencesList.append(_drugSpecificSentences);

    //This map correspond to SentencesPalettes. Must be static
    static QMap<QString, QVariant> _sentencesPaletteMap;

    _sentencesPaletteMap[QString::fromStdString(std::to_string(_index))] = sentencesList;

    SETTINGS.set(ezechiel::core::Module::GUI, "sentencesPalettes", _sentencesPaletteMap);
}


void SentencesPalette::addSentenceToGlobal(QString _sentence){
    _globalSentences.push_back(_sentence);
}

void SentencesPalette::addSentenceToSpecific(QString _sentence){
    _drugSpecificSentences.push_back(_sentence);
}

void SentencesPalette::removeSentenceFromSpecific(int _listIndex){
    _drugSpecificSentences.removeAt(_listIndex);
}

void SentencesPalette::removeSentenceFromGlobal(int _listIndex){
    _globalSentences.removeAt(_listIndex);
}

AUTO_PROPERTY_IMPL(SentencesPalette, QStringList, globalSentences, GlobalSentences)
AUTO_PROPERTY_IMPL(SentencesPalette, QStringList, drugSpecificSentences, DrugSpecificSentences)

QML_POINTERLIST_CLASS_IMPL(SentencesPalettes, SentencesPalette)

}
}
