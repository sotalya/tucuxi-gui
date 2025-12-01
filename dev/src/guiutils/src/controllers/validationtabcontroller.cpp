/* 
 * Tucuxi - Tucuxi-gui software. 
 * This software is able to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "validationtabcontroller.h"


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
