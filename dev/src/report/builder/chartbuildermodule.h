#ifndef CHARTBUILDERMODULE_H
#define CHARTBUILDERMODULE_H

class Chart;

#include "entity.h"
#include "reportbuildermodule.h"

class ChartBuilderModule : public Tucuxi::Gui::Core::Entity, public ReportBuilderModule
{
    Q_OBJECT

public:
    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);

//    Chart *chart() const;
//    ChartBuilderModule &setChart(Chart *chart);

//private:
//    Chart *_chart;
};

#endif // CHARTBUILDERMODULE_H
