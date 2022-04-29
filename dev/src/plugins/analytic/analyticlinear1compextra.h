//@@license@@

#ifndef ANALYTICLINEAR1COMPEXTRA_H
#define ANALYTICLINEAR1COMPEXTRA_H

#include <QCoreApplication>
#include <QList>
#include <QMultiMap>
#include <QStringList>

#include "ezutils.h"
#include "medical.h"
#include "drugmodel.h"
#include "distribution.h"

/** \ingroup analytic
 *  \brief Mono-compartimental extravascular predictor for the Analytic model
 */
class AnalyticLinear1CompExtraK : public DrugModel
{
    Q_DECLARE_TR_FUNCTIONS(AnalyticLinear1CompExtraK)
    MODELUTILITIES(AnalyticLinear1CompExtraK)
public:

    AnalyticLinear1CompExtraK();

    //! The id for this model is : ch.heig-vd.ezechiel.model.analytic.extra
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

protected:

    virtual bool fetchParameters(const ParameterSet* &parameters, double &V1, double &Ke, double &Ka, double &F);

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


class AnalyticLinear1CompExtraClV : public AnalyticLinear1CompExtraK
{
    Q_DECLARE_TR_FUNCTIONS(AnalyticLinear1CompExtraClV)
    MODELUTILITIES(AnalyticLinear1CompExtraClV)
public:

    AnalyticLinear1CompExtraClV();

    //! The id for this model is : ch.heig-vd.ezechiel.model.analytic.extra
    virtual ModelDescriptor descriptor ();

    virtual Version version();

    virtual Duration halflife ( const Duration &start,
                        const ParameterSet* &parameters,
                        const SharedDosage &dosage,
                        const Residuals &residuals);


private:
    virtual bool fetchParameters(const ParameterSet* &parameters, double &V1,double &Ke, double &Ka, double &F);
    //The unit we're using
    Unit _ug,_ugl;
};

#endif // ANALYTICLINEAR1COMPEXTRA_H
