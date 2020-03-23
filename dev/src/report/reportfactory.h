#ifndef REPORTFACTORY_H
#define REPORTFACTORY_H

class Report;
class ReportData;

#include <QHash>
#include <functional>

class ReportFactory
{

public:
    ReportFactory();
    Report *create(ReportData *data);

private:
    void registerTemplate(const QString &templateName, std::function<Report *(ReportData *)> function);
    QHash<QString, std::function<Report *(ReportData *)>> _registery;
};

#endif // REPORTFACTORY_H
