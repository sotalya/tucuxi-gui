#include "reportbuilder.h"
#include "reportbuildermodule.h"

const char *const ReportBuilder::_DEFAULT_TEMPLATE = "ezechiel-default.html";
const char *const ReportBuilder::_REFERENCE_FORMAT = "yyyyMMddhhmmss-01";

ReportBuilder::ReportBuilder(QObject* parent):
QObject(parent)
{

}

ReportBuilder::~ReportBuilder()
{

}

void ReportBuilder::reset()
{
    qDeleteAll(_modules.values());
    _modules.clear();
    _sorted.clear();
}

ReportBuilder &ReportBuilder::setModule(QString className, ReportBuilderModule *module)
{
//    QString className = module->metaObject()->className();

    unsetModuleByClassName(className);

    _sorted.append(className);
    _modules.insert(className, module);

//    module->setParent(this);

    return *this;
}

void ReportBuilder::unsetModuleByClassName(const QString &className)
{
    if (_sorted.contains(className))
        _sorted.removeAll(className);
    if (_modules.contains(className))
        delete _modules.take(className);
}

ReportBuilderModule *ReportBuilder::moduleByClassName(const QString &className)
{
    if (_modules.contains(className))
        return _modules[className];
    return 0;
}

ReportBuilderModule *ReportBuilder::takeModuleByClassName(const QString &className)
{
    _sorted.removeAll(className);

    ReportBuilderModule *module = (ReportBuilderModule*)_modules.take(className);

    return module;
}
