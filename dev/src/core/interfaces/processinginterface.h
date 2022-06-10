//@@license@@

#ifndef PROCESSINGINTERFACE_H
#define PROCESSINGINTERFACE_H

#include "core/utils/dataset.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/interfaces/processingtraits.h"
#include "core/dal/amount.h"
#include "core/dal/dosage.h"
#include "core/dal/predictionresult.h"
#include "core/utils/logging.h"

namespace Tucuxi {
namespace math {
    class ProcessingAborter;
}

namespace Gui {
namespace Core {


/**
 * This interface is the entry point for any calculation.
 * There will exist two implementation:
 * - ProcessingMath which will actually do the computation
 * - ProcessingProxy that will communicate with an external server to let him do the job.
 *
 * Everytime we add a method in this interface all subclasses should be updated accordingly.
 * We have to be careful about the objects we pass to it. For instance, the pointer to
 * the prediction could point to an object that could be modified during the calculation.
 * The caller should be aware that during calculation the object should not change.
 * So, maybe there should be a facade for before ProcessingMath in order to copy all the
 * objects in order to get rid of any multithreaded issue.
 *
 * The methods of this interface are services, and should not keep an internal state in the objects.
 */
class ProcessingInterface
{
public:
    /**
     * Points are calculated here
     * @param analysis the drugresponseanalysis with information from a drug and a patient
     * @param traits information about the points to calculate
     * @param pred prediction to hold the state of the curve(s)
     */
    virtual ProcessingResult points(const Tucuxi::Gui::Core::DrugResponseAnalysis* analysis,
                       const Tucuxi::Gui::Core::PointsTraits traits,
                       Tucuxi::Gui::Core::PredictionResult &pred) = 0;

    /**
     * Single points are calculated here in contrast to a series of contiguous points @see points
     * The difference is that this method only calculates residuals until/between the point(s) of interest
     * @param analysis the drugresponseanalysis with information from a drug and a patient
     * @param traits information about the points to calculate
     * @param amount holds a single point of interest (obsolete)
     * @param pred prediction to hold the state of the point(s)
     */
    virtual ProcessingResult point(const Tucuxi::Gui::Core::DrugResponseAnalysis* analysis,
                      const Tucuxi::Gui::Core::PointTraits traits,
                      Tucuxi::Gui::Core::Amount &amount,
                       Tucuxi::Gui::Core::PredictionResult &pred) = 0;

    /**
     * Percentiles are calculated here for apriori and population
     * @param analysis the drugresponseanalysis with information from a drug and a patient
     * @param traits information about the points to calculate
     * @param pred prediction to hold the state of the point(s)
     * @param aborter responsible for aborting long running calculations
     */
    virtual ProcessingResult percentiles(const Tucuxi::Gui::Core::DrugResponseAnalysis* prediction,
                            const Tucuxi::Gui::Core::PercentilesTraits traits,
                            Tucuxi::Gui::Core::PredictionResult &pred,
                            Tucuxi::math::ProcessingAborter *aborter = 0) = 0;

    /**
     * Percentiles are calculated here for aposteriori
     * @param analysis the drugresponseanalysis with information from a drug and a patient
     * @param traits information about the points to calculate
     * @param pred prediction to hold the state of the point(s)
     * @param aborter responsible for aborting long running calculations
     */
    virtual ProcessingResult calculateAposterioriPercentiles(const Tucuxi::Gui::Core::DrugResponseAnalysis* prediction,
                               const Tucuxi::Gui::Core::PercentilesTraits traits,
                               Tucuxi::Gui::Core::PredictionResult &pred,
                               Tucuxi::math::ProcessingAborter *aborter = 0) = 0;

    /**
     * Percentiles are calculated here for aposteriori
     * @param analysis the drugresponseanalysis with information from a drug and a patient
     * @param traits information about the points to calculate
     * @param pred prediction to hold the state of the curve(s)
     */
    virtual ProcessingResult computeSuggestedAdjustments(const Tucuxi::Gui::Core::DrugResponseAnalysis* analysis,
                        const Tucuxi::Gui::Core::ReverseTraits traits,
                        Tucuxi::Gui::Core::PredictionResult &pred) = 0;
};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

#endif // PROCESSINGINTERFACE_H
