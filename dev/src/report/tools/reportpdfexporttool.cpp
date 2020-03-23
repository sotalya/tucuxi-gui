#include "reportpdfexporttool.h"
#include "report.h"

#include <QPrinter>
#include <QWebPage>
#include <QWebFrame>

const QString ReportPdfExportTool::_ID   = ".pdf";
const QString ReportPdfExportTool::_TYPE = "PDF file (*" + ReportPdfExportTool::_ID + ")";

QString ReportPdfExportTool::extension() const
{
    return _ID;
}

QString ReportPdfExportTool::fileType() const
{
    return _TYPE;
}

bool ReportPdfExportTool::exportImpl()
{
    // ToDo ///////////////////////////////////////////////////////////////////////////////////////
    // Currently, the margins are hardcoded to the standard report css defined margins. When imple-
    // menting methods to get and set margins in the Report class, get them from the report instead.
    // ////////////////////////////////////////////////////////////////////////////////////////////

    QMargins margins = report()->margins();

    QPrinter printer;
    printer.setOutputFileName(filePath());
    printer.setOutputFormat(QPrinter::NativeFormat);
    printer.setColorMode(QPrinter::Color);
    printer.setResolution(QPrinter::HighResolution);
    printer.setPageMargins(margins.left(), margins.top(), margins.right(), margins.bottom(), QPrinter::Millimeter);

    report()->page()->mainFrame()->print(&printer);

    return true;
}
