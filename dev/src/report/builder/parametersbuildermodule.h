#ifndef PARAMETERSBUILDERMODULE_H
#define PARAMETERSBUILDERMODULE_H

#include "core/dal/entity.h"
#include "core/dal/drug/parameters.h"
#include "reportbuildermodule.h"

class ParametersBuilderModule : public ezechiel::GuiCore::Entity, public ReportBuilderModule
{
    Q_OBJECT

public:
    ParametersBuilderModule(ezechiel::GuiCore::ParameterSet* parameters = new ezechiel::GuiCore::ParameterSet(), QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);

    ezechiel::GuiCore::ParameterSet* parameters() const;
    ParametersBuilderModule &setParameters(ezechiel::GuiCore::ParameterSet* parameters);

private:
    ezechiel::GuiCore::ParameterSet* _parameters;
};

#endif // PARAMETERSBUILDERMODULE_H
