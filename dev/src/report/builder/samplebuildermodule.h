#ifndef SAMPLEBUILDERMODULE_H
#define SAMPLEBUILDERMODULE_H

#include "entity.h"
#include "../dal/measure.h"
#include "stdadminrepository.h"
#include "adminfactory.h"
#include "reportbuildermodule.h"

class SampleBuilderModule : public ezechiel::core::Entity, public ReportBuilderModule
{
    Q_OBJECT

public:
    SampleBuilderModule(Measure * sample, QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);

    Measure * sample() const;
    SampleBuilderModule &setSample(Measure * sample);

private:
    Measure * _sample;
};

#endif // SAMPLEBUILDERMODULE_H
