#ifndef ANALYSTBUILDERMODULE_H
#define ANALYSTBUILDERMODULE_H

#include "entity.h"
#include "reportbuildermodule.h"

class AnalystBuilderModule :public Tucuxi::GuiCore::Entity, public ReportBuilderModule
{
    Q_OBJECT
    AUTO_PROPERTY_DECL(Practician*, practician, Practician)

public:
    AnalystBuilderModule(QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);
};

#endif // ANALYSTBUILDERMODULE_H
