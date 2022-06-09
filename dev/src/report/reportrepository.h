//@@license@@

#ifndef REPORTREPOSITORY_H
#define REPORTREPOSITORY_H

#include "abstractrepository.h"
#include "core.h"
#include "errorenums.h"
#include "reportdata.h"

#define REPORTREPO (dynamic_cast<ReportRepository*>(ABSTRACTREPO))

class ReportRepository : public ezechiel::GuiCore::AbstractRepository
{
public:
    virtual ezechiel::GuiCore::Response getReport(const ezechiel::GuiCore::ident &id, ReportData &data) = 0;
    virtual ezechiel::GuiCore::Response getReport(const QString &field, const QVariant &value, ReportData &data) = 0;
    virtual ezechiel::GuiCore::Response setReport(ReportData &data) = 0;
    virtual ezechiel::GuiCore::Response deleteReport(ReportData &data) = 0;
    virtual ezechiel::GuiCore::Response getReportsList(QList<ReportData *> &dataList, const QString &where = QString()) = 0;

};

#endif // REPORTREPOSITORY_H
