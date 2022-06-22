//@@license@@

#ifndef VALIDATIONTABCONTROLLER_H
#define VALIDATIONTABCONTROLLER_H

#include <QObject>
#include <QDateTime>

#include "core/utils/autoproperty.h"
#include "abstractviewcontroller.h"
#include "admin/src/dal/sentencepalette.h"


class ValidationTabController : public AbstractViewController
{
    Q_OBJECT

public:
    explicit ValidationTabController(QObject *parent = nullptr);

    Q_INVOKABLE QString getShortCutText(int section, int key, int modifiers);

    Tucuxi::Gui::Admin::SentencesPalettes *_sentencesPalettes;

    // These two fields are actually not used at all.
//    STD_PROPERTY_DECL(QString, posology, Posology)
//    STD_PROPERTY_DECL(QDateTime, nextControlDate, nextControlDate)

};


#endif // VALIDATIONTABCONTROLLER_H
