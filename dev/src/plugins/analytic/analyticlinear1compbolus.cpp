//@@license@@

#include "analyticlinear1compbolus.h"

#include "core_errors.h"
#include "coremessagehandler.h"
#include "core.h"
#include "stdparamnames.h"
#include "knownmodels.h"
#include "version.h"

#include <math.h>


AnalyticLinear1CompBolusK::AnalyticLinear1CompBolusK()
{
    _priority = 10;
    //We want ug for the dose
    _ug.fromString("ug");
    //And ug/l for the result
    _ugl.fromString("ug/l");

    // Parameters
    _parameters.clear();
    _parameters.append(Descriptor("Ke",StdParamNames::getNameKe(),StdParamNames::getDescKe()));            
    _parameters.append(Descriptor("V",StdParamNames::getNameV(),StdParamNames::getDescV()));
}

AnalyticLinear1CompBolusClV::AnalyticLinear1CompBolusClV()
{
    _priority = 10;
    //We want ug for the dose
    _ug.fromString("ug");
    //And ug/l for the result
    _ugl.fromString("ug/l");

    // Parameters
    _parameters.clear();
    _parameters.append(Descriptor("CL",StdParamNames::getNameCL(),StdParamNames::getDescCL()));
    _parameters.append(Descriptor("V",StdParamNames::getNameV(),StdParamNames::getDescV()));
}

//Create it here
ModelDescriptor AnalyticLinear1CompBolusK::descriptor()
{
    return ModelDescriptor ( A1C_BOLUS_K_ID,
                             MODEL_LINEAR_1COMP_BOLUS_K_ID,
                             tr("Linear One-compartment Bolus (Analytic)"),
                             tr("Model using an analytic approach from the CHUV."),
                             "bolus" );
}

//Create it here
ModelDescriptor AnalyticLinear1CompBolusClV::descriptor()
{
    return ModelDescriptor ( A1C_BOLUS_CLV_ID,
                             MODEL_LINEAR_1COMP_BOLUS_CLV_ID,
                             tr("Linear One-compartment Bolus (Analytic)"),
                             tr("Model using an analytic approach from the CHUV."),
                             "bolus" );
}

//Yeah!
Version AnalyticLinear1CompBolusK::version()
{
    return Version("0.1");
}

Version AnalyticLinear1CompBolusClV::version()
{
    return Version("0.1");
}

//Static (that's what the CHUV said)
Duration AnalyticLinear1CompBolusK::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
{
    //Fetch the parameters
    double Ke = parameters->get("Ke")->value();

    //Check for division by 0
    Q_ASSERT(Ke != 0.0);

    //Log(2) static value
#define LOG2H 0.693147180559945*3600000.0
    // t½ = ln(2) / (CL/V)    

    return Duration(0,0,0,LOG2H / (Ke));
#undef LOG2H    
}

//Static (that's what the CHUV said)
Duration AnalyticLinear1CompBolusClV::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
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

//Simple formula application
DataSet AnalyticLinear1CompBolusK::predict(const Duration &start, const Duration &until, const ParameterSet* &parameters, uint points, const SharedDosage &dosage, Residuals &residuals, bool uniform)
{
    Q_UNUSED(uniform);

    //Parameters
    double Ke;
    double V;

    fetchParameters(parameters, V, Ke);

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
    double firstPart = D/V;
    double eht;

    //Calculate each point
    for (uint i=0; i < points; i++) {
        eht = exp(-Ke*T);
        data[i] = firstPart * eht + residuals.at(0).value * eht;
        T += delta;
    }

    //Recalibrate start time
    T -= delta;

    //Set the new residual
    residuals[0] = Residual(data[points-1]);

    return data;
}

bool AnalyticLinear1CompBolusK::fetchParameters(const ParameterSet* &parameters, double &V, double &Ke)
{
    //Fetch the parameters
    Ke = parameters->get("Ke")->value();
    V = parameters->get("V")->value();

    //Check for division by 0
    Q_ASSERT(V != 0.0);
    return true;
}

DataSet AnalyticLinear1CompBolusK::steadystateImpl(const Duration &start, const Duration &duration, const ParameterSet* &parameters, const SharedDosage &dosage, const Duration &halflife, int multiplier, uint points, Residuals &residual, bool uniform, bool computeResidual)
{
    Q_UNUSED(uniform);
    Q_UNUSED(halflife);
    Q_UNUSED(multiplier);

    //The result
    DataSet data(points, _ugl);

    //Get the parameters
    double Ke;
    double V;

    fetchParameters(parameters, V, Ke);

    //Check for division by 0
    Q_ASSERT(V != 0.0);

    //Get the dose
    double D = dosage->dose().valueIn(_ug);

    //Set the start time
    double T = start.toHours();

    //Compute the delta
    double delta = 0.0;
    if (points > 1)
        delta = duration.toHours() / (points - 1);
    else
        T += duration.toHours();

    //Compute the invarants
    double inv = D/V;
    double keI = 1 - exp(-Ke * dosage->interval().toHours());
    double eht;

    //Compute each point
    for (uint i=0; i < points; i++) {
        eht = exp(-Ke * T);
        data[i] = inv * (eht / keI);
        T += delta;
    }

    //Recalibrate start time
    T -= delta;

    //Compute the residual
    if (computeResidual)
        residual[0] = Residual(data[points-1]);

    return data;
}

bool AnalyticLinear1CompBolusClV::fetchParameters(const ParameterSet* &parameters, double &V, double &Ke)
{
    //Fetch the parameters
    double CL = parameters->get("CL")->value();
    V = parameters->get("V")->value();

    //Check for division by 0
    Q_ASSERT(V != 0.0);

    Ke = CL/V;
    return true;
}
