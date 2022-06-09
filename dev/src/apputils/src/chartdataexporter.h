#ifndef CHARTDATAEXPORTER_H
#define CHARTDATAEXPORTER_H

#include <QString>

namespace ezechiel {
namespace GuiCore {
class ChartData;
class PredictionResult;
}
}


#define EXPORT_APRIORI      0x1
#define EXPORT_APOSTERIORI  0x2
#define EXPORT_POPULATION   0x4
#define EXPORT_ADJUSTMENTS  0x8
#define EXPORT_ALL          0xF

class ChartDataExporter
{
public:
    ChartDataExporter();

    QString exportData(const ezechiel::GuiCore::ChartData *chartData, int what = EXPORT_ALL);

    QString exportPrediction(ezechiel::GuiCore::PredictionResult *pred);
};

class ChartDataHtmlExporter
{
public:
    ChartDataHtmlExporter();

    QString exportData(const ezechiel::GuiCore::ChartData *chartData, int what);

    QString exportPrediction(ezechiel::GuiCore::PredictionResult *pred);
};

#endif // CHARTDATAEXPORTER_H
