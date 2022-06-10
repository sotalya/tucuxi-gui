//@@license@@

#include "reporttemplatexmlfactory.h"
#include "reporttemplate.h"
#include "core.h"
#include "xmlvalidator.h"
#include "xmltranslator.h"

#include <QXmlStreamReader>
#include <QDir>

using namespace Tucuxi::GuiCore;
const char *const ReportTemplateXmlFactory::_TEMPLATES_XML_FILE = ":/templates.xml";
const char *const ReportTemplateXmlFactory::_TEMPLATES_XSD_FILE = ":/schemas/templates.xsd";

const char *const ReportTemplateXmlFactory::_TEMPLATE_TAG    = "template";
const char *const ReportTemplateXmlFactory::_FILE_TAG        = "file";
const char *const ReportTemplateXmlFactory::_TRANSLATION_TAG = "translation";
const char *const ReportTemplateXmlFactory::_NAMES_TAG       = "names";
const char *const ReportTemplateXmlFactory::_DESCS_TAG       = "descs";

ReportTemplateXmlFactory::ReportTemplateXmlFactory(QObject *parent) : ReportTemplateFactory(parent)
{

}

ReportTemplateXmlFactory::~ReportTemplateXmlFactory()
{

}

bool ReportTemplateXmlFactory::readTemplates()
{
    QString xmlFilePath = _TEMPLATES_XML_FILE;
    QString xsdFilePath = _TEMPLATES_XSD_FILE;

    XmlValidator *validator = CORE->xmlValidator();
    if (!validator->validate(xmlFilePath, xsdFilePath)) {
        _error = validator->errorMessage();
        return false;
    }

    QFile xmlFile(xmlFilePath);
    if (!xmlFile.open(QIODevice::ReadOnly)) {
        _error = tr("The templates file '%1' could not be opened.").arg(xmlFile.fileName());
        return false;
    }

    QXmlStreamReader parser;
    parser.setDevice(&xmlFile);

    _templates.clear();

    while (!parser.atEnd() && parser.readNext()) {
        if (parser.isStartElement() && parser.name() == _TEMPLATE_TAG)
            buildTemplate(parser);
    }

    if (parser.hasError()) {
        _error = parser.errorString();
        return false;
    }

    return true;
}

void ReportTemplateXmlFactory::buildTemplate(QXmlStreamReader &parser)
{
    ReportTemplate reportTemplate;
    XmlTranslator translator;

    while (parser.readNextStartElement()) {

        if (parser.name() == _FILE_TAG) {
            reportTemplate.setTemplateName(parser.readElementText().trimmed());
        } else if (parser.name() == _TRANSLATION_TAG) {
            setTranslationType(parser.readElementText().trimmed(), reportTemplate);
        } else if (parser.name() == _NAMES_TAG) {
            reportTemplate.setName(translator.translate(parser, "lang").trimmed());
        } else if (parser.name() == _DESCS_TAG) {
            reportTemplate.setDescription(translator.translate(parser, "lang").trimmed());
        } else {
            parser.skipCurrentElement();
        }
    }

    _templates.append(reportTemplate);
}

void ReportTemplateXmlFactory::setTranslationType(const QString &type, ReportTemplate &reportTemplate)
{
    // ToDo ///////////////////////////////////////////////////////////////////////////////////////
    // Add support for supplied translation when necessary.
    // ////////////////////////////////////////////////////////////////////////////////////////////

    if (type == "supplied") {
        return;
    }

    if (type == "native") {
        QStringList languages;
        languages.append("en");

        QDir translationDir = CORE->path(Core::Translations);
        QStringList translationFiles = translationDir.entryList(QStringList("report_*.qm"));

        for (int i = 0; i < translationFiles.size(); ++i) {
            QString locale = translationFiles.at(i);

            locale.truncate(locale.lastIndexOf('.'));
            locale.remove(0, locale.indexOf('_') + 1);

            languages.append(locale);
        }

        reportTemplate.setLanguages(languages);
    }
}
