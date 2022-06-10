#include "targetbuildermodule.h"
#include "reportdata.h"
#include "identifiers.h"

using namespace Tucuxi::Gui::Core::Core;

TargetBuilderModule::TargetBuilderModule(const QList<Target*> *targets, QObject *parent) :
    _targets(*targets)
{

}

bool TargetBuilderModule::setData(ReportData *data)
{
    if (_targets.size() == 0)
        return true;

    data->setValue(ID::Targets::Count, QString::number(_targets.size()));

    for (int i = 0; i < _targets.size(); ++i) {
        Target* target = _targets.at(i);

        data->setValue(QString(ID::Targets::Target::Type).arg(i), target->getType()->getLabel());

        // ToDo ///////////////////////////////////////////////////////////////////////////////////
        // When refactoring, find a generic way to deal with targets type and their values count.
        // ////////////////////////////////////////////////////////////////////////////////////////

        data->setValue(QString(ID::Targets::Target::Count).arg(i), target->getType()->getTargetType() == Tucuxi::Gui::Core::TargetMethod::PeakTarget ? "2" :  "1");

        data->setValue(QString(ID::Targets::Target::Value::Unit).arg(i).arg(0), target->getCbest()->unit().toString());
        data->setValue(QString(ID::Targets::Target::Value::Min).arg(i).arg(0), QString::number(target->getCmin()->value()));
        data->setValue(QString(ID::Targets::Target::Value::Max).arg(i).arg(0), QString::number(target->getCmax()->value()));
        data->setValue(QString(ID::Targets::Target::Value::Best).arg(i).arg(0), QString::number(target->getCbest()->value()));

        if (target->getType()->getTargetType() == Tucuxi::Gui::Core::TargetMethod::PeakTarget) {
            data->setValue(QString(ID::Targets::Target::Value::Unit).arg(i).arg(1), "h");
            data->setValue(QString(ID::Targets::Target::Value::Min).arg(i).arg(1), QString::number(target->getTmin()->value()));
            data->setValue(QString(ID::Targets::Target::Value::Max).arg(i).arg(1), QString::number(target->getTmax()->value()));
            data->setValue(QString(ID::Targets::Target::Value::Best).arg(i).arg(1), QString::number(target->getTbest()->value()));
        }
    }

    return true;
}

void TargetBuilderModule::unsetData(ReportData *data)
{
    int targetsCount = data->value(ID::Targets::Count).toInt();

    for (int i = 0; i < targetsCount; ++i) {
        int targetValuesCount = data->value(ID::Targets::Target::Count).toInt();

        for (int j = 0; j < targetValuesCount; ++j) {
            data->unsetValue(QString(ID::Targets::Target::Value::Unit).arg(i).arg(j));
            data->unsetValue(QString(ID::Targets::Target::Value::Min).arg(i).arg(j));
            data->unsetValue(QString(ID::Targets::Target::Value::Max).arg(i).arg(j));
            data->unsetValue(QString(ID::Targets::Target::Value::Best).arg(i).arg(j));
        }
        data->unsetValue(QString(ID::Targets::Target::Type).arg(i));
        data->unsetValue(QString(ID::Targets::Target::Count).arg(i));
    }
    data->unsetValue(ID::Targets::Count);
}

QList<Target*> TargetBuilderModule::targets() const
{
    return _targets;
}

TargetBuilderModule &TargetBuilderModule::setTargets(const QList<Target*> targets)
{
    _targets = targets;
    return *this;
}
