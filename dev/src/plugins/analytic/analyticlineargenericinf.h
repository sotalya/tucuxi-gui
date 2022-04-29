//@@license@@

#ifndef ANALYTICLINEARGENERICINF_H
#define ANALYTICLINEARGENERICINF_H

#include "drugmodel.h"

/** \ingroup analytic
 *  \brief Generic infusion predictor for the Analytic model
 */
class AnalyticLinearGenericInf : public DrugModel
{
    Q_DECLARE_TR_FUNCTIONS(AnalyticLinearGenericInf)

public:
    AnalyticLinearGenericInf();

    virtual ModelDescriptor descriptor() = 0;   

    virtual Version version() = 0;

    virtual Duration halflife(const Duration &start,
                      const ParameterSet* &parameters,
                      const SharedDosage &dosage,
                      const Residuals &residuals) = 0;

    virtual DataSet predict(const Duration &start,
                    const Duration &until,
                    const ParameterSet* &parameters,
                    uint points,
                    const SharedDosage &dosage,
                    Residuals &residuals,
                    bool uniform);

protected:
    //Computes part or complete prediction
    virtual DataSet compute( const Duration &start,
                     const Duration &until,
                     const ParameterSet* &parameters,
                     uint points,
                     const SharedDosage &dosage,
                     Residuals &residuals) = 0;


    //Used units
    Unit _ug,_ugl;

private:
    virtual DataSet steadystateImpl(const Duration &start,
                                    const Duration &duration,
                                    const ParameterSet* &parameters,
                                    const SharedDosage &dosage,
                                    const Duration &halflife,
                                    int multiplier,
                                    uint points,
                                    Residuals &residual,
                                    bool uniform,
                                    bool computeResidual);

    virtual DataSet computeSteadyState(const Duration &start,
                                       const Duration &duration,
                                       const ParameterSet* &parameters,
                                       const SharedDosage &dosage,
                                       uint points,
                                       Residuals &residual,
                                       bool computeResidual) = 0;
};

#endif // ANALYTICLINEARGENERICINF_H
