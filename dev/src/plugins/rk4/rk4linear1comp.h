//@@license@@

#ifndef RK4LINEAR1MODEL_H
#define RK4LINEAR1MODEL_H

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
class RK4Linear1CompModel : public RK4GenericModel
{
    Q_DECLARE_TR_FUNCTIONS(RK4Linear1CompModel)
public:

    RK4Linear1CompModel();

    Version version();

    Duration halflife ( const Duration &start,
                        const ParameterSet* &parameters,
                        const SharedDosage &dosage,
                        const Residuals &residuals);

protected:

    virtual void getParameters(const ParameterSet* &parameters) = 0;

    QVector<double> y0() = 0;
      //Used units
    Unit _ug,_ugl;
    //Some variable put there to not have to pass them as a function parameter (sic)
    double CL,V,Ke;
};

//! \brief The RK4 bolus model
/*! \ingroup rk4
 * Bolus model.
 */
class RK4Linear1CompBolusK : public RK4Linear1CompModel {

    Q_DECLARE_TR_FUNCTIONS(RK4Linear1CompBolusK)
    MODELUTILITIES(RK4Linear1CompBolusK)

public:

    RK4Linear1CompBolusK();

    ModelDescriptor descriptor ();

protected:

    virtual void getParameters(const ParameterSet* &parameters);

    void yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result);

    QVector<double> y0 ();
};

class RK4Linear1CompBolusClV : public RK4Linear1CompBolusK {

    Q_DECLARE_TR_FUNCTIONS(RK4Linear1CompBolusClV)
    MODELUTILITIES(RK4Linear1CompBolusClV)

public:

    RK4Linear1CompBolusClV();

    ModelDescriptor descriptor ();

protected:

    virtual void getParameters(const ParameterSet* &parameters);
};

//! \brief The RK4 infusion model
/*! \ingroup rk4
 * Infusion model.
 */
class RK4Linear1CompInfK  : public RK4Linear1CompModel {
    Q_DECLARE_TR_FUNCTIONS(RK4Linear1CompInfK)
    MODELUTILITIES(RK4Linear1CompInfK)

public:

    RK4Linear1CompInfK();

    ModelDescriptor descriptor ();

    DataSet predict(const Duration &start,
                    const Duration &until,
                    const ParameterSet* &parameters,
                    uint points,
                    const SharedDosage &dosage,
                    Residuals &residuals,
                    bool uniform);

protected:

    void yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result);

    virtual QVector<double> y0 ();

private:
    virtual void getParameters(const ParameterSet* &parameters);
};

class RK4Linear1CompInfClV  : public RK4Linear1CompInfK {
    Q_DECLARE_TR_FUNCTIONS(RK4Linear1CompInfClV)
    MODELUTILITIES(RK4Linear1CompInfClV)

public:

    RK4Linear1CompInfClV();

    ModelDescriptor descriptor ();

private:

    virtual void getParameters(const ParameterSet* &parameters);
};

class RK4Linear1CompExtraK: public RK4Linear1CompModel {

    Q_DECLARE_TR_FUNCTIONS(RK4Linear1CompExtraK)
    MODELUTILITIES(RK4Linear1CompExtraK)

public:
    RK4Linear1CompExtraK();

    ModelDescriptor descriptor ();

protected:

    void yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result);

    QVector<double> y0 ();

    double Ka,F;

private:
    virtual void getParameters(const ParameterSet* &parameters);
};

class RK4Linear1CompExtraClV: public RK4Linear1CompExtraK {

    Q_DECLARE_TR_FUNCTIONS(RK4Linear1CompExtraClV)
    MODELUTILITIES(RK4Linear1CompExtraClV)

public:
    RK4Linear1CompExtraClV();
    ModelDescriptor descriptor ();

private:

    virtual void getParameters(const ParameterSet* &parameters);
};


#endif // RK4LINEAR1MODEL_H
