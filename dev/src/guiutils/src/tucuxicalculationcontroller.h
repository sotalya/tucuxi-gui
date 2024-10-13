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


#ifndef TUCUXICALCULATIONCONTROLLER_H
#define TUCUXICALCULATIONCONTROLLER_H

#include "calculationcontroller.h"

class TucuxiCalculationController : public CalculationController
{
    Q_OBJECT
public:
    explicit TucuxiCalculationController(QObject *parent = nullptr);


public slots:
    void computePopPred(Tucuxi::Gui::Core::PredictionSpec*);
    void computePopPerc(Tucuxi::Gui::Core::PredictionSpec*);
    void computeAprPred(Tucuxi::Gui::Core::PredictionSpec*);
    void computeAprPerc(Tucuxi::Gui::Core::PredictionSpec*);
    void computeApoPred(Tucuxi::Gui::Core::PredictionSpec*);
    void computeApoPerc(Tucuxi::Gui::Core::PredictionSpec*);
    void computeRevPred(Tucuxi::Gui::Core::PredictionSpec*);
    void computeAdjPred(Tucuxi::Gui::Core::PredictionSpec*);
    void abortAll();
//    void updateAdj();

private:
    void preparePredResults(QVector<int> time, QVector<double> data, Tucuxi::Gui::Core::PredictionResult* pred);
    Tucuxi::Gui::Core::PredictionResult* computePrediction(const Tucuxi::Core::DrugModel& _model,
        const Tucuxi::Core::DrugTreatment &_treatment,
        Tucuxi::Core::PredictionParameterType _type,
        const Tucuxi::Common::DateTime &_startDate,
        const Tucuxi::Common::DateTime &_endDate,
        int nbPoints);

    Tucuxi::Gui::Core::PredictionResult* computePercentiles(const Tucuxi::Core::DrugModel& _model,
        const Tucuxi::Core::DrugTreatment &_treatment,
        Tucuxi::Core::PredictionParameterType _type,
        const Tucuxi::Common::DateTime &_startDate,
        const Tucuxi::Common::DateTime &_endDate,
        int nbPoints);

private:
    int m_requestID;
    PopPercAborter *popPercAborter;
    AprPercAborter *aprPercAborter;
    ApoPercAborter *apoPercAborter;
};


#endif // TUCUXICALCULATIONCONTROLLER_H
