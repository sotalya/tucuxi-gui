#ifndef ANALYTICLINEAR3COMPBOLUS_H
#define ANALYTICLINEAR3COMPBOLUS_H

#include "drugmodel.h"

class AnalyticLinear3CompBolusK : public DrugModel
{
    Q_DECLARE_TR_FUNCTIONS(AnalyticLinear3CompBolusK)
    MODELUTILITIES(AnalyticLinear3CompBolusK)
public:
    AnalyticLinear3CompBolusK();

    ModelDescriptor descriptor ();

    Version version ();

    Duration halflife ( const Duration &start,
                                const ParameterSet* &parameters,
                                const SharedDosage &dosage,
                                const Residuals &residuals);

    DataSet predict( const Duration &start,
                             const Duration &until,
                             const ParameterSet* &parameters,
                             uint points,
                             const SharedDosage &dosage,
                             Residuals &residuals,
                             bool uniform);
protected:

    virtual bool fetchParameters(const ParameterSet* &parameters, double &CL, double &V1, double &V2, double &V3, double &Q2, double &Q3);

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

   //Used units
   Unit _ug,_ugl;
};

class AnalyticLinear3CompBolusClV : public AnalyticLinear3CompBolusK
{
    Q_DECLARE_TR_FUNCTIONS(AnalyticLinear3CompBolusClV)
    MODELUTILITIES(AnalyticLinear3CompBolusClV)
public:
    AnalyticLinear3CompBolusClV();

    ModelDescriptor descriptor ();

    Version version ();

    Duration halflife ( const Duration &start,
                                const ParameterSet* &parameters,
                                const SharedDosage &dosage,
                                const Residuals &residuals);

protected:

    virtual bool fetchParameters(const ParameterSet* &parameters, double &CL,double &V1,double &V2,double &V3,double &Q1, double &Q2);

private:

   //Used units
   Unit _ug,_ugl;
};

#endif // ANALYTICLINEAR3COMPBOLUS_H
