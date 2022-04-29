//@@license@@

#ifndef RK4MM2MODEL_H
#define RK4MM2MODEL_H

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

class RK4MichaelisMenten2CompModel : public RK4GenericModel
{
    Q_DECLARE_TR_FUNCTIONS(RK4MichaelisMenten2CompModel)    
public:
    RK4MichaelisMenten2CompModel();   

    Version version ();

    Duration halflife ( const Duration &start,
                        const ParameterSet* &parameters,
                        const SharedDosage &dosage,
                        const Residuals &residuals);

protected:    
    //Used units
    Unit _ug,_ugl;
    //Some variable put there to not have to pass them as a function parameter (sic)
    double V1,V2,Q,Km,Vm;
    double K12,K21;
private:
    virtual void getParameters(const ParameterSet* &parameters) = 0;
};

class RK4MichaelisMenten2CompBolusK : public RK4MichaelisMenten2CompModel {
    Q_DECLARE_TR_FUNCTIONS(RK4MichaelisMenten2CompBolusK)
    MODELUTILITIES(RK4MichaelisMenten2CompBolusK)
public:
    RK4MichaelisMenten2CompBolusK();

    ModelDescriptor descriptor ();

protected:
    void yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result);
    QVector<double> y0 ();

private:
    virtual void getParameters(const ParameterSet* &parameters);
};

class RK4MichaelisMenten2CompBolusClV : public RK4MichaelisMenten2CompBolusK {
    Q_DECLARE_TR_FUNCTIONS(RK4MichaelisMenten2CompBolusClV)
    MODELUTILITIES(RK4MichaelisMenten2CompBolusClV)
public:
    RK4MichaelisMenten2CompBolusClV();

    ModelDescriptor descriptor ();

private:
    virtual void getParameters(const ParameterSet* &parameters);
};

class RK4MichaelisMenten2CompExtraK : public RK4MichaelisMenten2CompModel {
    Q_DECLARE_TR_FUNCTIONS(RK4MichaelisMenten2CompExtraK)
    MODELUTILITIES(RK4MichaelisMenten2CompExtraK)
public:
    RK4MichaelisMenten2CompExtraK();

    ModelDescriptor descriptor ();

protected:
    void yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result);
    QVector<double> y0 ();
    double F,Ka;

private:
    virtual void getParameters(const ParameterSet* &parameters);    
};

class RK4MichaelisMenten2CompExtraClV : public RK4MichaelisMenten2CompExtraK {
    Q_DECLARE_TR_FUNCTIONS(RK4MichaelisMenten2CompExtraClV)
    MODELUTILITIES(RK4MichaelisMenten2CompExtraClV)
public:
    RK4MichaelisMenten2CompExtraClV();

    ModelDescriptor descriptor ();
private:
    virtual void getParameters(const ParameterSet* &parameters);
};

class RK4MichaelisMenten2CompInfK  : public RK4MichaelisMenten2CompModel {
    Q_DECLARE_TR_FUNCTIONS(RK4MichaelisMenten2CompInfK)
    MODELUTILITIES(RK4MichaelisMenten2CompInfK)
public:
    RK4MichaelisMenten2CompInfK();

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
    QVector<double> y0 ();
private:
    virtual void getParameters(const ParameterSet* &parameters);
};

class RK4MichaelisMenten2CompInfClV  : public RK4MichaelisMenten2CompInfK {
    Q_DECLARE_TR_FUNCTIONS(RK4MichaelisMenten2CompInfClV)
    MODELUTILITIES(RK4MichaelisMenten2CompInfClV)
public:
    RK4MichaelisMenten2CompInfClV();

    ModelDescriptor descriptor ();

private:
    virtual void getParameters(const ParameterSet* &parameters);
};

#endif // RK4MM2MODEL_H
