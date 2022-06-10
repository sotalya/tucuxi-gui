#ifndef CONCENTRATIONSBUILDERMODULE_H
#define CONCENTRATIONSBUILDERMODULE_H

#include "entity.h"
#include "amount.h"
#include "QDateTime"
#include "reportbuildermodule.h"

class ConcentrationsBuilderModule : public Tucuxi::GuiCore::Entity, public ReportBuilderModule
{
    Q_OBJECT

public:
    ConcentrationsBuilderModule(const QList<QPair<Tucuxi::GuiCore::Amount, QDateTime>> &concentrations = QList<QPair<Tucuxi::GuiCore::Amount, QDateTime>>(), QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);

    QList<QPair<Tucuxi::GuiCore::Amount, QDateTime>> concentrations() const;
    ConcentrationsBuilderModule &setConcentrations(const QList<QPair<Tucuxi::GuiCore::Amount, QDateTime>> &concentrations);

private:
    QList<QPair<Tucuxi::GuiCore::Amount, QDateTime>> _concentrations;
};

#endif // CONCENTRATIONSBUILDERMODULE_H
