//@@license@@

#ifndef ABIEXTRAPREDICTOR_H
#define ABIEXTRAPREDICTOR_H

#include <QCoreApplication>
#include <QList>
#include <QMultiMap>
#include <QStringList>

#include "ezutils.h"
#include "medical.h"
#include "drugmodel.h"
#include "distribution.h"

/** \ingroup analytic
 *  \brief Bi-compartimental extravascular predictor for the Analytic model
 */
class AnalyticLinear2CompExtraK : public DrugModel
{
    Q_DECLARE_TR_FUNCTIONS(AnalyticLinear2CompExtraK)
    MODELUTILITIES(AnalyticLinear2CompExtraK)
public:
    AnalyticLinear2CompExtraK();

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
    virtual bool fetchParameters(const ParameterSet* &parameters, double &CL,double &V1, double &V2, double &Q, double &Ka, double &F);

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

class AnalyticLinear2CompExtraClV : public AnalyticLinear2CompExtraK
{
    Q_DECLARE_TR_FUNCTIONS(AnalyticLinear2CompExtraClV)
    MODELUTILITIES(AnalyticLinear2CompExtraClV)
public:
    AnalyticLinear2CompExtraClV();

    ModelDescriptor descriptor ();

    Version version ();

    Duration halflife ( const Duration &start,
                        const ParameterSet* &parameters,
                        const SharedDosage &dosage,
                        const Residuals &residuals);

protected:

    virtual bool fetchParameters(const ParameterSet* &parameters, double &CL,double &V1, double &V2, double &Q, double &Ka, double &F);

private:

    //The unit we're using
    Unit _ug,_ugl;

};

#endif // ABIEXTRAPREDICTOR_H
