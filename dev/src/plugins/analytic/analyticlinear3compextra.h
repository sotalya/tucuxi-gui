#ifndef ANALYTICLINEAR3COMPEXTRA_H
#define ANALYTICLINEAR3COMPEXTRA_H

#include "drugmodel.h"

class AnalyticLinear3CompExtraK : public DrugModel
{
    Q_DECLARE_TR_FUNCTIONS(AnalyticLinear3CompExtraK)
    MODELUTILITIES(AnalyticLinear3CompExtraK)

public:
    AnalyticLinear3CompExtraK();

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

    virtual bool fetchParameters(const ParameterSet* &parameters, double &CL,double &V1, double &V2, double &V3, double &Q2, double &Q3, double &Ka, double &F);

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

    //The unit we're using
    Unit _ug,_ugl;

};

class AnalyticLinear3CompExtraClV : public AnalyticLinear3CompExtraK
{
    Q_DECLARE_TR_FUNCTIONS(AnalyticLinear3CompExtraClV)
    MODELUTILITIES(AnalyticLinear3CompExtraClV)

public:
    AnalyticLinear3CompExtraClV();

    ModelDescriptor descriptor ();

    Version version ();

    Duration halflife ( const Duration &start,
                        const ParameterSet* &parameters,
                        const SharedDosage &dosage,
                        const Residuals &residuals);

protected:

    virtual bool fetchParameters(const ParameterSet* &parameters, double &CL,double &V1, double &V2, double &V3, double &Q2, double &Q3, double &Ka, double &F);

private:

    //The unit we're using
    Unit _ug,_ugl;

};


#endif // ANALYTICLINEAR3COMPEXTRA_H
