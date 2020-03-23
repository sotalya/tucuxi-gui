#include "basebuildermodule.h"
#include "reportdata.h"

BaseBuilderModule::BaseBuilderModule(QObject *parent) :
    _templateName(),
    _language(),
    _title(),
    _reference(),
    _creationDate(),
    _publicationDate()
{

}

bool BaseBuilderModule::setData(ReportData *data)
{
    if (!_templateName.isEmpty())
        data->setTemplateName(_templateName);
    if (!_language.isEmpty())
        data->setLanguage(_language);
    if (!_title.isEmpty())
        data->setTitle(_title);
    if (!_reference.isEmpty())
        data->setReference(_reference);
    if (_creationDate.isValid())
        data->setCreationDate(_creationDate);
    if (_publicationDate.isValid())
        data->setPublicationDate(_publicationDate);

    return true;
}

void BaseBuilderModule::unsetData(ReportData *data)
{
    data->setTemplateName(QString());
    data->setLanguage(QString());
    data->setTitle(QString());
    data->setReference(QString());
    data->setCreationDate(QDateTime());
    data->setPublicationDate(QDateTime());
}

QString BaseBuilderModule::templateName() const
{
    return _templateName;
}

QString BaseBuilderModule::language() const
{
    return _language;
}

QString BaseBuilderModule::title() const
{
    return _title;
}

QString BaseBuilderModule::reference() const
{
    return _reference;
}

QDateTime BaseBuilderModule::creationDate() const
{
    return _creationDate;
}

QDateTime BaseBuilderModule::publicationDate() const
{
    return _publicationDate;
}

BaseBuilderModule &BaseBuilderModule::setTemplateName(const QString &templateName)
{
    _templateName = templateName;
    return *this;
}

BaseBuilderModule &BaseBuilderModule::setLanguage(const QString &language)
{
    _language = language;
    return *this;
}

BaseBuilderModule &BaseBuilderModule::setTitle(const QString &title)
{
    _title = title;
    return *this;
}

BaseBuilderModule &BaseBuilderModule::setReference(const QString &reference)
{
    _reference = reference;
    return *this;
}

BaseBuilderModule &BaseBuilderModule::setCreationDate(const QDateTime &date)
{
    _creationDate = date;
    return *this;
}

BaseBuilderModule &BaseBuilderModule::setPublicationDate(const QDateTime &date)
{
    _publicationDate = date;
    return *this;
}
