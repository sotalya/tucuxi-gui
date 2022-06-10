//@@license@@

#ifndef PROCESSINGTUCUCORE_H
#define PROCESSINGTUCUCORE_H


#include "core/errors_processing.h"

#include "core/interfaces/processinginterface.h"
#include "core/utils/logging.h"


namespace Tucuxi {
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
class ProcessingTucucore : public Tucuxi::GuiCore::ProcessingInterface
{
public:
    ProcessingTucucore();
    virtual ~ProcessingTucucore();

    virtual Tucuxi::ProcessingResult points(const Tucuxi::GuiCore::DrugResponseAnalysis* analysis,
                       const Tucuxi::GuiCore::PointsTraits traits,
                       Tucuxi::GuiCore::PredictionResult &prediction);

    virtual Tucuxi::ProcessingResult point(const Tucuxi::GuiCore::DrugResponseAnalysis* analysis,
                      const Tucuxi::GuiCore::PointTraits traits,
                      Tucuxi::GuiCore::Amount &amount,
                      Tucuxi::GuiCore::PredictionResult &prediction);

    virtual Tucuxi::ProcessingResult percentiles(const Tucuxi::GuiCore::DrugResponseAnalysis* analysis,
                            const Tucuxi::GuiCore::PercentilesTraits traits,
                            Tucuxi::GuiCore::PredictionResult &prediction,
                            Tucuxi::math::ProcessingAborter *aborter);

    virtual Tucuxi::ProcessingResult calculateAposterioriPercentiles(const Tucuxi::GuiCore::DrugResponseAnalysis* analysis,
                               const Tucuxi::GuiCore::PercentilesTraits traits,
                               Tucuxi::GuiCore::PredictionResult &prediction,
                               Tucuxi::math::ProcessingAborter *aborter);

    virtual Tucuxi::ProcessingResult computeSuggestedAdjustments(const Tucuxi::GuiCore::DrugResponseAnalysis* analysis,
                                            const Tucuxi::GuiCore::ReverseTraits spec,
                                            Tucuxi::GuiCore::PredictionResult &prediction);

protected:


    virtual Tucuxi::ProcessingResult generalCalculatePercentiles(const Tucuxi::GuiCore::DrugResponseAnalysis* analysis,
                               const Tucuxi::GuiCore::PercentilesTraits traits,
                               Tucuxi::GuiCore::PredictionResult &pred,
                               Tucuxi::math::ProcessingAborter *aborter);

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
