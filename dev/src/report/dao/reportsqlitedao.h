#ifndef REPORTSQLITEDAO_H
#define REPORTSQLITEDAO_H

class Report;
class ReportData;

#include "genericdao.h"
#include "genericdatabase.h"
#include "errorenums.h"

#include <QCoreApplication>

class ReportSQLiteDAO : public GenericDAO<Report>
{
    Q_DECLARE_TR_FUNCTIONS(ReportSQLiteDAO)

public:
    virtual Report *findById(int id);
    virtual Report *findByValue(const QString &field, const QVariant &value);
    virtual QList<Report *> findAll();
    virtual bool save(Report *report);
    virtual bool remove(const Report *report);

private:
    void enforceFKConstraint(ReportData *data);
};

#endif // REPORTSQLITEDAO_H
