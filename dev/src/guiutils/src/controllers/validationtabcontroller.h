//@@license@@

#ifndef VALIDATIONTABCONTROLLER_H
#define VALIDATIONTABCONTROLLER_H

#include <QObject>
#include <QDateTime>

#include "core/utils/autoproperty.h"
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

    // These two fields are actually not used at all.
//    STD_PROPERTY_DECL(QString, posology, Posology)
//    STD_PROPERTY_DECL(QDateTime, nextControlDate, nextControlDate)

};

}
}
}

#endif // VALIDATIONTABCONTROLLER_H
