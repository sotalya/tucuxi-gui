#ifndef REPORTBUILDER_H
#define REPORTBUILDER_H

class Report;
class ReportBuilderModule;

#include <QObject>
#include <QStringList>
#include <QMap>

class ReportBuilder : public QObject
{
    Q_OBJECT

public:
    ReportBuilder(QObject *parent = 0);
    virtual ~ReportBuilder();

    virtual Report *build() = 0;
    virtual void reset();

    ReportBuilder &setModule(QString, ReportBuilderModule*);
    template <typename T>
    ReportBuilder &unsetModule();
    template <typename T>
    T *module();
    template <typename T>
    T *takeModule();

protected:
    static const char *const _DEFAULT_TEMPLATE;
    static const char *const _REFERENCE_FORMAT;

    void unsetModuleByClassName(const QString &className);
    ReportBuilderModule *moduleByClassName(const QString &className);
    ReportBuilderModule *takeModuleByClassName(const QString &className);

    QMap<QString, ReportBuilderModule*> _modules;
    QStringList _sorted;
};

template <typename T>
ReportBuilder &ReportBuilder::unsetModule() {
    unsetModuleByClassName(T::staticMetaObject.className());
    return *this;
}

template <typename T>
T *ReportBuilder::module() {
    return qobject_cast<T *>(moduleByClassName(T::staticMetaObject.className()));
}

template <typename T>
T *ReportBuilder::takeModule() {
    return qobject_cast<T *>(takeModuleByClassName(T::staticMetaObject.className()));
}

#endif // REPORTBUILDER_H
