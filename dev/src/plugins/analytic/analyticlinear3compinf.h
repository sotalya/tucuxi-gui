//@@license@@

#ifndef ANALYTICLINEAR3COMPINF_H
#define ANALYTICLINEAR3COMPINF_H

#include "analyticlineargenericinf.h"

class AnalyticLinear3CompInfK : public AnalyticLinearGenericInf
{
    Q_DECLARE_TR_FUNCTIONS(AnalyticLinear3CompInfK)
    MODELUTILITIES(AnalyticLinear3CompInfK)

public:
    AnalyticLinear3CompInfK();

    virtual ModelDescriptor descriptor();

    virtual Version version();

    virtual Duration halflife(const Duration &start,
                              const ParameterSet* &parameters,
                              const SharedDosage &dosage,
                              const Residuals &residuals);	

protected:
    //Computes part or complete prediction
    virtual DataSet compute(const Duration &start,
                            const Duration &until,
                            const ParameterSet* &parameters,
                            uint points, const SharedDosage &dosage,
                            Residuals &residuals);

    virtual bool fetchParameters(const ParameterSet* &parameters, double &CL, double &V1,double &V2,double &V3, double &Q2, double &Q3);

private:
    //Computes part of complete steady state cycle
    virtual DataSet computeSteadyState(const Duration &start,
                                       const Duration &duration,
                                       const ParameterSet* &parameters,
                                       const SharedDosage &dosage,
                                       uint points,
                                       Residuals &residual,
                                       bool computeResidual);
};

class AnalyticLinear3CompInfClV : public AnalyticLinear3CompInfK
{
    Q_DECLARE_TR_FUNCTIONS(AnalyticLinear3CompInfClV)
    MODELUTILITIES(AnalyticLinear3CompInfClV)

public:
    AnalyticLinear3CompInfClV();

    virtual ModelDescriptor descriptor();

    virtual Version version();

    virtual Duration halflife(const Duration &start,
                              const ParameterSet* &parameters,
                              const SharedDosage &dosage,
                              const Residuals &residuals);

protected:

    virtual bool fetchParameters(const ParameterSet* &parameters, double &CL, double &V1,double &V2,double &V3, double &Q2, double &Q3);

};

#endif // ANALYTICLINEAR3COMPINF_H
