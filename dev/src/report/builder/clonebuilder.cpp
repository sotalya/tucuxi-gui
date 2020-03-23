#include "clonebuilder.h"
#include "identifiers.h"
#include "report.h"
#include "reportdata.h"
#include "reportfactory.h"
#include "entity.h"
#include "basebuildermodule.h"

CloneBuilder::CloneBuilder(const ReportData *data, QObject *parent) :
    ReportBuilder(parent),
    _data(data)
{
    QString reference = _data->reference();

    if (!reference.isEmpty() && QRegExp("^.*-[0-9]{2}$").exactMatch(reference)) {
        reference.replace(reference.count() - 2, 2, QString("%1").arg(reference.right(2).toInt() + 1, 2, 10, QChar('0')));

        BaseBuilderModule *module = new BaseBuilderModule();
        module->setReference(reference);

        setModule(module->metaObject()->className(), module);
    }
}

Report *CloneBuilder::build()
{
    QScopedPointer<ReportData> data(new ReportData());

    //Set the default data
    if (_data == 0)
        return 0;

    data->setTemplateName(_data->templateName());
    data->setValues(_data->values());
    data->setImages(_data->images());
    data->setHiddenSections(_data->hiddenSections());

    //Reset the date and reference
    data->unsetValue(ID::Publication);
    data->unsetValue(ID::Creation);
    data->unsetValue(ID::Reference);

    //Set the modules data
    for (int i = 0; i < _modules.size(); ++i)
        _modules[_sorted.at(i)]->setData(data.data());

    //Update the creation date
    if (!data->creationDate().isValid())
        data->setCreationDate(QDateTime::currentDateTime());

    //Update the reference
    if (data->reference().isEmpty())
        data->setReference(data->creationDate().toString(_REFERENCE_FORMAT));

    ReportFactory factory;
    return factory.create(data.take());
}
