#include "analyticlinear1compextra.h"

#include "core_errors.h"
#include "coremessagehandler.h"
#include "stdparamnames.h"
#include "knownmodels.h"
#include "version.h"

#include <QStringList>

#include <math.h>


AnalyticLinear1CompExtraK::AnalyticLinear1CompExtraK()
{
    _priority = 10;
    //We want mg for the dose
    _ug.fromString("ug");
    //And ug/l for the result
    _ugl.fromString("ug/l");

    // Parameters
    _parameters.clear();
    _parameters.append(Descriptor("Ke",StdParamNames::getNameKe(),StdParamNames::getDescKe()));
    _parameters.append(Descriptor("Ka",StdParamNames::getNameKA(),StdParamNames::getDescKA()));
    _parameters.append(Descriptor("F",StdParamNames::getNameF(),StdParamNames::getDescF()));
}

AnalyticLinear1CompExtraClV::AnalyticLinear1CompExtraClV()
{
    _priority = 10;
    //We want mg for the dose
    _ug.fromString("ug");
    //And ug/l for the result
    _ugl.fromString("ug/l");

    // Parameters
    _parameters.clear();
    _parameters.append(Descriptor("CL",StdParamNames::getNameCL(),StdParamNames::getDescCL()));
    _parameters.append(Descriptor("V",StdParamNames::getNameV(),StdParamNames::getDescV()));
    _parameters.append(Descriptor("Ka",StdParamNames::getNameKA(),StdParamNames::getDescKA()));
    _parameters.append(Descriptor("F",StdParamNames::getNameF(),StdParamNames::getDescF()));
}

//Create it here
ModelDescriptor AnalyticLinear1CompExtraK::descriptor()
{
    return ModelDescriptor ( A1C_EXTRA_K_ID,
                             MODEL_LINEAR_1COMP_EXTRA_K_ID,
                             tr("Linear One-compartment Extravascular (Analytic)"),
                             tr("Model using an analytic approach from the CHUV."),
                             "extra" );
}

//Create it here
ModelDescriptor AnalyticLinear1CompExtraClV::descriptor()
{
    return ModelDescriptor ( A1C_EXTRA_CLV_ID,
                             MODEL_LINEAR_1COMP_EXTRA_CLV_ID,
                             tr("Linear One-compartment Extravascular (Analytic)"),
                             tr("Model using an analytic approach from the CHUV."),
                             "extra" );
}

//Yeah!
Version AnalyticLinear1CompExtraK::version()
{
    return Version("0.1");
}

Version AnalyticLinear1CompExtraClV::version()
{
    return Version("0.1");
}

//Static (that's what the CHUV said)
Duration AnalyticLinear1CompExtraK::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
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
Duration AnalyticLinear1CompExtraClV::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
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

bool AnalyticLinear1CompExtraK::fetchParameters(const ParameterSet* &parameters, double &V,double &Ke, double &Ka, double &F) {

    //Fetch the parameters
    Ke = parameters->get("Ke")->value();
    V = parameters->get("V")->value();
    Ka = parameters->get("Ka")->value();
    F = parameters->get("F")->value();

    //Check for division by 0
    Q_ASSERT(V != 0.0);

    return true;
}

DataSet AnalyticLinear1CompExtraK::steadystateImpl(const Duration &start, const Duration &duration, const ParameterSet* &parameters, const SharedDosage &dosage, const Duration &halflife, int multiplier, uint points, Residuals &residual, bool uniform, bool computeResidual)
{
    Q_UNUSED(uniform);
    Q_UNUSED(halflife);
    Q_UNUSED(multiplier);

    //The result
    DataSet data(points, _ugl);

    //Get the parameters
    double V, Ke, F, Ka;

    fetchParameters(parameters, V, Ke, Ka, F);

    //Check for division by 0
    Q_ASSERT(V != 0.0);

    //Set the dose
    double D = dosage->dose().valueIn(_ug);

    //Set the start time
    double T = start.toHours();

    //Compute the delta
    double delta = 0.0;
    if (points > 1)
        delta = duration.toHours() / (points - 1);
    else
        T += duration.toHours();

    //Check for division by 0
    Q_ASSERT(Ka != Ke);

    //Compute the invariants
    double inv = (F*D/V) * (Ka/(Ka - Ke));
    double keI = 1 - exp(-Ke * dosage->interval().toHours());
    double kaI = 1 - exp(-Ka * dosage->interval().toHours());
    double eht;
    double aht;

    //Compute each point
    for (uint i=0; i < points; i++) {
        eht = exp(-Ke * T);
        aht = exp(-Ka * T);
        data[i] = inv * ((eht / keI) - (aht / kaI));
        T += delta;
    }

    //Recalibrate start time
    T -= delta;

    //Compute the residual
    if (computeResidual) {
        residual[0] = Residual(data[points-1]);
        residual[1] = Residual(F*D/V*exp(-Ka*T)/(1-exp(-Ka*dosage->interval().toHours())));
        //residual[1] = Residual((residuals.at(1).value+F*D/V)*exp(-Ka*T));
    }

    return data;
}

bool AnalyticLinear1CompExtraClV::fetchParameters(const ParameterSet* &parameters, double &V,double &Ke, double &Ka, double &F) {

    //Fetch the parameters
    double CL = parameters->get("CL")->value();
    V = parameters->get("V")->value();
    Ka = parameters->get("Ka")->value();
    F = parameters->get("F")->value();

    //Check for division by 0
    Q_ASSERT(V != 0.0);

    Ke = CL/V;
    return true;
}

//Simple formula application
DataSet AnalyticLinear1CompExtraK::predict(const Duration &start, const Duration &until, const ParameterSet* &parameters, uint points, const SharedDosage &dosage, Residuals &residuals, bool uniform)
{
    Q_UNUSED(uniform);

    //parameters
    double V, Ke, F, Ka;

    fetchParameters(parameters, V, Ke, Ka, F);

    //Check for division by 0
    Q_ASSERT(V != 0.0);

    //Fetch the dosage
    double D = dosage->dose().valueIn(_ug);

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
    double resid1 = residuals.at(0).value;
    double resid2 = residuals.at(1).value + F*D/V;

    //Check for division by 0
    Q_ASSERT(Ka != Ke);

    double part2 = Ka*resid2/(-Ka+Ke);
    double eht;

    //Calculate each point
    for (uint i=0; i < points; i++) {
        eht = exp(-Ke*T);
        data[i] = eht*resid1 + (-eht+exp(-Ka*T))*part2;
        T += delta;
    }

    //Recalibrate start time
    T -= delta;

    //Set the new residual
    residuals[0] = Residual(data[points-1]);
    residuals[1] = Residual((residuals.at(1).value+F*D/V)*exp(-Ka*T));

    return data;
}
