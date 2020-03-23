#include "newreportbuilder.h"
#include "report.h"
#include "reportdata.h"
#include "reportfactory.h"
#include "entity.h"
#include "identifiers.h"
#include "core.h"
#include "reportbuildermodule.h"

#include <QMap>
#include <QStringList>

NewReportBuilder::NewReportBuilder(QObject *parent) :
    ReportBuilder(parent)
{
}

Report *NewReportBuilder::build()
{
    QScopedPointer<ReportData> data(new ReportData());

    //Set the modules data
    for (int i = 0; i < _modules.size(); ++i) {
        ReportBuilderModule * rbm = _modules[_sorted.at(i)];
        rbm->setData(data.data());
    }

    //Set the default title
    QString title = tr("Therapeutic monitoring");
    if (!data->value(ID::Treatment::Drug).isEmpty())
        title.append(" " + tr("of") + " " + data->value(ID::Treatment::Drug));
    if (!data->value(ID::Patient::Firstname).isEmpty() && !data->value(ID::Patient::Lastname).isEmpty())
        title.append(" " + tr("for") + " " + data->value(ID::Patient::Firstname) + " " + data->value(ID::Patient::Lastname));

    //Set the default values
    if (data->templateName().isEmpty())
        data->setTemplateName(_DEFAULT_TEMPLATE);
    if (data->title().isEmpty())
        data->setTitle(title);
    if (data->language().isEmpty())
        data->setLanguage(CORE->language());
    if (!data->creationDate().isValid())
        data->setCreationDate(QDateTime::currentDateTime());
    if (data->reference().isEmpty())
        data->setReference(data->creationDate().toString(_REFERENCE_FORMAT));

    ReportFactory factory;
    return factory.create(data.take());
}
