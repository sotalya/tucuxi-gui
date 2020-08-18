#include "sentencepalette.h"

namespace ezechiel {
namespace core {

SentencesPalette::SentencesPalette(ezechiel::core::AbstractRepository *repository, QObject *parent) :
    Entity(repository, parent)
{
    _globalSentences.push_back("HELLO");
    _globalSentences.push_back("Je suis un cycliste");
    _globalSentences.push_back("Les corbeaux sont beaux");
    _globalSentences.push_back("je veux écrire une longue phrase pour voir si ça retourne à la ligne");

    _drugSpecificSentences.push_back("La vie est belle et plus belle la vie");
    _drugSpecificSentences.push_back("Le bleu est bleu ");
    _drugSpecificSentences.push_back("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    _drugSpecificSentences.push_back("OUAIS OAUIS");
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
