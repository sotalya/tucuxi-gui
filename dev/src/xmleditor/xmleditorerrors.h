#ifndef XMLEDITORERRORS_H
#define XMLEDITORERRORS_H

//Used by cmake when pre-processing this file
#ifndef PREPROC_ONLY
#include <QString>
#include <QObject>
#endif

//XML editor errors enumeration
#undef DECLARE_XML_EDITOR_ERROR
#define DECLARE_XML_EDITOR_ERROR(x, i) x = i,

/** \ingroup errors
 * Enumeration of the XML editor.
 */
enum XmlEditorError {
    #include "xmleditorerrors.cpp"
    //! Last possible XML editor error (not to be used).
    XMLEDITORLASTERROR = 250
};

/** \ingroup xmleditor
 *  \brief Contains information about the XML Editor's errors.
 *  \sa CoreErrorInfo
 */
class XmlEditorErrorInfo
{

public:

    #undef DECLARE_XML_EDITOR_ERROR
    #define DECLARE_XML_EDITOR_ERROR(x, i) case i : return #x;

    /** \brief Returns the XML editor's error name.
     *  \sa CoreErrorInfo::name
     */
    static QString name(XmlEditorError d)
    {
        switch (d) {
         #include "xmleditorerrors.cpp"
        default: return "UNKNOWNERROR";
        }
    }

    #undef DECLARE_XML_EDITOR_ERROR
    #define DECLARE_XML_EDITOR_ERROR(x, i) case i : return QObject::tr(#x);

    /** \brief Returns the possibly translated XML editor's error name.
     *  \sa CoreErrorInfo::desc
     */
    static QString desc(XmlEditorError d)
    {
        switch (d) {
        #include "xmleditorerrors.cpp"
        default: return QObject::tr("UNKNOWNERROR");
        }
    }
};

#endif // XMLEDITORERRORS_H
