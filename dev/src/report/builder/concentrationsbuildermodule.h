#ifndef CONCENTRATIONSBUILDERMODULE_H
#define CONCENTRATIONSBUILDERMODULE_H

#include "entity.h"
#include "amount.h"
#include "QDateTime"
#include "reportbuildermodule.h"

class ConcentrationsBuilderModule : public Tucuxi::Gui::Core::Entity, public ReportBuilderModule
{
    Q_OBJECT

public:
    ConcentrationsBuilderModule(const QList<QPair<Tucuxi::Gui::Core::Amount, QDateTime>> &concentrations = QList<QPair<Tucuxi::Gui::Core::Amount, QDateTime>>(), QObject *parent = 0);

    virtual bool setData(ReportData *data);
    virtual void unsetData(ReportData *data);

    QList<QPair<Tucuxi::Gui::Core::Amount, QDateTime>> concentrations() const;
    ConcentrationsBuilderModule &setConcentrations(const QList<QPair<Tucuxi::Gui::Core::Amount, QDateTime>> &concentrations);

private:
    QList<QPair<Tucuxi::Gui::Core::Amount, QDateTime>> _concentrations;
};

#endif // CONCENTRATIONSBUILDERMODULE_H
