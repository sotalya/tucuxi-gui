//@@license@@

#ifndef ANALYTICLINEAR1COMPINF_1_H
#define ANALYTICLINEAR1COMPINF_1_H


#include <QCoreApplication>
#include <QList>
#include <QMultiMap>
#include <QStringList>

#include "ezutils.h"
#include "medical.h"
#include "analyticlineargenericinf.h"
#include "distribution.h"

/** \ingroup analytic
 *  \brief Mono-compartimental infusion predictor for the Analytic model
 */
class AnalyticLinear1CompInfK : public AnalyticLinearGenericInf
{
    Q_DECLARE_TR_FUNCTIONS(AnalyticLinear1CompInfK)
    MODELUTILITIES(AnalyticLinear1CompInfK)

public:
    AnalyticLinear1CompInfK();    

    virtual ModelDescriptor descriptor();

    virtual Version version();

    virtual Duration halflife(const Duration &start,
                              const ParameterSet* &parameters,
                              const SharedDosage &dosage,
                              const Residuals &residuals);

    int priority() const;

protected:

    virtual bool fetchParameters(const ParameterSet* &parameters, double &CL,double &V);

protected:
    //Computes part or complete prediction
    virtual DataSet compute(const Duration &start,
                            const Duration &until,
                            const ParameterSet* &parameters,
                            uint points,
                            const SharedDosage &dosage,
                            Residuals &residuals);

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

class AnalyticLinear1CompInfClV : public AnalyticLinear1CompInfK
{
    Q_DECLARE_TR_FUNCTIONS(AnalyticLinear1CompInfClV)
    MODELUTILITIES(AnalyticLinear1CompInfClV)

public:
    AnalyticLinear1CompInfClV();

    virtual ModelDescriptor descriptor();

    virtual Version version();

    virtual Duration halflife(const Duration &start,
                              const ParameterSet* &parameters,
                              const SharedDosage &dosage,
                              const Residuals &residuals);       

protected:

    virtual bool fetchParameters(const ParameterSet* &parameters, double &CL,double &V);

};

#endif // ANALYTICLINEAR1COMPINF_1_H
