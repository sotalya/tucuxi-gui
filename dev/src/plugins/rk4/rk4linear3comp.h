//@@license@@

#ifndef RK4LINEAR3MODEL_H
#define RK4LINEAR3MODEL_H

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


class RK4Linear3CompModel : public RK4GenericModel
{
    Q_DECLARE_TR_FUNCTIONS(RK4Linear3CompModel)    
public:
    RK4Linear3CompModel();    

    Version version ();

    Duration halflife ( const Duration &start,
                        const ParameterSet* &parameters,
                        const SharedDosage &dosage,
                        const Residuals &residuals);

protected:
    //Used units
    Unit _ug,_ugl;
    //Some variable put there to not have to pass them as a function parameter (sic)
    double CL,V1,V2,V3,Q2,Q3;

    double K12,K21,K13,K31,Ke;

private:
    virtual void getParameters(const ParameterSet* &parameters) = 0;
};

class RK4Linear3CompBolusK : public RK4Linear3CompModel {

    Q_DECLARE_TR_FUNCTIONS(RK4Linear3CompBolusK)
    MODELUTILITIES(RK4Linear3CompBolusK)
public:

    RK4Linear3CompBolusK();

    ModelDescriptor descriptor ();

protected:
    void yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result);
    QVector<double> y0 ();

private:
    virtual void getParameters(const ParameterSet* &parameters);
};

class RK4Linear3CompBolusClV : public RK4Linear3CompBolusK {

    Q_DECLARE_TR_FUNCTIONS(RK4Linear3CompBolusClV)
    MODELUTILITIES(RK4Linear3CompBolusClV)
public:

    RK4Linear3CompBolusClV();

    ModelDescriptor descriptor ();

private:
    virtual void getParameters(const ParameterSet* &parameters);
};

class RK4Linear3CompInfK  : public RK4Linear3CompModel {
    Q_DECLARE_TR_FUNCTIONS(RK4Linear3CompInfK)
    MODELUTILITIES(RK4Linear3CompInfK)
public:
    RK4Linear3CompInfK();

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

class RK4Linear3CompInfClV  : public RK4Linear3CompInfK {
    Q_DECLARE_TR_FUNCTIONS(RK4Linear3CompInfClV)
    MODELUTILITIES(RK4Linear3CompInfClV)
public:
    RK4Linear3CompInfClV();

    ModelDescriptor descriptor ();

private:
    virtual void getParameters(const ParameterSet* &parameters);
};

class RK4Linear3CompExtraK : public RK4Linear3CompModel {
    Q_DECLARE_TR_FUNCTIONS(RK4Linear3CompExtraK)
    MODELUTILITIES(RK4Linear3CompExtraK)
public:
    RK4Linear3CompExtraK();
    ModelDescriptor descriptor ();    

protected:
    void yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result);
    QVector<double> y0 ();
    double Ka,F;

private:
    virtual void getParameters(const ParameterSet* &parameters);
};

class RK4Linear3CompExtraClV : public RK4Linear3CompExtraK {
    Q_DECLARE_TR_FUNCTIONS(RK4Linear3CompExtraClV)
    MODELUTILITIES(RK4Linear3CompExtraClV)
public:
    RK4Linear3CompExtraClV();
    ModelDescriptor descriptor ();

private:
    virtual void getParameters(const ParameterSet* &parameters);
};

#endif // RK4LINEAR3MODEL_H
