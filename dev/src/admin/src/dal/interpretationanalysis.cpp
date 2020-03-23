#include "interpretationanalysis.h"

InterpretationAnalysis::InterpretationAnalysis(ezechiel::core::AbstractRepository *repository, QObject *parent) :
    Entity(repository,parent),
    _chartData(ezechiel::core::CoreFactory::createEntity<ezechiel::core::ChartData>(repository, this))
{
    setId(-1);
}


AUTO_PROPERTY_IMPL(InterpretationAnalysis, ezechiel::core::ChartData*, chartData, ChartData)
AUTO_PROPERTY_IMPL(InterpretationAnalysis, QString, expectedness, Expectedness)
AUTO_PROPERTY_IMPL(InterpretationAnalysis, QString, suitability, Suitability)
AUTO_PROPERTY_IMPL(InterpretationAnalysis, QString, prediction, Prediction)
AUTO_PROPERTY_IMPL(InterpretationAnalysis, QString, remonitoring, Remonitoring)
AUTO_PROPERTY_IMPL(InterpretationAnalysis, QString, warning, Warning)
AUTO_PROPERTY_IMPL(InterpretationAnalysis, QDateTime, nextControl, NextControl)
AUTO_PROPERTY_IMPL(InterpretationAnalysis, QString, dosageDescription, DosageDescription)
