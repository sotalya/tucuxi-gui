#include "chartdataexporter.h"

#include "core/dal/chartdata.h"
#include "core/dal/predictionresult.h"
#include "core/dal/predictiondata.h"

using namespace ezechiel::GuiCore;

ChartDataExporter::ChartDataExporter()
{

}

QString ChartDataExporter::exportPrediction(PredictionResult *pred)
{
    QString content = "";
    PredictionData *data = pred->getPredictive()->getPredictionData();

    content += QString("Cycles statistics\n\n");
    content += QString("Cycle Nb.;mean;auc;cumulativeAuc;trough;peak\n");

    for(int statIndex = 0; statIndex < data->getNbCycles(); statIndex ++) {
        CycleStatistics stat = data->getCycleStatistics(statIndex);
        content += QString("%1;%2;%3;%4;%5;%6\n").arg(statIndex).arg(stat.mean)
                .arg(stat.auc).arg(stat.cumulatedAuc).arg(stat.trough).arg(stat.peak);
    }

    content += QString("\n\n");

    return content;
}

QString ChartDataExporter::exportData(const ezechiel::GuiCore::ChartData *chartData, int what)
{
    QString content;
    content += QString("File generated at date :;%1\n\n").arg(QDateTime::currentDateTime().toString(Qt::ISODate));
    PredictionResult *apoPred = chartData->getApoPred();
    if ((what & EXPORT_APOSTERIORI) && (apoPred != nullptr)) {
        content += QString("--- A posteriori data---\n\n");
        std::vector<MeasureValues> measureValues = apoPred->getPredictive()->getPredictionData()->getMeasureValues();
        content += QString("Measures\n\n");
        content += QString("Date;Measure;Prediction\n");
        for (const auto &m : measureValues) {
            content += QString("%1;%2;%3\n").arg(m.m_time.toString(Qt::ISODate)).arg(m.m_measureValue).arg(m.m_predictedValue);
        }
        content += QString("\n\n");

        content += exportPrediction(apoPred);
    }
    PredictionResult *aprPred = chartData->getAprPred();
    if ((what & EXPORT_APRIORI) && (aprPred != nullptr)) {
        content += QString("--- A priori data---\n\n");

        content += exportPrediction(aprPred);
    }
    PredictionResult *popPred = chartData->getPopPred();
    if ((what & EXPORT_POPULATION) && (popPred != nullptr)) {
        content += QString("--- Population data---\n\n");

        content += exportPrediction(popPred);
    }

    PredictionResult *adjPred = chartData->getAdjPred();
    if ((what & EXPORT_ADJUSTMENTS) && (adjPred != nullptr)) {
        content += QString("--- Adjustment data---\n\n");

        content += exportPrediction(adjPred);
    }

    return content;
}


ChartDataHtmlExporter::ChartDataHtmlExporter()
{

}

QString ChartDataHtmlExporter::exportPrediction(PredictionResult *pred)
{
    QString content = "";
    PredictionData *data = pred->getPredictive()->getPredictionData();

    content += "<table>";
    content += QString("<tr><td colspan=6>Cycles statistics</td></tr>\n");
    content += QString("<tr><td>Cycle Nb.</td><td>mean</td><td>auc</td><td>cumulativeAuc</td><td>trough</td><td>peak</td></tr>\n");

    for(int statIndex = 0; statIndex < data->getNbCycles(); statIndex ++) {
        CycleStatistics stat = data->getCycleStatistics(statIndex);
        content += QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5</td><td>%6</td></tr>\n").arg(statIndex).arg(stat.mean)
                .arg(stat.auc).arg(stat.cumulatedAuc).arg(stat.trough).arg(stat.peak);
    }
    content += "</table>";

    content += QString("\n\n");

    return content;
}

QString ChartDataHtmlExporter::exportData(const ezechiel::GuiCore::ChartData *chartData, int what)
{
    QString content;
    content += "<html>";

    PredictionResult *apoPred = chartData->getApoPred();
    if ((what & EXPORT_APOSTERIORI) && (apoPred != nullptr)) {
        content += QString("--- A posteriori data---\n\n");
        content += "<table>";
        std::vector<MeasureValues> measureValues = apoPred->getPredictive()->getPredictionData()->getMeasureValues();
        content += QString("<tr><td colspan=3>Measures</td></tr>\n");
        content += QString("<tr><td>Date</td><td>Measure</td><td>Prediction</td></tr>\n");
        for (const auto &m : measureValues) {
            content += QString("<tr><td>%1</td><td>%2</td><td>%3</td></tr>\n").arg(m.m_time.toString(Qt::ISODate)).arg(m.m_measureValue).arg(m.m_predictedValue);
        }
        content += "</table>";
        content += QString("\n\n");

        content += exportPrediction(apoPred);
    }

    PredictionResult *aprPred = chartData->getAprPred();
    if ((what & EXPORT_APRIORI) && (aprPred != nullptr)) {
        content += QString("--- A priori data---\n\n");

        content += exportPrediction(aprPred);
    }

    PredictionResult *popPred = chartData->getPopPred();
    if ((what & EXPORT_POPULATION) && (popPred != nullptr)) {
        content += QString("--- Population data---\n\n");

        content += exportPrediction(popPred);
    }

    PredictionResult *adjPred = chartData->getAdjPred();
    if ((what & EXPORT_ADJUSTMENTS) && (adjPred != nullptr)) {
        content += QString("--- Adjustment data---\n\n");

        content += exportPrediction(adjPred);
    }

    content += "</html>";

    return content;
}
