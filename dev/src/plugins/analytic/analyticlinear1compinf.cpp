//@@license@@

#include "analyticlinear1compinf.h"

#include "core_errors.h"
#include "coremessagehandler.h"
#include "stdparamnames.h"
#include "knownmodels.h"
#include "version.h"

#include <QStringList>

#include <math.h>


AnalyticLinear1CompInfK::AnalyticLinear1CompInfK()
{
    _priority = 10;
    //Set the parameters
    _parameters.clear();
    _parameters.append(Descriptor("Ke",StdParamNames::getNameKe(),StdParamNames::getDescKe()));
    _parameters.append(Descriptor("V",StdParamNames::getNameV(),StdParamNames::getDescV()));
}

AnalyticLinear1CompInfClV::AnalyticLinear1CompInfClV()
{
    _priority = 10;
    //Set the parameters
    _parameters.clear();
    _parameters.append(Descriptor("CL",StdParamNames::getNameCL(),StdParamNames::getDescCL()));
    _parameters.append(Descriptor("V",StdParamNames::getNameV(),StdParamNames::getDescV()));
}

//Create it here
ModelDescriptor AnalyticLinear1CompInfK::descriptor()
{
    return ModelDescriptor ( A1C_INFUS_K_ID,
                             MODEL_LINEAR_1COMP_INFUS_K_ID,
                             tr("Linear One-compartment Infusion (Analytic)"),
                             tr("Model using an analytic approach from the CHUV."),
                             "infu" );
}

//Create it here
ModelDescriptor AnalyticLinear1CompInfClV::descriptor()
{
    return ModelDescriptor ( A1C_INFUS_CLV_ID,
                             MODEL_LINEAR_1COMP_INFUS_CLV_ID,
                             tr("Linear One-compartment Infusion (Analytic)"),
                             tr("Model using an analytic approach from the CHUV."),
                             "infu" );
}

//Yeah!
Version AnalyticLinear1CompInfK::version()
{
    return Version("0.1");
}

Version AnalyticLinear1CompInfClV::version()
{
    return Version("0.1");
}

//Static (that's what the CHUV said)
Duration AnalyticLinear1CompInfK::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
{
    //Fetch the parameters
    double Ke = parameters->get("Ke")->value();

    //Check for division by 0
    Q_ASSERT(Ke  != 0.0);

    //Log(2) static value
#define LOG2H 0.693147180559945*3600000.0
    // t½ = ln(2) / (CL/V)
    return Duration(0,0,0,LOG2H / Ke);
#undef LOG2H
}

//Static (that's what the CHUV said)
Duration AnalyticLinear1CompInfClV::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
{
    //Fetch the parameters
    double CL = parameters->get("CL")->value();
    double V = parameters->get("V")->value();

    //Check for division by 0
    Q_ASSERT(V  != 0.0);

    //Log(2) static value
#define LOG2H 0.693147180559945*3600000.0
    // t½ = ln(2) / (CL/V)
    return Duration(0,0,0,LOG2H / (CL/V));
#undef LOG2H
}

bool AnalyticLinear1CompInfK::fetchParameters(const ParameterSet* &parameters, double &CL, double &V)
{
    //Fetch the parameters
    double Ke = parameters->get("Ke")->value();
    V = parameters->get("V")->value();

    //Check for division by 0
    Q_ASSERT(V != 0.0);
    CL = Ke*V;
    return true;
}

bool AnalyticLinear1CompInfClV::fetchParameters(const ParameterSet* &parameters, double &CL, double &V)
{
    //Fetch the parameters
    CL = parameters->get("CL")->value();
    V = parameters->get("V")->value();

    //Check for division by 0
    Q_ASSERT(V != 0.0);
    return true;
}

//Compute the prediction for the given interval
DataSet AnalyticLinear1CompInfK::compute(const Duration &start, const Duration &until, const ParameterSet* &parameters, uint points, const SharedDosage &dosage, Residuals &residuals)
{

    double CL,V;

    fetchParameters(parameters, CL, V);
    //Check for division by 0
    Q_ASSERT(V  != 0.0);
    Q_ASSERT(CL != 0.0);
    Q_ASSERT(dosage->tinf().toHours() != 0);

    //Fetch the dosage, which is divided by the interval, giving mg/h
    double Ro = dosage->dose().valueIn(_ug) / dosage->tinf().toHours();
    double Tinf = dosage->tinf().toHours();

    //Starting time
    double T = start.toHours();

    //Delta
    double delta = 0.0;
    if (points > 1)
        delta = until.toHours() / (points - 1);
    else
        T += until.toHours();

    //Result
    DataSet data(points,_ugl);

    //Do that only once
    double H = CL/V;
    double firstPart = Ro/CL;
    double eht;

    //Calculate each point
    for (uint i=0; i < points; i++) {
        eht = exp(-H*T);
        data[i] = firstPart * (exp(H*qMin(T,Tinf)) - 1.0) * eht + residuals.at(0).value * eht;
        T += delta;
    }

    //Recalibrate start time
    T -= delta;

    //Set the new residual
    residuals[0] = Residual(data[points-1]);

    return data;
}

DataSet AnalyticLinear1CompInfK::computeSteadyState(const Duration &start, const Duration &duration, const ParameterSet* &parameters, const SharedDosage &dosage, uint points, Residuals &residual, bool computeResidual)
{
    //The result
    DataSet data(points,_ugl);

    //Get the parameters
    double CL,V;

    fetchParameters(parameters, CL, V);

    //Check for division by 0
    Q_ASSERT(V  != 0.0);
    Q_ASSERT(CL != 0.0);
    Q_ASSERT(dosage->tinf().toHours() != 0);

    //Set the dose parameters
    double D    = dosage->dose().valueIn(_ug);
    double Tinf = dosage->tinf().toHours();
    double Inte = dosage->interval().toHours();

    //Set the start time
    double T = start.toHours();

    //Compute the delta
    double delta = 0.0;
    if (points > 1)
        delta = duration.toHours() / (points - 1);
    else
        T += duration.toHours();

    //Compute the invariants
    double Ke  = CL/V;
    double inv = D / (CL * Tinf);
    double expTinf = exp(-Ke * Tinf);
    double expInte = exp(-Ke * Inte);
    double expTime;
    double expTmTi;
    double common;

    //Compute each point
    for (uint i=0; i < points; i++) {
        expTime = exp(-Ke * T);
        expTmTi = exp(-Ke * (T - Tinf));
        common  = ((1 - expTinf) * expTmTi) / (1 - expInte);
        data[i] = inv * (T > Tinf ? common : (common * expInte) + 1 - expTime);
        T += delta;
    }

    //Recalibrate start time
    T -= delta;

    //Compute the residual
    if (computeResidual)
        residual[0] = Residual(data[points-1]);

    return data;
}
