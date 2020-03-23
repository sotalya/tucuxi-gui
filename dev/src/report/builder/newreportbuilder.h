#ifndef NEWREPORTBUILDER_H
#define NEWREPORTBUILDER_H

#include "reportbuilder.h"

class NewReportBuilder : public ReportBuilder
{
    Q_OBJECT

public:
    NewReportBuilder(QObject *parent = 0);
    virtual Report *build();
};

#endif // NEWREPORTBUILDER_H
