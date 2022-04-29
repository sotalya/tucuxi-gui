//@@license@@

#include "reporttemplate.h"

ReportTemplate::ReportTemplate() :
    _template(),
    _name(),
    _description(),
    _languages()
{

}

ReportTemplate::ReportTemplate(const QString &templateName, const QString &name, const QString &description, const QStringList &languages) :
    _template(templateName),
    _name(name),
    _description(description),
    _languages(languages)
{

}

QString ReportTemplate::templateName() const
{
    return _template;
}

void ReportTemplate::setTemplateName(const QString &templateName)
{
    _template = templateName;
}

QString ReportTemplate::name() const
{
    return _name;
}

void ReportTemplate::setName(const QString &name)
{
    _name = name;
}

QString ReportTemplate::description() const
{
    return _description;
}

void ReportTemplate::setDescription(const QString &description)
{
    _description = description;
}

QStringList ReportTemplate::languages() const
{
    return _languages;
}

void ReportTemplate::setLanguages(const QStringList &languages)
{
    _languages = languages;
}
