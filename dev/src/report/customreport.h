#ifndef CUSTOMREPORT_H
#define CUSTOMREPORT_H

#include "report.h"

class CustomReport : public Report
{
    Q_OBJECT

public:
    CustomReport(ReportData *data, QObject *parent = 0);
};

#endif // CUSTOMREPORT_H
