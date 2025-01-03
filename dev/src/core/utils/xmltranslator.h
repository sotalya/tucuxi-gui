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


#ifndef DOMTRANSLATOR_H
#define DOMTRANSLATOR_H

#include <QString>
#include <QDomElement>
#include <QXmlStreamReader>

namespace Tucuxi {
namespace Gui {
namespace Core {



/** \ingroup xmleditor
 *  \brief Find the translation of an XML element.
 *
 * The XML files used by Tucuxi can sometimes provide an element's text in different languages.
 * The structure of those elements is always the same and is as follows:
 * \code
     <tags>
       <tag lang='en'>Text</tag>
       <tag lang='fr'>Texte</tag>
       <tag lang='it'>Testo</tag>
       ...
     </tags>
 * \endcode
 * In order to retrieve the translation corresponding to the language used by the application
 * (or the one passed to the translate functions), the translator will iterate through the children
 * of the current element (<tags> in the example above) and will stop as soon as the text matching
 * the correct language is found.
 *
 * If the text cannot be found, the translator will return the text in english. If it also cannot
 * be fount, then the translator will return the text of the first children it founds. If the element
 * to translate has no children, the translator will return an empty string.
 *
 * The translator can translate the text of a DOM element as well as a XML stream reader element.
 */
class XmlTranslator
{

public:

    //! Constructor.
    /** The default language used by the XML translator is the one use by the Core.
     * It is possible to set another language upon the creation of a new translator.
     * @param language An optional language.
     */
    XmlTranslator(const QString &lanugage = QString());

    //! Translates the given DOM.
    /** Translates the given DOM. The language is searched in the attribute with the gieven name.
     * @param element The element to translate.
     * @return The translated text of the element.
     */
    QString translate(QDomElement element, const QString &langAttribute) const;

    //! Translates the current element of a XML stream reader.
    /** Translates the current element of a XML stream reader. The language is searched in the
     * attribute with the given name.
     * @param parser The parser pointing to the element to translate.
     * @return The translated text of the element.
     */
    QString translate(QXmlStreamReader &parser, const QString &langAttribute) const;

private:
    //Language used by the translator
    QString _language;
};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

#endif // DOMTRANSLATOR_H
