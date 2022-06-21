#include "interpretationanalysis.h"

using namespace Tucuxi::Gui::Admin;

InterpretationAnalysis::InterpretationAnalysis(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent) :
    Entity(repository,parent),
    _chartData(Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::ChartData>(repository, this))
{
    setId(-1);
}


AUTO_PROPERTY_IMPL(InterpretationAnalysis, Tucuxi::Gui::Core::ChartData*, chartData, ChartData)
AUTO_PROPERTY_IMPL(InterpretationAnalysis, QString, expectedness, Expectedness)
AUTO_PROPERTY_IMPL(InterpretationAnalysis, QString, suitability, Suitability)
AUTO_PROPERTY_IMPL(InterpretationAnalysis, QString, prediction, Prediction)
AUTO_PROPERTY_IMPL(InterpretationAnalysis, QString, remonitoring, Remonitoring)
AUTO_PROPERTY_IMPL(InterpretationAnalysis, QString, warning, Warning)
AUTO_PROPERTY_IMPL(InterpretationAnalysis, QDateTime, nextControl, NextControl)
AUTO_PROPERTY_IMPL(InterpretationAnalysis, QString, dosageDescription, DosageDescription)
