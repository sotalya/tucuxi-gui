#ifndef REPORTREPOSITORY_H
#define REPORTREPOSITORY_H

#include "abstractrepository.h"
#include "core.h"
#include "errorenums.h"
#include "reportdata.h"

#define REPORTREPO (dynamic_cast<ReportRepository*>(ABSTRACTREPO))

class ReportRepository : public ezechiel::core::AbstractRepository
{
public:
    virtual ezechiel::core::Response getReport(const ezechiel::core::ident &id, ReportData &data) = 0;
    virtual ezechiel::core::Response getReport(const QString &field, const QVariant &value, ReportData &data) = 0;
    virtual ezechiel::core::Response setReport(ReportData &data) = 0;
    virtual ezechiel::core::Response deleteReport(ReportData &data) = 0;
    virtual ezechiel::core::Response getReportsList(QList<ReportData *> &dataList, const QString &where = QString()) = 0;

};

#endif // REPORTREPOSITORY_H
