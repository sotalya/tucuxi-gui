#include "chartbuildermodule.h"
#include "reportdata.h"
#include "identifiers.h"
//#include "chart.h"

bool ChartBuilderModule::setData(ReportData *data)
{
//    if (_chart == 0)
//        return true;

    //Compute size for A4 landscape
//    double pixelsPerMM = double(_chart->logicalDpiX()) / 25.4;  //1inch =  25.4mm

//    int imageWidth  = int(pixelsPerMM * (290));                 //290mm =~ A4 height
//    int imageHeight = _chart->height() * (double(imageWidth) / double(_chart->width()));

//    QSize size(imageWidth, imageHeight);

    //Build a new white QImage
//    QImage image(size, QImage::Format_RGB32);
//    image.fill(Qt::white);

    //Render and store the chart
//    _chart->renderTo(image);
//    data->setImage(ID::Chart::Image, image);

    return true;
}

void ChartBuilderModule::unsetData(ReportData *data)
{
    data->unsetImage(ID::Chart::Image);
}

//Chart *ChartBuilderModule::chart() const
//{
//    return _chart;
//}

//ChartBuilderModule &ChartBuilderModule::setChart(Chart *chart)
//{
//    _chart = chart;
//    return *this;
//}
