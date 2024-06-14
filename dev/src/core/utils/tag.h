/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
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


#ifndef TAG_H
#define TAG_H

#include <QCoreApplication>

//Used by cmake when pre-processing this file
#ifndef PREPROC_ONLY
#include <QString>
#include <QObject>
#endif

namespace Tucuxi {
namespace Gui {
namespace Core {



namespace DrugTag {

    #undef DECLARE_DRUG_TAG
    #define DECLARE_DRUG_TAG(x, t, n) x,

    /** \brief Enumeration of the drug XML tags.
     * The listing only includes the tags used in more than one parser, or used by other classes.
     */
    enum Tag {
        #include "drugtags.cpp"
        UNKOWN_TAG
    };

    #undef DECLARE_DRUG_ATTRIBUTE
    #define DECLARE_DRUG_ATTRIBUTE(x, a) x,

    /** \brief Enumeration of the drug XML attributes.
     * The listing only includes the attributes used in more than one parser, or used by other classes.
     */
    enum Attribute {
        #include "drugattributes.cpp"
        UNKOWN_ATTR
    };
}

class Tag
{
    Q_DECLARE_TR_FUNCTIONS(Tag)

public:

    #undef DECLARE_DRUG_TAG
    #define DECLARE_DRUG_TAG(x, t, n) if (name == #t) return DrugTag::x;

    /** \brief Returns a drug tag.
     * @param name A tag name.
     * @return The corresponding tag.
     */
    static DrugTag::Tag drugTag(QString name)
    {
        #include "drugtags.cpp"
        return DrugTag::UNKOWN_TAG;
    }

    #undef DECLARE_DRUG_TAG
    #define DECLARE_DRUG_TAG(x, t, n) case DrugTag::x: return #t;

    /** \brief Returns a drug's tag name.
     * @param tag A drug tag.
     * @return The tag name.
     */
    static QString name(DrugTag::Tag tag)
    {
        switch (tag) {
        #include "drugtags.cpp"
        default: return QString();
        }
    }

    #undef DECLARE_DRUG_TAG
    #define DECLARE_DRUG_TAG(x, t, n) case DrugTag::x: return tr(#n);

    /** Returns a translation for the given drug tag.
     * @param tag A drug tag.
     * @return A translated name.
     */
    static QString translate(DrugTag::Tag tag)
    {
        switch (tag) {
        #include "drugtags.cpp"
        default: return QString();
        }
    }

    #undef DECLARE_DRUG_ATTRIBUTE
    #define DECLARE_DRUG_ATTRIBUTE(x, a) case DrugTag::x: return #a;

    /** \brief Returns a drug attribute.
     * @param attr A drug attribute.
     * @return The attribute name.
     */
    static QString attribute(DrugTag::Attribute attr)
    {
        switch (attr) {
        #include "drugattributes.cpp"
        default: return QString();
        }
    }

private:
    //Private constructor
    Tag() {}
};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

#endif // TAG_H
