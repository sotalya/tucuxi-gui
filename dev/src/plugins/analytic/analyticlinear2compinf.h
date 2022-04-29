//@@license@@

#ifndef ABIPERFPREDICTOR_H
#define ABIPERFPREDICTOR_H

#include <QCoreApplication>
#include <QList>
#include <QMultiMap>
#include <QStringList>

#include "ezutils.h"
#include "medical.h"
#include "analyticlineargenericinf.h"
#include "distribution.h"

/** \ingroup analytic
 *  \brief Bi-compartimental infusion predictor for the Analytic model
 */
class AnalyticLinear2CompInfK : public AnalyticLinearGenericInf
{
    Q_DECLARE_TR_FUNCTIONS(AnalyticLinear2CompInfK)
    MODELUTILITIES(AnalyticLinear2CompInfK)

public:
    AnalyticLinear2CompInfK();

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
                            uint points,
                            const SharedDosage &dosage,
                            Residuals &residuals);

    virtual bool fetchParameters(const ParameterSet* &parameters, double &V1,double &k12,double &k21,double &Ke);

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

/** \ingroup analytic
 *  \brief Bi-compartimental infusion predictor for the Analytic model
 */
class AnalyticLinear2CompInfClV : public AnalyticLinear2CompInfK
{
    Q_DECLARE_TR_FUNCTIONS(AnalyticLinear2CompInfClV)
    MODELUTILITIES(AnalyticLinear2CompInfClV)

public:
    AnalyticLinear2CompInfClV();

    virtual ModelDescriptor descriptor();

    virtual Version version();

    virtual Duration halflife(const Duration &start,
                              const ParameterSet* &parameters,
                              const SharedDosage &dosage,
                              const Residuals &residuals);

protected:

    virtual bool fetchParameters(const ParameterSet* &parameters, double &V1,double &k12,double &k21,double &Ke);

};

#endif // ABIPERFPREDICTOR_H
