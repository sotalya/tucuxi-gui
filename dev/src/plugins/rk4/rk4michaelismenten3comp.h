//@@license@@

#ifndef RK4MM3MODEL_H
#define RK4MM3MODEL_H

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

class RK4MichaelisMenten3CompModel : public RK4GenericModel
{
    Q_DECLARE_TR_FUNCTIONS(RK4MichaelisMenten3CompModel)    
public:
    RK4MichaelisMenten3CompModel();    

    Version version ();

    Duration halflife ( const Duration &start,
                        const ParameterSet* &parameters,
                        const SharedDosage &dosage,
                        const Residuals &residuals);

protected:    
    //Used units
    Unit _ug,_ugl;
    //Some variable put there to not have to pass them as a function parameter (sic)
    double V1,V2,V3,Q2,Q3,Km,Vm;
    double K12,K21,K13,K31;
private:
    virtual void getParameters(const ParameterSet* &parameters) = 0;
};

class RK4MichaelisMenten3CompBolusK : public RK4MichaelisMenten3CompModel {
    Q_DECLARE_TR_FUNCTIONS(RK4MichaelisMenten3CompBolusK)
    MODELUTILITIES(RK4MichaelisMenten3CompBolusK)
public:
    RK4MichaelisMenten3CompBolusK();
    ModelDescriptor descriptor ();
protected:
    void yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result);
    QVector<double> y0 ();
private:
    virtual void getParameters(const ParameterSet* &parameters);
};

class RK4MichaelisMenten3CompBolusClV : public RK4MichaelisMenten3CompBolusK {
    Q_DECLARE_TR_FUNCTIONS(RK4MichaelisMenten3CompBolusClV)
    MODELUTILITIES(RK4MichaelisMenten3CompBolusClV)
public:
    RK4MichaelisMenten3CompBolusClV();
    ModelDescriptor descriptor ();
private:
    virtual void getParameters(const ParameterSet* &parameters);

};

class RK4MichaelisMenten3CompExtraK : public RK4MichaelisMenten3CompModel {
    Q_DECLARE_TR_FUNCTIONS(RK4MichaelisMenten3CompExtraK)
    MODELUTILITIES(RK4MichaelisMenten3CompExtraK)
public:
    RK4MichaelisMenten3CompExtraK();
    ModelDescriptor descriptor ();
protected:
    void yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result);
    QVector<double> y0 ();
    double F,Ka;
private:
    virtual void getParameters(const ParameterSet* &parameters);
};

class RK4MichaelisMenten3CompExtraClV : public RK4MichaelisMenten3CompExtraK {
    Q_DECLARE_TR_FUNCTIONS(RK4MichaelisMenten3CompExtraClV)
    MODELUTILITIES(RK4MichaelisMenten3CompExtraClV)
public:
    RK4MichaelisMenten3CompExtraClV();
    ModelDescriptor descriptor ();
private:
    virtual void getParameters(const ParameterSet* &parameters);
};

class RK4MichaelisMenten3CompInfK : public RK4MichaelisMenten3CompModel {
    Q_DECLARE_TR_FUNCTIONS(RK4MichaelisMenten3CompInfK)
    MODELUTILITIES(RK4MichaelisMenten3CompInfK)
public:
    RK4MichaelisMenten3CompInfK();
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

class RK4MichaelisMenten3CompInfClV : public RK4MichaelisMenten3CompInfK {
    Q_DECLARE_TR_FUNCTIONS(RK4MichaelisMenten3CompInfClV)
    MODELUTILITIES(RK4MichaelisMenten3CompInfClV)
public:
    RK4MichaelisMenten3CompInfClV();
    ModelDescriptor descriptor ();
private:
    virtual void getParameters(const ParameterSet* &parameters);
};

#endif // RK4MM3MODEL_H
