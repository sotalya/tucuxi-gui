//@@license@@

#include "core/utils/xmltranslator.h"
#include "tag.h"
#include "core/core.h"
#include <QRegularExpression>
#include <QString>

namespace Tucuxi {
namespace Gui {
namespace Core {



//Default constructor
XmlTranslator::XmlTranslator(const QString &language) : _language(language)
{
    //Set the default language if none was specified to the constructor
    if (_language.isEmpty())
        _language = CORE->language();
}

//Translates a QDomeElement
QString XmlTranslator::translate(QDomElement element, const QString &langAttribute) const
{
    //Holder for the translated text
    QString translation;

    //Get the first child
    QDomElement child = element.firstChildElement();

    //Iterate through the children
    while (!child.isNull()) {

        //If the translation is found, return it
        if (child.attribute(langAttribute) == _language)
            return child.firstChild().toText().data();

        //If english is found, or if still empty, store it
        if (child.attribute(langAttribute) == "en" || translation.isEmpty())
            translation = child.firstChild().toText().data();

        //Move to the next child element
        child = child.nextSiblingElement();
    }

    return translation;
}

//Translates a QXmlStreamReader element
QString XmlTranslator::translate(QXmlStreamReader &parser, const QString &langAttribute) const
{
    //Holder for the translated text
    QString translation;

    //Used to skip the remaining children
    bool found = false;

    //Cylce through the current element's children
    while (!parser.hasError() && parser.readNextStartElement()) {

        //If the translation is found, set the text and skip the remaining children
        if (parser.attributes().value(langAttribute) == _language) {
            translation = parser.readElementText();
            found = true;
        }

        //Else if the english translation is found, set the text and continue the search
        else if (!found && parser.attributes().value(langAttribute).contains(QRegularExpression("^en$")))
            translation = parser.readElementText();

        //Else if the translation is still empty, set it and continue the search
        else if (!found && translation.isEmpty())
            translation = parser.readElementText();

        //Else skip the current element
        else
            parser.skipCurrentElement();
    }

    return translation;
}

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

