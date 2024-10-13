/*
 * Tucuxi - Tucuxi-gui software.
 * This software is able to perform prediction of drug concentration in blood
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV.
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


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
