#ifndef VALIDATIONTABCONTROLLER_H
#define VALIDATIONTABCONTROLLER_H

#include <QObject>
#include <QDateTime>

#include "core/utils/autoproperty.h"
#include "abstractviewcontroller.h"


class ValidationTabController : public AbstractViewController
{
    Q_OBJECT

public:
    explicit ValidationTabController(QObject *parent = nullptr);

    // These two fields are actually not used at all.
//    STD_PROPERTY_DECL(QString, posology, Posology)
//    STD_PROPERTY_DECL(QDateTime, nextControlDate, nextControlDate)

public:

private:
};


#endif // VALIDATIONTABCONTROLLER_H