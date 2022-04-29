//@@license@@

#ifndef DEFAULTREPORT_H
#define DEFAULTREPORT_H

#include "report.h"

class DefaultReport : public Report
{
    Q_OBJECT

public:
    DefaultReport(ReportData *data, QObject *parent = 0);

protected:
    virtual void finishedLoading();
};

#endif // DEFAULTREPORT_H
