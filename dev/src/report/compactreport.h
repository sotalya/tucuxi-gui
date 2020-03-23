#ifndef COMPACTREPORT_H
#define COMPACTREPORT_H

#include "report.h"

class CompactReport : public Report
{
    Q_OBJECT

public:
    CompactReport(ReportData *data, QObject *parent = 0);

protected:
    virtual void finishedLoading();
};

#endif // COMPACTREPORT_H
