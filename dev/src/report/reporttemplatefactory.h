#ifndef REPORTTEMPLATEFACTORY_H
#define REPORTTEMPLATEFACTORY_H

class ReportTemplate;

#include <QObject>

class ReportTemplateFactory : public QObject
{

public:
    ReportTemplateFactory(QObject *parent = 0);
    virtual ~ReportTemplateFactory() = 0;

    QList<ReportTemplate> templates() const;
    virtual bool readTemplates() = 0;

    QString error() const;
    void resetError();

protected:
    QList<ReportTemplate> _templates;
    QString _error;
};

#endif // REPORTTEMPLATEFACTORY_H
