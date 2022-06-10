#ifndef BASEBUILDERMODULE_H
#define BASEBUILDERMODULE_H

#include "entity.h"
#include "reportbuildermodule.h"

#include <QDateTime>

class BaseBuilderModule :public Tucuxi::GuiCore::Entity, public ReportBuilderModule
{
    Q_OBJECT

public:
    BaseBuilderModule(QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);

    QString templateName() const;
    QString language() const;
    QString title() const;
    QString reference() const;
    QDateTime creationDate() const;
    QDateTime publicationDate() const;

    BaseBuilderModule &setTemplateName(const QString &templateName);
    BaseBuilderModule &setLanguage(const QString &language);
    BaseBuilderModule &setTitle(const QString &title);
    BaseBuilderModule &setReference(const QString &reference);
    BaseBuilderModule &setCreationDate(const QDateTime &date);
    BaseBuilderModule &setPublicationDate(const QDateTime &date);

private:
    QString _templateName;
    QString _language;
    QString _title;
    QString _reference;

    QDateTime _creationDate;
    QDateTime _publicationDate;
};

#endif // BASEBUILDERMODULE_H
