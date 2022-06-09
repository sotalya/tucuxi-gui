#ifndef RECIPIENTBUILDERMODULE_H
#define RECIPIENTBUILDERMODULE_H

#include "entity.h"
#include "reportdata.h"
#include "practician.h"
#include "institute.h"
#include "reportbuildermodule.h"

class RecipientBuilderModule : public ezechiel::GuiCore::Entity, public ReportBuilderModule
{
    Q_OBJECT
    AUTO_PROPERTY_DECL(Practician*, practician, Practician)
    AUTO_PROPERTY_DECL(Institute*, institute, Institute)

public:
    RecipientBuilderModule(QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);

};

#endif // RECIPIENTBUILDERMODULE_H
