#ifndef INTERPRETATIONBUILDERMODULE_H
#define INTERPRETATIONBUILDERMODULE_H

class CurveModel;

#include "admin/src/dal/interpretation.h"
#include "admin/src/dal/measure.h"
#include "admin/src/stdadminrepository.h"
#include "admin/src/adminfactory.h"
#include "core/core.h"
#include "core/corefactory.h"
#include "core/utils/autoproperty.h"
#include "core/dal/entity.h"
#include "reportbuildermodule.h"

class InterpretationBuilderModule : public Tucuxi::GuiCore::Entity, public ReportBuilderModule
{
    Q_OBJECT
    AUTO_PROPERTY_DECL(Interpretation*, interpretation, Interpretation)
    AUTO_PROPERTY_DECL(Tucuxi::GuiCore::PredictionSpec*, prediction, Prediction)
    AUTO_PROPERTY_DECL(Measure*, sample, Sample)

public:
    InterpretationBuilderModule(Interpretation* interpretation, Tucuxi::GuiCore::PredictionSpec* prediction, Measure * sample, QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);

    InterpretationBuilderModule &setTreatment(Tucuxi::GuiCore::PredictionSpec* prediction);
};

#endif // INTERPRETATIONBUILDERMODULE_H
