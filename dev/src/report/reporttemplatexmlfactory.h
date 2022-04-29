//@@license@@

#ifndef REPORTTEMPLATEXMLFACTORY_H
#define REPORTTEMPLATEXMLFACTORY_H

class QXmlStreamReader;

#include "reporttemplatefactory.h"

#include <QApplication>

class ReportTemplateXmlFactory : public ReportTemplateFactory
{
    Q_DECLARE_TR_FUNCTIONS(ReportTemplateXmlFactory)

public:
    ReportTemplateXmlFactory(QObject *parent = 0);
    virtual ~ReportTemplateXmlFactory();

    virtual bool readTemplates();

private:
    static const char *const _TEMPLATES_XML_FILE;
    static const char *const _TEMPLATES_XSD_FILE;

    static const char *const _TEMPLATE_TAG;
    static const char *const _FILE_TAG;
    static const char *const _TRANSLATION_TAG;
    static const char *const _NAMES_TAG;
    static const char *const _DESCS_TAG;

    void buildTemplate(QXmlStreamReader &parser);
    void setTranslationType(const QString &type, ReportTemplate &reportTemplate);
};

#endif // REPORTTEMPLATEXMLFACTORY_H
