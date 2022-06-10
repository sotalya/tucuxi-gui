#ifndef TEAMBUILDERMODULE_H
#define TEAMBUILDERMODULE_H

#include "entity.h"
#include "practician.h"
#include "institute.h"
#include "autoproperty.h"
#include "reportbuildermodule.h"

class TeamBuilderModule : public Tucuxi::GuiCore::Entity, public ReportBuilderModule
{
    Q_OBJECT
    AUTO_PROPERTY_DECL(Practician*, practician, Practician)

public:
    TeamBuilderModule(Practician* practician, QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);

};

#endif // TEAMBUILDERMODULE_H
