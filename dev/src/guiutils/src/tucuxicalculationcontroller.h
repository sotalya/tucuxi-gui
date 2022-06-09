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
    void computePopPred(ezechiel::GuiCore::PredictionSpec*);
    void computePopPerc(ezechiel::GuiCore::PredictionSpec*);
    void computeAprPred(ezechiel::GuiCore::PredictionSpec*);
    void computeAprPerc(ezechiel::GuiCore::PredictionSpec*);
    void computeApoPred(ezechiel::GuiCore::PredictionSpec*);
    void computeApoPerc(ezechiel::GuiCore::PredictionSpec*);
    void computeRevPred(ezechiel::GuiCore::PredictionSpec*);
    void computeAdjPred(ezechiel::GuiCore::PredictionSpec*);
    void abortAll();
//    void updateAdj();

private:
    void preparePredResults(QVector<int> time, QVector<double> data, ezechiel::GuiCore::PredictionResult* pred);
    ezechiel::GuiCore::PredictionResult* computePrediction(const Tucuxi::Core::DrugModel& _model,
        const Tucuxi::Core::DrugTreatment &_treatment,
        Tucuxi::Core::PredictionParameterType _type,
        const Tucuxi::Common::DateTime &_startDate,
        const Tucuxi::Common::DateTime &_endDate,
        int nbPoints);

    ezechiel::GuiCore::PredictionResult* computePercentiles(const Tucuxi::Core::DrugModel& _model,
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
