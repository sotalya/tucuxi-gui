#ifndef MODELVSTREATMENTCOMPATIBILITYCHECKER_H
#define MODELVSTREATMENTCOMPATIBILITYCHECKER_H

namespace Tucuxi {
namespace GuiCore {

class DrugModel;
class DrugTreatment;

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
    bool check(Tucuxi::GuiCore::DrugModel *model,
               Tucuxi::GuiCore::DrugTreatment *treatment);
};

#endif // MODELVSTREATMENTCOMPATIBILITYCHECKER_H
