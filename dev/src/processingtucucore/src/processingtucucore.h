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
    namespace Gui {
    namespace Core {

        class ValidDose;
        class ValidDoses;
        class ValidInterval;
        class ValidInfusion;
    }
    }
}

namespace Tucuxi {
    namespace Core {
        class CacheComputing;
    }
}

namespace Tucuxi {
namespace Gui {
namespace Processing {

/**
 * This class implements the interface ProcessingInterface,
 * as defined in the UML. Points can be population or apriori.
 * Point is generally only used for aposteriori.
 * Reverse is the reverse engine.
 * calculateAposterioriPercentiles is the aposteriori percentiles.
 * @see processinginterface.h
*/
class ProcessingTucucore : public Tucuxi::Gui::Core::ProcessingInterface
{
public:
    ProcessingTucucore();
    virtual ~ProcessingTucucore();

    virtual Tucuxi::ProcessingResult points(const Tucuxi::Gui::Core::DrugResponseAnalysis* analysis,
                       const Tucuxi::Gui::Core::PointsTraits traits,
                       Tucuxi::Gui::Core::PredictionResult &prediction);

    virtual Tucuxi::ProcessingResult point(const Tucuxi::Gui::Core::DrugResponseAnalysis* analysis,
                      const Tucuxi::Gui::Core::PointTraits traits,
                      Tucuxi::Gui::Core::Amount &amount,
                      Tucuxi::Gui::Core::PredictionResult &prediction);

    virtual Tucuxi::ProcessingResult percentiles(const Tucuxi::Gui::Core::DrugResponseAnalysis* analysis,
                            const Tucuxi::Gui::Core::PercentilesTraits traits,
                            Tucuxi::Gui::Core::PredictionResult &prediction,
                            Tucuxi::math::ProcessingAborter *aborter);

    virtual Tucuxi::ProcessingResult calculateAposterioriPercentiles(const Tucuxi::Gui::Core::DrugResponseAnalysis* analysis,
                               const Tucuxi::Gui::Core::PercentilesTraits traits,
                               Tucuxi::Gui::Core::PredictionResult &prediction,
                               Tucuxi::math::ProcessingAborter *aborter);

    virtual Tucuxi::ProcessingResult computeSuggestedAdjustments(const Tucuxi::Gui::Core::DrugResponseAnalysis* analysis,
                                            const Tucuxi::Gui::Core::ReverseTraits spec,
                                            Tucuxi::Gui::Core::PredictionResult &prediction);

protected:


    virtual Tucuxi::ProcessingResult generalCalculatePercentiles(const Tucuxi::Gui::Core::DrugResponseAnalysis* analysis,
                               const Tucuxi::Gui::Core::PercentilesTraits traits,
                               Tucuxi::Gui::Core::PredictionResult &pred,
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

}
}
}

#endif // PROCESSINGTUCUCORE_H
