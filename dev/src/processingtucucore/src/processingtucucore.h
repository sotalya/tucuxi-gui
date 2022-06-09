//@@license@@

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
    namespace GuiCore {
        class ValidDose;
        class ValidDoses;
        class ValidInterval;
        class ValidInfusion;
    }
}

namespace Tucuxi {
    namespace Core {
        class CacheComputing;
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
class ProcessingTucucore : public ezechiel::GuiCore::ProcessingInterface
{
public:
    ProcessingTucucore();
    virtual ~ProcessingTucucore();

    virtual ezechiel::ProcessingResult points(const ezechiel::GuiCore::DrugResponseAnalysis* analysis,
                       const ezechiel::GuiCore::PointsTraits traits,
                       ezechiel::GuiCore::PredictionResult &prediction);

    virtual ezechiel::ProcessingResult point(const ezechiel::GuiCore::DrugResponseAnalysis* analysis,
                      const ezechiel::GuiCore::PointTraits traits,
                      ezechiel::GuiCore::Amount &amount,
                      ezechiel::GuiCore::PredictionResult &prediction);

    virtual ezechiel::ProcessingResult percentiles(const ezechiel::GuiCore::DrugResponseAnalysis* analysis,
                            const ezechiel::GuiCore::PercentilesTraits traits,
                            ezechiel::GuiCore::PredictionResult &prediction,
                            ezechiel::math::ProcessingAborter *aborter);

    virtual ezechiel::ProcessingResult calculateAposterioriPercentiles(const ezechiel::GuiCore::DrugResponseAnalysis* analysis,
                               const ezechiel::GuiCore::PercentilesTraits traits,
                               ezechiel::GuiCore::PredictionResult &prediction,
                               ezechiel::math::ProcessingAborter *aborter);

    virtual ezechiel::ProcessingResult computeSuggestedAdjustments(const ezechiel::GuiCore::DrugResponseAnalysis* analysis,
                                            const ezechiel::GuiCore::ReverseTraits spec,
                                            ezechiel::GuiCore::PredictionResult &prediction);

protected:


    virtual ezechiel::ProcessingResult generalCalculatePercentiles(const ezechiel::GuiCore::DrugResponseAnalysis* analysis,
                               const ezechiel::GuiCore::PercentilesTraits traits,
                               ezechiel::GuiCore::PredictionResult &pred,
                               ezechiel::math::ProcessingAborter *aborter);

    int m_requestID;

    ///
    /// \brief m_caches A vector of caches for the percentiles
    ///
    /// [0] : Typical patient percentiles
    /// [1] : A priori percentiles
    /// [2] : A posteriori percentiles
    /// [3] : Adjustment percentiles
    ///
    std::vector<std::unique_ptr<Tucuxi::Core::CacheComputing> > m_caches;
};

#endif // PROCESSINGTUCUCORE_H
