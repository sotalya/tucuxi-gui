#include "rk4linear1comp.h"

#include "core.h"
#include "core_errors.h"
#include "units.h"
#include "coremessagehandler.h"
#include "stdparamnames.h"
#include "knownmodels.h"
#include "version.h"

#include <math.h>

RK4Linear1CompModel::RK4Linear1CompModel() : RK4GenericModel(1)
{
    //We want ug for the dose
    _ug.fromString("ug");
    //And ug/l for the result
    _ugl.fromString("ug/l");    

}

Version RK4Linear1CompModel::version()
{
    return Version("0.1");
}


//Copied on the analytic model
Duration RK4Linear1CompModel::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
{
    //Fetch the parameters
    double CL = parameters->get("CL")->value();
    double V = parameters->get("V")->value();

    //Check for division by 0
    Q_ASSERT(CL != 0.0);
    Q_ASSERT(V  != 0.0);

    //Log(2) static value
#define LOG2H 0.693147180559945*3600000.0
    // t½ = ln(2) / (CL/V)
    return Duration(0,0,0,LOG2H / (CL/V));
#undef LOG2H
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// Bolus, mono-compartimental model
//////////////////////////////////////////////////////////////////////////////////////////////////
RK4Linear1CompBolusK::RK4Linear1CompBolusK()
{
    _parameters.clear();
    _parameters.append(Descriptor("Ke",StdParamNames::getNameKe(),StdParamNames::getDescKe()));
    _parameters.append(Descriptor("V",StdParamNames::getNameV(),StdParamNames::getDescV()));
}

RK4Linear1CompBolusClV::RK4Linear1CompBolusClV ()
{
    _parameters.clear();
    _parameters.append(Descriptor("CL",StdParamNames::getNameCL(),StdParamNames::getDescCL()));
    _parameters.append(Descriptor("V",StdParamNames::getNameV(),StdParamNames::getDescV()));
}

QVector<double> RK4Linear1CompBolusK::y0 ()
{
    //Check for division by 0
    Q_ASSERT(V != 0.0);

    QVector<double> initDosage(1);
    initDosage[0] = D/V;
    return initDosage;
}

ModelDescriptor RK4Linear1CompBolusK::descriptor()
{
    return ModelDescriptor(RK4_LIN_1C_BOLUS_K_ID,
                           MODEL_LINEAR_1COMP_BOLUS_K_ID,
                           tr("Linear One-compartment Bolus (RK4)"),
                           tr("Model using the classic Runge-Kutta method, 4th order."),
                           "bolus");
}

ModelDescriptor RK4Linear1CompBolusClV::descriptor()
{
    return ModelDescriptor(RK4_LIN_1C_BOLUS_CLV_ID,
                           MODEL_LINEAR_1COMP_BOLUS_CLV_ID,
                           tr("Linear One-compartment Bolus (RK4)"),
                           tr("Model using the classic Runge-Kutta method, 4th order."),
                           "bolus");
}

void RK4Linear1CompBolusK::getParameters(const ParameterSet* &parameters)
{
    GETPARAMETER(Ke,"Ke");
    GETPARAMETER(V,"V");

    //Check for division by 0
    Q_ASSERT(V != 0.0);
}

void RK4Linear1CompBolusClV::getParameters(const ParameterSet* &parameters)
{
    GETPARAMETER(CL,"CL");
    GETPARAMETER(V,"V");

    //Check for division by 0
    Q_ASSERT(V != 0.0);

    Ke = CL/V;
}

void RK4Linear1CompBolusK::yDeriv (double, const QVector<double> &yn, const SharedDosage &, QVector<double> &result)
{
    result[0] = -Ke*yn.at(0);
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// Infusion, mono-compartimental model
//////////////////////////////////////////////////////////////////////////////////////////////////
RK4Linear1CompInfK::RK4Linear1CompInfK()
{
    _parameters.clear();
    _parameters.append(Descriptor("Ke",StdParamNames::getNameKe(),StdParamNames::getDescKe()));
    _parameters.append(Descriptor("V",StdParamNames::getNameV(),StdParamNames::getDescV()));
}

RK4Linear1CompInfClV::RK4Linear1CompInfClV()
{
    _parameters.clear();
    _parameters.append(Descriptor("CL",StdParamNames::getNameCL(),StdParamNames::getDescCL()));
    _parameters.append(Descriptor("V",StdParamNames::getNameV(),StdParamNames::getDescV()));
}

void RK4Linear1CompInfK::yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result)
{
    double tinf = dosage->tinf().toHours();

    //Check for division by 0
    Q_ASSERT(tinf != 0.0);
    Q_ASSERT(V    != 0.0);

    double K0 = D/tinf;

    if (tn <= tinf)
        result[0] = K0/V - (Ke * yn.at(0));
    else
        result[0] = -Ke*yn.at(0);
}

QVector<double> RK4Linear1CompInfK::y0 ()
{
    QVector<double> initDosage(1);
    initDosage[0] = 0.0;
    return initDosage;
}

ModelDescriptor RK4Linear1CompInfK::descriptor()
{
    return ModelDescriptor(RK4_LIN_1C_INFUS_K_ID,
                           MODEL_LINEAR_1COMP_INFUS_K_ID,
                           tr("Linear One-compartment Infusion (RK4)"),
                           tr("Model using the classic Runge-Kutta method, 4th order."),
                           "infu");
}

ModelDescriptor RK4Linear1CompInfClV::descriptor()
{
    return ModelDescriptor(RK4_LIN_1C_INFUS_CLV_ID,
                           MODEL_LINEAR_1COMP_INFUS_CLV_ID,
                           tr("Linear One-compartment Infusion (RK4)"),
                           tr("Model using the classic Runge-Kutta method, 4th order."),
                           "infu");
}

void RK4Linear1CompInfK::getParameters(const ParameterSet* &parameters)
{
    GETPARAMETER(Ke,"Ke");
    GETPARAMETER(V,"V");

    //Check for division by 0
    Q_ASSERT(V != 0.0);
}

void RK4Linear1CompInfClV::getParameters(const ParameterSet* &parameters)
{
    GETPARAMETER(CL,"CL");
    GETPARAMETER(V,"V");

    //Check for division by 0
    Q_ASSERT(V != 0.0);

    Ke = CL/V;
}

DataSet RK4Linear1CompInfK::predict(const Duration &start, const Duration &until, const ParameterSet* &parameters, uint points, const SharedDosage &dosage, Residuals &residuals, bool uniform)
{
    //Convert the start time (T), the infusion end time (Tinf) and the prediction duration (D)
    double Tinf = dosage->tinf().toHours();
    double T = start.toHours();
    double D = until.toHours();

    //Check for division by 0
    Q_ASSERT(D != 0.0);

    //If the infusion end time (Tinf) doesn't occure during the prediction, simply compute
    if (uniform || points < 3 || Tinf <= T || Tinf >= T + D)
        return RK4Linear1CompModel::predict(start, until, parameters, points, dosage, residuals, uniform);

    //Compute the number of points prio and post infusion
    uint prioPoints = ((Tinf - T) * (points - 1) / D) + 1;
    uint postPoints = points - prioPoints;

    //Prepare the prio and post infusion datasets
    DataSet prioInfu(prioPoints, _ugl);
    DataSet postInfu(postPoints, _ugl);

    //Use temporary residuals for the prio infu prediction
    Residuals temp = residuals;

    //Compute the prio and post infusion predictions
    prioInfu = RK4Linear1CompModel::predict(start, dosage->tinf() - start, parameters, prioPoints, dosage, temp, uniform);
    postInfu = RK4Linear1CompModel::predict(dosage->tinf(), until - dosage->tinf() + start, parameters, postPoints, dosage, residuals, uniform);

    //Concatenate the two and return the complete prediction
    return prioInfu + postInfu;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Extravascular administrations, mono-compartimental model, but with 2 compartiments
//////////////////////////////////////////////////////////////////////////////////////////////////
RK4Linear1CompExtraK::RK4Linear1CompExtraK()
{
    setNbComp(2);
    _parameters.clear();
    _parameters.append(Descriptor("Ke",StdParamNames::getNameKe(),StdParamNames::getDescKe()));
    _parameters.append(Descriptor("V",StdParamNames::getNameV(),StdParamNames::getDescV()));
    _parameters.append(Descriptor("Ka",StdParamNames::getNameKA(),StdParamNames::getDescKA()));
    _parameters.append(Descriptor("F",StdParamNames::getNameF(),StdParamNames::getDescF()));
}

RK4Linear1CompExtraClV::RK4Linear1CompExtraClV()
{
    setNbComp(2);
    _parameters.clear();
    _parameters.append(Descriptor("CL",StdParamNames::getNameCL(),StdParamNames::getDescCL()));
    _parameters.append(Descriptor("V",StdParamNames::getNameV(),StdParamNames::getDescV()));
    _parameters.append(Descriptor("Ka",StdParamNames::getNameKA(),StdParamNames::getDescKA()));
    _parameters.append(Descriptor("F",StdParamNames::getNameF(),StdParamNames::getDescF()));
}

void RK4Linear1CompExtraK::yDeriv (double, const QVector<double> &yn, const SharedDosage &, QVector<double> &result)
{
    result[0] = Ka*yn.at(1)-Ke*yn.at(0);
    result[1] = -Ka*yn.at(1);
}

QVector<double> RK4Linear1CompExtraK::y0 ()
{
    //Check for division by 0
    Q_ASSERT(V != 0.0);

    QVector<double> initDosage(2);
    initDosage[0] = 0.0;
    initDosage[1] = F*D/V;
    return initDosage;
}

ModelDescriptor RK4Linear1CompExtraK::descriptor()
{
    return ModelDescriptor(RK4_LIN_1C_EXTRA_K_ID,
                           MODEL_LINEAR_1COMP_EXTRA_K_ID,
                           tr("Linear One-compartment Extravascular (RK4)"),
                           tr("Model using the classic Runge-Kutta method, 4th order. However with 2 compartments instead of 1."),
                           "extra");
}

ModelDescriptor RK4Linear1CompExtraClV::descriptor()
{
    return ModelDescriptor(RK4_LIN_1C_EXTRA_CLV_ID,
                           MODEL_LINEAR_1COMP_EXTRA_CLV_ID,
                           tr("Linear One-compartment Extravascular (RK4)"),
                           tr("Model using the classic Runge-Kutta method, 4th order. However with 2 compartments instead of 1."),
                           "extra");
}

void RK4Linear1CompExtraK::getParameters(const ParameterSet* &parameters)
{
    GETPARAMETER(Ke,"Ke");
    GETPARAMETER(V,"V");

    //Check for division by 0
    Q_ASSERT(V != 0.0);

    GETPARAMETER(F,"F");
    GETPARAMETER(Ka,"Ka");
}

void RK4Linear1CompExtraClV::getParameters(const ParameterSet* &parameters)
{
    GETPARAMETER(CL,"CL");
    GETPARAMETER(V,"V");

    //Check for division by 0
    Q_ASSERT(V != 0.0);

    Ke = CL/V;

    GETPARAMETER(F,"F");
    GETPARAMETER(Ka,"Ka");
}
