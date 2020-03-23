#include "chartcolormanager.h"

const QMap<ParameterType, QList<QColor> > ChartColorManager::refColors = initColors();

ChartColorManager::ChartColorManager()
{

}

QColor ChartColorManager::getDefaultColor(ParameterType pType)
{
    return refColors[pType].first();
}

QColor ChartColorManager::getFreeColor(ParameterType mainCurveType)
{
    // Try and find the first free curve
    foreach (QColor c, refColors[mainCurveType]) {
        if (!usedColors.contains(c)) {
            usedColors.append(c);
            return c;
        }
    }
    return QColor();
}

void ChartColorManager::freeColor(QColor color)
{
    usedColors.removeAll(color);
}

QMap<ParameterType, QList<QColor> > ChartColorManager::initColors()
{
    QMap<ParameterType, QList<QColor> > colors;
    colors[APOSTERIORI] << QColor(30,125,0) << QColor(60,125,0) << QColor(90,125,0);
    colors[APRIORI] << QColor(0,30,179) << QColor(0,60,179) << QColor(0,90,179);
    colors[POPULATION] << QColor(119,0,179) << QColor(149,0,179) << QColor(179,0,179);
    colors[MANUAL] << QColor(179,30,0) << QColor(179,60,0) << QColor(179,90,0);
    return colors;
}
