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

bool ValidationTabController::validateSentenceShortcut(int sectionId, int key, int modifiers, QString text, QString drugId, bool isEditing, int _index){
    auto _section = _sentencesPalettes->getSection(sectionId);
    uint i = 0;


    // Global Sentences
    for(auto const &sentences : _section->getGlobalSentences()){
        if(modifiers == sentences->getModifier() && key == sentences->getKey() && (text != sentences->getText() || !isEditing)){
            if(isEditing && _index != i){
                return false;
            }
            else if(!isEditing){
                return false;
            }
        }
        i++;
    }
    // Specific Sentences
    if(drugId != ""){
        for(auto const &drug : _section->getSpecificSentences()){
            if(drugId == drug->getDrugId()){
                i = 0;
                for(auto const &sentences : drug->getSentences()){
                    if(modifiers == sentences->getModifier() && key == sentences->getKey() && (text != sentences->getText() || !isEditing)){
                        if(isEditing && _index != i){
                            return false;
                        }
                        else if(!isEditing){
                            return false;
                        }
                    }
                    i++;
                }
            }
        }
    }
    else{
        for(auto const &drug : _section->getSpecificSentences()){
            i = 0;
            for(auto const &sentences : drug->getSentences()){
                if(modifiers == sentences->getModifier() && key == sentences->getKey() && (text != sentences->getText() || !isEditing)){
                    if(isEditing && _index != i){
                        return false;
                    }
                    else if(!isEditing){
                        return false;
                    }
                }
                i++;
            }
        }
    }
    return true;
}

bool ValidationTabController::validateText(int sectionId, QString text, QString drugId, bool isEditing, int _index){
    auto _section = _sentencesPalettes->getSection(sectionId);

    // Global Sentences
    for(auto const &sentences : _section->getGlobalSentences()){
        if(text == sentences->getText() && !isEditing){
            return false;
        }
    }
    // Specific Sentences
    if(drugId != ""){
        for(auto const &drug : _section->getSpecificSentences()){
            if(drugId == drug->getDrugId()){
                for(auto const &sentences : drug->getSentences()){
                    if(text == sentences->getText() && !isEditing){
                        return false;
                    }
                }
            }
        }
    }
    else{
        for(auto const &drug : _section->getSpecificSentences()){
            for(auto const &sentences : drug->getSentences()){
                if(text == sentences->getText() && !isEditing){
                    return false;
                }
            }
        }
    }
    return true;
}
