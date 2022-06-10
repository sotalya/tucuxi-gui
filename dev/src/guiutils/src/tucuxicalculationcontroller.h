//@@license@@

#ifndef TUCUXICALCULATIONCONTROLLER_H
#define TUCUXICALCULATIONCONTROLLER_H

#include "calculationcontroller.h"

class TucuxiCalculationController : public CalculationController
{
    Q_OBJECT
public:
    explicit TucuxiCalculationController(QObject *parent = nullptr);


public slots:
    void computePopPred(Tucuxi::GuiCore::PredictionSpec*);
    void computePopPerc(Tucuxi::GuiCore::PredictionSpec*);
    void computeAprPred(Tucuxi::GuiCore::PredictionSpec*);
    void computeAprPerc(Tucuxi::GuiCore::PredictionSpec*);
    void computeApoPred(Tucuxi::GuiCore::PredictionSpec*);
    void computeApoPerc(Tucuxi::GuiCore::PredictionSpec*);
    void computeRevPred(Tucuxi::GuiCore::PredictionSpec*);
    void computeAdjPred(Tucuxi::GuiCore::PredictionSpec*);
    void abortAll();
//    void updateAdj();

private:
    void preparePredResults(QVector<int> time, QVector<double> data, Tucuxi::GuiCore::PredictionResult* pred);
    Tucuxi::GuiCore::PredictionResult* computePrediction(const Tucuxi::Core::DrugModel& _model,
        const Tucuxi::Core::DrugTreatment &_treatment,
        Tucuxi::Core::PredictionParameterType _type,
        const Tucuxi::Common::DateTime &_startDate,
        const Tucuxi::Common::DateTime &_endDate,
        int nbPoints);

    Tucuxi::GuiCore::PredictionResult* computePercentiles(const Tucuxi::Core::DrugModel& _model,
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
