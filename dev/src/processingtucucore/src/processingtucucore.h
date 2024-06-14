/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
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
