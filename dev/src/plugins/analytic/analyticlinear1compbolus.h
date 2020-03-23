#ifndef AMONOINTRAPREDICTOR_H
#define AMONOINTRAPREDICTOR_H

#include <QCoreApplication>
#include <QList>
#include <QMap>
#include <QStringList>

#include "ezutils.h"
#include "medical.h"
#include "drugmodel.h"
#include "distribution.h"

/** \ingroup analytic
 * \brief Mono-compartimental bolus predictor for the Analytic model
 */
class AnalyticLinear1CompBolusK : public DrugModel
{
    Q_DECLARE_TR_FUNCTIONS(AnalyticLinear1CompBolusK)
    MODELUTILITIES(AnalyticLinear1CompBolusK)
public:

    AnalyticLinear1CompBolusK();

    virtual ModelDescriptor descriptor ();

    virtual Version version ();

    virtual Duration halflife ( const Duration &start,
                                const ParameterSet* &parameters,
                                const SharedDosage &dosage,
                                const Residuals &residuals);

    virtual DataSet predict( const Duration &start,
                             const Duration &until,
                             const ParameterSet* &parameters,
                             uint points,
                             const SharedDosage &dosage,
                             Residuals &residuals,
                             bool uniform);

private:

    virtual bool fetchParameters(const ParameterSet* &parameters, double &V, double &Ke);

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

class AnalyticLinear1CompBolusClV : public AnalyticLinear1CompBolusK
{
    Q_DECLARE_TR_FUNCTIONS(AnalyticLinear1CompBolusClV)
    MODELUTILITIES(AnalyticLinear1CompBolusClV)
public:

    AnalyticLinear1CompBolusClV();

    virtual ModelDescriptor descriptor ();

    virtual Version version ();

    virtual Duration halflife ( const Duration &start,
                                const ParameterSet* &parameters,
                                const SharedDosage &dosage,
                                const Residuals &residuals);


private:
    virtual bool fetchParameters(const ParameterSet* &parameters, double &V, double &Ke);

    //The unit we're using
    Unit _ug,_ugl;
};

#endif // AMONOINTRAPREDICTOR_H
