#ifndef CONCENTRATIONSBUILDERMODULE_H
#define CONCENTRATIONSBUILDERMODULE_H

#include "entity.h"
#include "amount.h"
#include "QDateTime"
#include "reportbuildermodule.h"

class ConcentrationsBuilderModule : public ezechiel::core::Entity, public ReportBuilderModule
{
    Q_OBJECT

public:
    ConcentrationsBuilderModule(const QList<QPair<ezechiel::core::Amount, QDateTime>> &concentrations = QList<QPair<ezechiel::core::Amount, QDateTime>>(), QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);

    QList<QPair<ezechiel::core::Amount, QDateTime>> concentrations() const;
    ConcentrationsBuilderModule &setConcentrations(const QList<QPair<ezechiel::core::Amount, QDateTime>> &concentrations);

private:
    QList<QPair<ezechiel::core::Amount, QDateTime>> _concentrations;
};

#endif // CONCENTRATIONSBUILDERMODULE_H
