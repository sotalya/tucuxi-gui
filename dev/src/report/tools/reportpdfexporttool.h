#ifndef REPORTPDFEXPORTTOOL_H
#define REPORTPDFEXPORTTOOL_H

#include "reportexporttool.h"

class ReportPdfExportTool : public ReportExportTool
{

public:
    virtual QString extension() const;
    virtual QString fileType() const;

private:
    virtual bool exportImpl();

    static const QString _ID;
    static const QString _TYPE;
};

#endif // REPORTPDFEXPORTTOOL_H
