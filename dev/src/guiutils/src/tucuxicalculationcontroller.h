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
