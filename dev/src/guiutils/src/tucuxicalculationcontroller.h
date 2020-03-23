#ifndef TUCUXICALCULATIONCONTROLLER_H
#define TUCUXICALCULATIONCONTROLLER_H

#include "calculationcontroller.h"

class TucuxiCalculationController : public CalculationController
{
    Q_OBJECT
public:
    explicit TucuxiCalculationController(QObject *parent = nullptr);


public slots:
    void computePopPred(ezechiel::core::PredictionSpec*);
    void computePopPerc(ezechiel::core::PredictionSpec*);
    void computeAprPred(ezechiel::core::PredictionSpec*);
    void computeAprPerc(ezechiel::core::PredictionSpec*);
    void computeApoPred(ezechiel::core::PredictionSpec*);
    void computeApoPerc(ezechiel::core::PredictionSpec*);
    void computeRevPred(ezechiel::core::PredictionSpec*);
    void computeAdjPred(ezechiel::core::PredictionSpec*);
    void abortAll();
//    void updateAdj();

private:
    void preparePredResults(QVector<int> time, QVector<double> data, ezechiel::core::PredictionResult* pred);
    ezechiel::core::PredictionResult* computePrediction(const Tucuxi::Core::DrugModel& _model,
        const Tucuxi::Core::DrugTreatment &_treatment,
        Tucuxi::Core::PredictionParameterType _type,
        const Tucuxi::Common::DateTime &_startDate,
        const Tucuxi::Common::DateTime &_endDate,
        int nbPoints);

    ezechiel::core::PredictionResult* computePercentiles(const Tucuxi::Core::DrugModel& _model,
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
