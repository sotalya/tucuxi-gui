//@@license@@

#ifndef REPORTREPOSITORY_H
#define REPORTREPOSITORY_H

#include "abstractrepository.h"
#include "core.h"
#include "errorenums.h"
#include "reportdata.h"

#define REPORTREPO (dynamic_cast<ReportRepository*>(ABSTRACTREPO))

class ReportRepository : public Tucuxi::Gui::Core::AbstractRepository
{
public:
    virtual Tucuxi::Gui::Core::Response getReport(const Tucuxi::Gui::Core::ident &id, ReportData &data) = 0;
    virtual Tucuxi::Gui::Core::Response getReport(const QString &field, const QVariant &value, ReportData &data) = 0;
    virtual Tucuxi::Gui::Core::Response setReport(ReportData &data) = 0;
    virtual Tucuxi::Gui::Core::Response deleteReport(ReportData &data) = 0;
    virtual Tucuxi::Gui::Core::Response getReportsList(QList<ReportData *> &dataList, const QString &where = QString()) = 0;

};

#endif // REPORTREPOSITORY_H
