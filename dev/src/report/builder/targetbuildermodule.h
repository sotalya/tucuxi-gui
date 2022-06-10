#ifndef TARGETBUILDERMODULE_H
#define TARGETBUILDERMODULE_H

#include "core/dal/entity.h"
#include "core/dal/drug/target.h"
#include "core/corefactory.h"
#include "core/corerepository.h"
#include "reportbuildermodule.h"

class TargetBuilderModule : public Tucuxi::GuiCore::Entity, public ReportBuilderModule
{
    Q_OBJECT

public:
    TargetBuilderModule(const QList<Tucuxi::GuiCore::Target*> *targets = 0, QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);

    QList<Tucuxi::GuiCore::Target*> targets() const;
    TargetBuilderModule &setTargets(const QList<Tucuxi::GuiCore::Target*> targets);

private:
    QList<Tucuxi::GuiCore::Target*> _targets;
};

#endif // TARGETBUILDERMODULE_H

