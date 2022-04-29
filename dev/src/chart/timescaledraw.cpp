//@@license@@

#include "timescaledraw.h"

#include "core.h"
#include "coremessagehandler.h"

#include <QDebug>

// Nothing?
TimeScaleDraw::TimeScaleDraw() : QwtScaleDraw()
{
}

// Add the double as seconds in the base time
QwtText TimeScaleDraw::label(double v) const
{
    QDateTime date = QDateTime::fromTime_t((uint) v);
    return date.toString(" d MMM : hh'h'");
}
