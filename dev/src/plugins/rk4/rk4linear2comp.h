#ifndef RK4LINEAR2MODEL_H
#define RK4LINEAR2MODEL_H

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

class RK4Linear2CompModel : public RK4GenericModel
{
    Q_DECLARE_TR_FUNCTIONS(RK4Linear2CompModel)    
public:
    RK4Linear2CompModel();


    Version version ();

    Duration halflife ( const Duration &start,
                        const ParameterSet* &parameters,
                        const SharedDosage &dosage,
                        const Residuals &residuals);

protected:
    //Used units
    Unit _ug,_ugl;
    //Some variable put there to not have to pass them as a function parameter (sic)
    double CL,V1,V2,Q;
    double Ke,K12,K21;

private:
     virtual void getParameters(const ParameterSet* &parameters) = 0;
};

class RK4Linear2CompBolusK : public RK4Linear2CompModel {
    Q_DECLARE_TR_FUNCTIONS(RK4Linear2CompBolusK)
    MODELUTILITIES(RK4Linear2CompBolusK)
public:
    RK4Linear2CompBolusK();

    ModelDescriptor descriptor ();

protected:
    void yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result);
    QVector<double> y0 ();

private:
     virtual void getParameters(const ParameterSet* &parameters);
};

class RK4Linear2CompBolusClV : public RK4Linear2CompBolusK {
    Q_DECLARE_TR_FUNCTIONS(RK4Linear2CompBolusClV)
    MODELUTILITIES(RK4Linear2CompBolusClV)
public:
    RK4Linear2CompBolusClV();
    ModelDescriptor descriptor ();

private:
    virtual void getParameters(const ParameterSet* &parameters);
};

class RK4Linear2CompiInfK  : public RK4Linear2CompModel {

    Q_DECLARE_TR_FUNCTIONS(RK4Linear2CompiInfK)
    MODELUTILITIES(RK4Linear2CompiInfK)

public:

    RK4Linear2CompiInfK();
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

private:
    virtual void getParameters(const ParameterSet* &parameters);
};

class RK4Linear2CompiInfClV  : public RK4Linear2CompiInfK {

    Q_DECLARE_TR_FUNCTIONS(RK4Linear2CompiInfClV)
    MODELUTILITIES(RK4Linear2CompiInfClV)

public:

    RK4Linear2CompiInfClV();

    ModelDescriptor descriptor ();

private:
    virtual void getParameters(const ParameterSet* &parameters);
};

class RK4Linear2CompExtraK : public RK4Linear2CompModel {
    Q_DECLARE_TR_FUNCTIONS(RK4Linear2CompExtraK)
    MODELUTILITIES(RK4Linear2CompExtraK)

public:
    RK4Linear2CompExtraK();

    ModelDescriptor descriptor ();

protected:
    void yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result);
    QVector<double> y0 ();
    double F, Ka;

private:
    virtual void getParameters(const ParameterSet* &parameters);
};

class RK4Linear2CompExtraClV : public RK4Linear2CompExtraK {
    Q_DECLARE_TR_FUNCTIONS(RK4Linear2CompExtraClV)
    MODELUTILITIES(RK4Linear2CompExtraClV)

public:
    RK4Linear2CompExtraClV();
    ModelDescriptor descriptor ();

private:
    virtual void getParameters(const ParameterSet* &parameters);
};


#endif // RK4LINEAR2MODEL_H
