//@@license@@

#include "measuretabcontroller.h"

#include "core/corefactory.h"
#include "core/dal/coremeasure.h"
#include "core/dal/drugtreatment.h"
#include "admin/src/adminfactory.h"
#include "admin/src/dal/measure.h"
#include "core/utils/units.h"
#include "admin/src/dal/interpretation.h"
#include "interpretationcontroller.h"

using namespace Tucuxi::Gui::Admin;
using namespace Tucuxi::Gui::GuiUtils;

STD_PROPERTY_IMPL(MeasureTabController, Tucuxi::Gui::Core::CoreMeasureList*, measures, Measures)

MeasureTabController::MeasureTabController(QObject *parent) : AbstractViewController(parent)
{
    _measures = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::CoreMeasureList>(ABSTRACTREPO, this);
}

void MeasureTabController::setSampleId(int index, const QString &id)
{
    if (!isIndexValid(index)) return;

    Measure *measure = static_cast<Measure*>(_measures->at(index));
    if (measure->sampleID() == id) return;

    measure->sampleID(id);
}

void MeasureTabController::setMoment(int index, QDateTime time)
{
    if (!isIndexValid(index)) return;

    Measure *measure = static_cast<Measure*>(_measures->at(index));
    if (measure->getMoment() == time) return;

    _measures->at(index)->setMoment(time);

    // Force arrival date if
    if (time > measure->arrivalDate() ||        // before the measure or
        !masterController->isFlowRequest())     // working with a new patient
    {
        measure->arrivalDate(time);
    }
}

void MeasureTabController::setArrivalDate(int index, QDateTime time)
{
    if (!isIndexValid(index)) return;

    Measure *measure = static_cast<Measure*>(_measures->at(index));
    if (measure->arrivalDate() == time) return;

    if (time < measure->getMoment())
    {
        time = measure->getMoment();
    }
    measure->arrivalDate(time);
}

void MeasureTabController::setEnable(int index, bool enable)
{
    if (!isIndexValid(index)) return;

    Measure *measure = static_cast<Measure*>(_measures->at(index));

    measure->enable(enable);
}

void MeasureTabController::setDbValue(int index, double value)
{
    if (!isIndexValid(index)) return;

    Measure *measure = static_cast<Measure*>(_measures->at(index));
    if (measure->getConcentration()->getDbvalue() == value) return;

    measure->getConcentration()->setDbvalue(value);
}

void MeasureTabController::addMeasure()
{
    Measure* measure = AdminFactory::createEntity<Measure>(ABSTRACTREPO, _measures);
    measure->getConcentration()->setUnitstring(masterController->getDefaultUnit());
    measure->getConcentration()->setUnit(Tucuxi::Gui::Core::Unit(masterController->getDefaultUnit()));
    _measures->append(measure);
}

void MeasureTabController::removeMeasure(int index)
{
    if (index >= _measures->size()) return;
    _measures->remove(index);
    masterController->measureUpdated();
}

void MeasureTabController::forceRefresh()
{
    masterController->measureUpdated();
}

bool MeasureTabController::isIndexValid(int index)
{
    return (index >= 0) && (index < _measures->size());
}
