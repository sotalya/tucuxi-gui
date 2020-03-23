#ifndef PARAMETERSBUILDERMODULE_H
#define PARAMETERSBUILDERMODULE_H

#include "core/dal/entity.h"
#include "core/dal/drug/parameters.h"
#include "reportbuildermodule.h"

class ParametersBuilderModule : public ezechiel::core::Entity, public ReportBuilderModule
{
    Q_OBJECT

public:
    ParametersBuilderModule(ezechiel::core::ParameterSet* parameters = new ezechiel::core::ParameterSet(), QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);

    ezechiel::core::ParameterSet* parameters() const;
    ParametersBuilderModule &setParameters(ezechiel::core::ParameterSet* parameters);

private:
    ezechiel::core::ParameterSet* _parameters;
};

#endif // PARAMETERSBUILDERMODULE_H
