//@@license@@

#ifndef CHARTCOLORMANAGER_H
#define CHARTCOLORMANAGER_H

#include "../dal/parameters.h"

#include <QColor>

using namespace ezechiel::GuiCore;
class ChartColorManager
{
public:
    explicit ChartColorManager();
    
    // Returns the default color for the given parameter type
    static QColor getDefaultColor(ParameterType pType);

    // Returns a free color depending of the type
    QColor getFreeColor(ParameterType mainCurveType);

    // Free the current color
    void freeColor(QColor color);

private:
    static QMap<ParameterType, QList<QColor> > initColors();
    static const QMap<ParameterType, QList<QColor> > refColors;
    QList<QColor> usedColors;
};

#endif // CHARTCOLORMANAGER_H
