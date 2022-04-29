//@@license@@

#include "reportfactory.h"
#include "reportdata.h"
#include "report.h"
#include "customreport.h"
#include "defaultreport.h"
#include "compactreport.h"

#define REGISTER_REPORT(TEMPLATE, CLASS) registerTemplate(TEMPLATE, [](ReportData *data) -> Report * {return new CLASS(data);});

ReportFactory::ReportFactory() :
    _registery()
{
    REGISTER_REPORT("ezechiel-default.html", DefaultReport);
    REGISTER_REPORT("ezechiel-compact.html", CompactReport);
}

Report *ReportFactory::create(ReportData *data)
{
    if (data == 0)
        return new CustomReport(new ReportData());

    if (!_registery.contains(data->templateName()))
        return new CustomReport(data);

    return _registery.value(data->templateName())(data);
}

void ReportFactory::registerTemplate(const QString &templateName, std::function<Report *(ReportData *)> function)
{
    _registery.insert(templateName, function);
}
