//@@license@@

#ifndef REPORTREPOSITORY_H
#define REPORTREPOSITORY_H

#include "abstractrepository.h"
#include "core.h"
#include "errorenums.h"
#include "reportdata.h"

#define REPORTREPO (dynamic_cast<ReportRepository*>(ABSTRACTREPO))

class ReportRepository : public Tucuxi::GuiCore::AbstractRepository
{
public:
    virtual Tucuxi::GuiCore::Response getReport(const Tucuxi::GuiCore::ident &id, ReportData &data) = 0;
    virtual Tucuxi::GuiCore::Response getReport(const QString &field, const QVariant &value, ReportData &data) = 0;
    virtual Tucuxi::GuiCore::Response setReport(ReportData &data) = 0;
    virtual Tucuxi::GuiCore::Response deleteReport(ReportData &data) = 0;
    virtual Tucuxi::GuiCore::Response getReportsList(QList<ReportData *> &dataList, const QString &where = QString()) = 0;

};

#endif // REPORTREPOSITORY_H
