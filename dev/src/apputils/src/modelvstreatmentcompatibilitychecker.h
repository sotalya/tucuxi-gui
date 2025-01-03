/*
 * Tucuxi - Tucuxi-gui software.
 * This software is able to perform prediction of drug concentration in blood
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


#ifndef MODELVSTREATMENTCOMPATIBILITYCHECKER_H
#define MODELVSTREATMENTCOMPATIBILITYCHECKER_H

namespace Tucuxi {
namespace Gui {
namespace Core {


class DrugModel;
class DrugTreatment;

}
}
}

/**
 * @brief The ModelVsTreatmentCompatibilityChecker class
 * This class is meant to be used in order to check wether a drug model is
 * compatible with a drug treatment. It only offers a single method taking
 * a drug treatment and a drug model as parameters.
 */
class ModelVsTreatmentCompatibilityChecker
{
public:
    //! Empty constructor
    ModelVsTreatmentCompatibilityChecker();

    /**
     * @brief check Function to check compatibility of a drug model vs a treatment
     * @param model The drug model
     * @param treatment The drug treatment
     * @return true if the model is appropriate for the treatment, false else
     * This function checks the compabitility of a model for a specific
     * treatment. For now it checks that each dosage of the treatment can be
     * handled by the the model. Later on it could also use the patient
     * variates to check if the model is applicable. For instance if age is
     * greater than a certain value, then a model for neonates should not
     * be used.
     */
    bool check(Tucuxi::Gui::Core::DrugModel *model,
               Tucuxi::Gui::Core::DrugTreatment *treatment);
};

#endif // MODELVSTREATMENTCOMPATIBILITYCHECKER_H
