#ifndef ABIINTRAPREDICTOR_H
#define ABIINTRAPREDICTOR_H

#include <QCoreApplication>
#include <QList>
#include <QMap>
#include <QStringList>

#include "ezutils.h"
#include "medical.h"
#include "drugmodel.h"
#include "distribution.h"

/** \ingroup analytic
 *  \brief Bi-compartimental bolus predictor for the Analytic model
 */
class AnalyticLinear2CompBolusK : public DrugModel
{
    Q_DECLARE_TR_FUNCTIONS(AnalyticLinear2CompBolusK)
    MODELUTILITIES(AnalyticLinear2CompBolusK)
public:
    AnalyticLinear2CompBolusK();

    virtual ModelDescriptor descriptor ();

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
    virtual bool fetchParameters(const ParameterSet* &parameters, double &CL,double &V1,double &V2,double &Q);

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

class AnalyticLinear2CompBolusClV : public AnalyticLinear2CompBolusK
{
    Q_DECLARE_TR_FUNCTIONS(AnalyticLinear2CompBolusClV)
    MODELUTILITIES(AnalyticLinear2CompBolusClV)
public:
    AnalyticLinear2CompBolusClV();

    virtual ModelDescriptor descriptor ();

    Version version ();

    Duration halflife ( const Duration &start,
                                const ParameterSet* &parameters,
                                const SharedDosage &dosage,
                                const Residuals &residuals);

protected:

     virtual bool fetchParameters(const ParameterSet* &parameters, double &CL,double &V1,double &kV2,double &Q);

private:

   //Used units
   Unit _ug,_ugl;
};

#endif // ABIINTRAPREDICTOR_H
