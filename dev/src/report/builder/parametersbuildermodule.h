#ifndef PARAMETERSBUILDERMODULE_H
#define PARAMETERSBUILDERMODULE_H

#include "core/dal/entity.h"
#include "core/dal/drug/parameters.h"
#include "reportbuildermodule.h"

class ParametersBuilderModule : public Tucuxi::Gui::Core::Entity, public ReportBuilderModule
{
    Q_OBJECT

public:
    ParametersBuilderModule(Tucuxi::Gui::Core::ParameterSet* parameters = new Tucuxi::Gui::Core::ParameterSet(), QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);

    Tucuxi::Gui::Core::ParameterSet* parameters() const;
    ParametersBuilderModule &setParameters(Tucuxi::Gui::Core::ParameterSet* parameters);

private:
    Tucuxi::Gui::Core::ParameterSet* _parameters;
};

#endif // PARAMETERSBUILDERMODULE_H
