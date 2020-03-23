#include "reportexporttool.h"

ReportExportTool::ReportExportTool() :
    _filePath(),
    _report(0)
{

}

ReportExportTool::~ReportExportTool()
{

}

bool ReportExportTool::exportReport()
{
    if (report() == 0)
        return false;
    if (filePath().isEmpty())
        return false;

    return exportImpl();
}

QString ReportExportTool::filePath() const
{
    return _filePath;
}

void ReportExportTool::setFilePath(const QString &filePath)
{
    _filePath = filePath;
}

const Report *ReportExportTool::report() const
{
    return _report;
}

void ReportExportTool::setReport(const Report *report)
{
    _report = report;
}
