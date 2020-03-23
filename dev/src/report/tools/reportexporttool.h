#ifndef REPORTEXPORTTOOL_H
#define REPORTEXPORTTOOL_H

class Report;

#include <QString>

class ReportExportTool
{

public:
    ReportExportTool();
    virtual ~ReportExportTool();

    bool exportReport();

    QString filePath() const;
    void setFilePath(const QString &filePath);

    const Report *report() const;
    void setReport(const Report *report);

    virtual QString extension() const = 0;
    virtual QString fileType() const = 0;

private:
    virtual bool exportImpl() = 0;

    const Report *_report;
    QString _filePath;
};

#endif // REPORTEXPORTTOOL_H
