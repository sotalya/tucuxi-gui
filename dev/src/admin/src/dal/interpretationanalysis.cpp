#include "interpretationanalysis.h"

InterpretationAnalysis::InterpretationAnalysis(Tucuxi::GuiCore::AbstractRepository *repository, QObject *parent) :
    Entity(repository,parent),
    _chartData(Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::ChartData>(repository, this))
{
    setId(-1);
}


AUTO_PROPERTY_IMPL(InterpretationAnalysis, Tucuxi::GuiCore::ChartData*, chartData, ChartData)
AUTO_PROPERTY_IMPL(InterpretationAnalysis, QString, expectedness, Expectedness)
AUTO_PROPERTY_IMPL(InterpretationAnalysis, QString, suitability, Suitability)
AUTO_PROPERTY_IMPL(InterpretationAnalysis, QString, prediction, Prediction)
AUTO_PROPERTY_IMPL(InterpretationAnalysis, QString, remonitoring, Remonitoring)
AUTO_PROPERTY_IMPL(InterpretationAnalysis, QString, warning, Warning)
AUTO_PROPERTY_IMPL(InterpretationAnalysis, QDateTime, nextControl, NextControl)
AUTO_PROPERTY_IMPL(InterpretationAnalysis, QString, dosageDescription, DosageDescription)
