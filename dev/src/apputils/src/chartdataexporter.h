#ifndef CHARTDATAEXPORTER_H
#define CHARTDATAEXPORTER_H

#include <QString>

namespace ezechiel {
namespace core {
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

    QString exportData(const ezechiel::core::ChartData *chartData, int what = EXPORT_ALL);

    QString exportPrediction(ezechiel::core::PredictionResult *pred);
};

class ChartDataHtmlExporter
{
public:
    ChartDataHtmlExporter();

    QString exportData(const ezechiel::core::ChartData *chartData, int what);

    QString exportPrediction(ezechiel::core::PredictionResult *pred);
};

#endif // CHARTDATAEXPORTER_H
