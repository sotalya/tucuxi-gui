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


#ifndef VALIDATIONTABCONTROLLER_H
#define VALIDATIONTABCONTROLLER_H

#include <QObject>
#include <QDateTime>

#include "abstractviewcontroller.h"
#include "admin/src/dal/sentencepalette.h"

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class ValidationTabController : public AbstractViewController
{
    Q_OBJECT

public:
    explicit ValidationTabController(QObject *parent = nullptr);

    Q_INVOKABLE QString getShortCutText(int section, int key, int modifiers, QString drugId);

    ///
    /// \brief validateSentenceShortcut
    /// \param section      The section for which this shortcut is used
    /// \param key          The key used for this shortcut (like A, D, ...)
    /// \param modifiers    The modifiers used for this shortcut (like Shift, Ctrl, ...)
    /// \param text         The text used for this shortcut
    /// \param drugId       The ID of the drug that uses this shortcut (like all, imatinib, ...)
    /// \return             true/false based on the validity of the shortcut
    /// Used from the QML code to validate if a choosen shortcut is already in use for another sentence
    Q_INVOKABLE bool validateSentenceShortcut(int sectionId, int key, int modifiers, QString text, QString drugId, bool isEditing, int _index);

    ///
    /// \brief validateText
    /// \param section      The section from where the text is from
    /// \param text         The text used
    /// \param drugId       The ID of the drug that uses this shortcut (like all, imatinib, ...)
    /// \return             true/false based on the validity of the shortcut
    /// Used from the QML code to validate if a choosen text is already in use in the same section
    Q_INVOKABLE bool validateText(int sectionId, QString text, QString drugId, bool isEditing, int _index);

    Tucuxi::Gui::Admin::SentencesPalettes *_sentencesPalettes;

};

}
}
}

#endif // VALIDATIONTABCONTROLLER_H
