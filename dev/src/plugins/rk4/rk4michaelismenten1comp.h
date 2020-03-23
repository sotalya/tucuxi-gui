#ifndef RK4MMPREDICTOR_H
#define RK4MMPREDICTOR_H

#include <QCoreApplication>
#include <QList>
#include <QMultiMap>
#include <QStringList>

#include "ezutils.h"
#include "medical.h"
#include "drugmodel.h"
#include "dataset.h"
#include "distribution.h"
#include "rk4genericmodel.h"

//! \brief The RK4 model
/*! \ingroup rk4
 * Base class for all the drug intakes.
 */
class RK4MichaelisMenten1CompModel : public RK4GenericModel
{
    Q_DECLARE_TR_FUNCTIONS(RK4MichaelisMenten1CompModel)    
public:

    RK4MichaelisMenten1CompModel();    

    Version version ();

    Duration halflife ( const Duration &start,
                        const ParameterSet* &parameters,
                        const SharedDosage &dosage,
                        const Residuals &residuals);

protected:
    virtual void getParameters(const ParameterSet* &parameters);

    //Used units
    Unit _ug,_ugl;
    //Some variable put there to not have to pass them as a function parameter (sic)
    double V,Km,Vm;
};

//! \brief The RK4 bolus model
/*! \ingroup rk4
 * Bolus model.
 */
class RK4MichaelisMenten1CompBolus : public RK4MichaelisMenten1CompModel {
    Q_DECLARE_TR_FUNCTIONS(RK4MichaelisMenten1CompBolus)
    MODELUTILITIES(RK4MichaelisMenten1CompBolus)
public:

    RK4MichaelisMenten1CompBolus ();

    virtual ModelDescriptor descriptor ();

private:
    virtual void yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result);
    virtual QVector<double> y0 ();
};

//! \brief The RK4 infusion model
/*! \ingroup rk4
 * Infusion model.
 */
class RK4MichaelisMenten1CompInf  : public RK4MichaelisMenten1CompModel {
    Q_DECLARE_TR_FUNCTIONS(RK4MichaelisMenten1CompInf)
    MODELUTILITIES(RK4MichaelisMenten1CompInf)

public:
    RK4MichaelisMenten1CompInf ();

    virtual ModelDescriptor descriptor ();

    virtual DataSet predict(const Duration &start,
                            const Duration &until,
                            const ParameterSet* &parameters,
                            uint points,
                            const SharedDosage &dosage,
                            Residuals &residuals,
                            bool uniform);
private:
    virtual void yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result);
};

//! \brief The RK4 extra model
/*! \ingroup rk4
 * Extravascular model.
 */
class RK4MichaelisMenten1CompExtraInf : public RK4MichaelisMenten1CompModel {
    Q_DECLARE_TR_FUNCTIONS(RK4MichaelisMenten1CompExtraInf)
    MODELUTILITIES(RK4MichaelisMenten1CompExtraInf)
public:
    RK4MichaelisMenten1CompExtraInf ();

    virtual ModelDescriptor descriptor ();


    virtual DataSet predict(const Duration &start,
                            const Duration &until,
                            const ParameterSet* &parameters,
                            uint points,
                            const SharedDosage &dosage,
                            Residuals &residuals,
                            bool uniform);
private:
    virtual void getParameters(const ParameterSet* &parameters);
    virtual void yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result);
    virtual QVector<double> y0 ();
protected:
    double F,Ka;
};



//! \brief The RK4 extra model
/*! \ingroup rk4
 * Extravascular model.
 */
class RK4MichaelisMenten1CompExtra : public RK4MichaelisMenten1CompModel {
    Q_DECLARE_TR_FUNCTIONS(RK4MichaelisMenten1CompExtra)
    MODELUTILITIES(RK4MichaelisMenten1CompExtra)
public:
    RK4MichaelisMenten1CompExtra ();
    virtual ModelDescriptor descriptor ();    
private:
    virtual void getParameters(const ParameterSet* &parameters);
    virtual void yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result);
    virtual QVector<double> y0 ();
protected:
    double F,Ka;
};


//! \brief The RK4 extra model
/*! \ingroup rk4
 * Extravascular model.
 */
class RK4MichaelisMenten1CompExtraVmOverV : public RK4MichaelisMenten1CompExtra {
    Q_DECLARE_TR_FUNCTIONS(RK4MichaelisMenten1CompExtraVmOverV)
    MODELUTILITIES(RK4MichaelisMenten1CompExtraVmOverV)
public:
    RK4MichaelisMenten1CompExtraVmOverV ();
    virtual ModelDescriptor descriptor ();    
private:
    virtual void yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result);
};


//! \brief The RK4 inf model
/*! \ingroup rk4
 * Infusion model.
 */
class RK4MichaelisMenten1CompInfVmOverV : public RK4MichaelisMenten1CompInf {
    Q_DECLARE_TR_FUNCTIONS(RK4MichaelisMenten1CompInfVmOverV)
    MODELUTILITIES(RK4MichaelisMenten1CompInfVmOverV)
public:
    RK4MichaelisMenten1CompInfVmOverV ();
    virtual ModelDescriptor descriptor ();    
private:
    virtual void yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result);
};

//! \brief The RK4 blus model
/*! \ingroup rk4
 * Bolus model.
 */
class RK4MichaelisMenten1CompBolusVmOverV : public RK4MichaelisMenten1CompBolus {
    Q_DECLARE_TR_FUNCTIONS(RK4MichaelisMenten1CompBolusVmOverV)
    MODELUTILITIES(RK4MichaelisMenten1CompBolusVmOverV)
public:
    RK4MichaelisMenten1CompBolusVmOverV ();
    virtual ModelDescriptor descriptor ();    
private:
    virtual void yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result);
};


#endif // RK4MMPREDICTOR_H
