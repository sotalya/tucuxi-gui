//@@license@@

#include "reporttemplatefactory.h"
#include "reporttemplate.h"

ReportTemplateFactory::ReportTemplateFactory(QObject *parent) : QObject(parent)
{

}

ReportTemplateFactory::~ReportTemplateFactory()
{

}

QList<ReportTemplate> ReportTemplateFactory::templates() const
{
    return _templates;
}

QString ReportTemplateFactory::error() const
{
    return _error;
}

void ReportTemplateFactory::resetError()
{
    _error.clear();
}
