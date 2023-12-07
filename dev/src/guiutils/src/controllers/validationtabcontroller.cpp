//@@license@@

#include "validationtabcontroller.h"


//STD_PROPERTY_IMPL(ValidationTabController, QString, posology, Posology)
//STD_PROPERTY_IMPL(ValidationTabController, QDateTime, nextControlDate, nextControlDate)

using namespace Tucuxi::Gui::GuiUtils;

ValidationTabController::ValidationTabController(QObject *parent)
    : AbstractViewController(parent)
{
}


QString ValidationTabController::getShortCutText(int section, int key, int modifiers, QString drugId)
{
    if ((modifiers & Qt::AltModifier) || (modifiers & Qt::ControlModifier) || (modifiers & Qt::ShiftModifier) || (modifiers & Qt::MetaModifier)) {
            QString s = _sentencesPalettes->getSection(section)->getSentencePerKey(key, modifiers, drugId);
            return s;
    }
    return "";
}

bool ValidationTabController::validateSentenceShortcut(int sectionId, int key, int modifiers, QString text, QString drugId){
    auto _section = _sentencesPalettes->getSection(sectionId);

    // Global Sentences
    for(auto const &sentences : _section->getGlobalSentences()){
        if(modifiers == sentences->getModifier() && key == sentences->getKey() && text != sentences->getText()){
            return false;
        }
    }
    // Specific Sentences
    if(drugId != ""){
        for(auto const &drug : _section->getSpecificSentences()){
            if(drugId == drug->getDrugId()){
                for(auto const &sentences : drug->getSentences()){
                    if(modifiers == sentences->getModifier() && key == sentences->getKey() && text != sentences->getText()){
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

bool ValidationTabController::validateText(int sectionId, QString text, QString drugId){
    auto _section = _sentencesPalettes->getSection(sectionId);

    // Global Sentences
    for(auto const &sentences : _section->getGlobalSentences()){
        if(text == sentences->getText()){
            return false;
        }
    }
    // Specific Sentences
    if(drugId != ""){
        for(auto const &drug : _section->getSpecificSentences()){
            if(drugId == drug->getDrugId()){
                for(auto const &sentences : drug->getSentences()){
                    if(text == sentences->getText()){
                        return false;
                    }
                }
            }
        }
    }
    return true;
}
