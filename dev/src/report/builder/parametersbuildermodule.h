#ifndef PARAMETERSBUILDERMODULE_H
#define PARAMETERSBUILDERMODULE_H

#include "core/dal/entity.h"
#include "core/dal/drug/parameters.h"
#include "reportbuildermodule.h"

class ParametersBuilderModule : public Tucuxi::GuiCore::Entity, public ReportBuilderModule
{
    Q_OBJECT

public:
    ParametersBuilderModule(Tucuxi::GuiCore::ParameterSet* parameters = new Tucuxi::GuiCore::ParameterSet(), QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);

    Tucuxi::GuiCore::ParameterSet* parameters() const;
    ParametersBuilderModule &setParameters(Tucuxi::GuiCore::ParameterSet* parameters);

private:
    Tucuxi::GuiCore::ParameterSet* _parameters;
};

#endif // PARAMETERSBUILDERMODULE_H
