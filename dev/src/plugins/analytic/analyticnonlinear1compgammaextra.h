#ifndef AnalyticNonlinear1CompGammaExtra_H
#define AnalyticNonlinear1CompGammaExtra_H

#include <QCoreApplication>
#include <QList>
#include <QMultiMap>
#include <QStringList>

#include "ezutils.h"
#include "medical.h"
#include "drugmodel.h"
#include "distribution.h"

class AnalyticNonlinear1CompGammaExtraK : public DrugModel
{
    Q_DECLARE_TR_FUNCTIONS(AnalyticNonlinear1CompGammaExtraK)
    MODELUTILITIES(AnalyticNonlinear1CompGammaExtraK)
public:

    AnalyticNonlinear1CompGammaExtraK();

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

    virtual bool fetchParameters(const ParameterSet* &parameters, double &Ke,double &V,double &A,double &Ga1, double &Gb1, double &Ga2, double &Gb2, double &F, double &R);

private:

    //The unit we're using
    Unit _ug,_ugl;
};


class AnalyticNonlinear1CompGammaExtraClV : public AnalyticNonlinear1CompGammaExtraK
{
    Q_DECLARE_TR_FUNCTIONS(AnalyticNonlinear1CompGammaExtraClV)
    MODELUTILITIES(AnalyticNonlinear1CompGammaExtraClV)
public:

    AnalyticNonlinear1CompGammaExtraClV();

    virtual ModelDescriptor descriptor ();

    virtual Version version();

    virtual Duration halflife ( const Duration &start,
                        const ParameterSet* &parameters,
                        const SharedDosage &dosage,
                        const Residuals &residuals);

protected:

    virtual bool fetchParameters(const ParameterSet* &parameters, double &Ke,double &V,double &A,double &Ga1, double &Gb1, double &Ga2, double &Gb2, double &F, double &R);

private:

    //The unit we're using
    Unit _ug,_ugl;
};

#endif // AnalyticNonlinear1CompGammaExtra_H
