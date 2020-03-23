#ifndef PROCESSINGTUCUCORE_H
#define PROCESSINGTUCUCORE_H


#include "core/errors_processing.h"

#include "core/interfaces/processinginterface.h"
#include "core/utils/logging.h"


namespace ezechiel {
    namespace processing {
        class PredictionCalculator;
        class TargetEvaluator;
    }
    namespace core {
        class ValidDose;
        class ValidDoses;
        class ValidInterval;
        class ValidInfusion;
    }
}

/**
 * This class implements the interface ProcessingInterface,
 * as defined in the UML. Points can be population or apriori.
 * Point is generally only used for aposteriori.
 * Reverse is the reverse engine.
 * calculateAposterioriPercentiles is the aposteriori percentiles.
 * @see processinginterface.h
*/
class ProcessingTucucore : public ezechiel::core::ProcessingInterface
{
public:
    ProcessingTucucore();
    virtual ~ProcessingTucucore();

    virtual ezechiel::ProcessingResult points(const ezechiel::core::DrugResponseAnalysis* analysis,
                       const ezechiel::core::PointsTraits traits,
                       ezechiel::core::PredictionResult &prediction);

    virtual ezechiel::ProcessingResult point(const ezechiel::core::DrugResponseAnalysis* analysis,
                      const ezechiel::core::PointTraits traits,
                      ezechiel::core::Amount &amount,
                      ezechiel::core::PredictionResult &prediction);

    virtual ezechiel::ProcessingResult percentiles(const ezechiel::core::DrugResponseAnalysis* analysis,
                            const ezechiel::core::PercentilesTraits traits,
                            ezechiel::core::PredictionResult &prediction,
                            ezechiel::math::ProcessingAborter *aborter);

    virtual ezechiel::ProcessingResult calculateAposterioriPercentiles(const ezechiel::core::DrugResponseAnalysis* analysis,
                               const ezechiel::core::PercentilesTraits traits,
                               ezechiel::core::PredictionResult &prediction,
                               ezechiel::math::ProcessingAborter *aborter);

    virtual ezechiel::ProcessingResult computeSuggestedAdjustments(const ezechiel::core::DrugResponseAnalysis* analysis,
                                            const ezechiel::core::ReverseTraits spec,
                                            ezechiel::core::PredictionResult &prediction);

protected:


    virtual ezechiel::ProcessingResult generalCalculatePercentiles(const ezechiel::core::DrugResponseAnalysis* analysis,
                               const ezechiel::core::PercentilesTraits traits,
                               ezechiel::core::PredictionResult &pred,
                               ezechiel::math::ProcessingAborter *aborter);

    int m_requestID;

};

#endif // PROCESSINGTUCUCORE_H
