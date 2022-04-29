//@@license@@

#ifndef REPORTTEMPLATE_H
#define REPORTTEMPLATE_H

#include <QObject>
#include <QStringList>

class ReportTemplate
{

public:

    ReportTemplate();
    ReportTemplate(const QString &templateName, const QString &name, const QString &description, const QStringList &languages);

    QString templateName() const;
    void setTemplateName(const QString &templateName);

    QString name() const;
    void setName(const QString &name);

    QString description() const;
    void setDescription(const QString &description);

    QStringList languages() const;
    void setLanguages(const QStringList &languages);

private:
    QString _template;
    QString _name;
    QString _description;
    QStringList _languages;
};

Q_DECLARE_METATYPE(ReportTemplate)

#endif // REPORTTEMPLATE_H
