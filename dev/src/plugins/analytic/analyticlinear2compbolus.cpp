//@@license@@

#include "analyticlinear2compbolus.h"

#include "core_errors.h"
#include "coremessagehandler.h"
#include "core.h"
#include "stdparamnames.h"
#include "knownmodels.h"
#include "version.h"

#include <math.h>


AnalyticLinear2CompBolusK::AnalyticLinear2CompBolusK()
{
    _priority = 10;
    //We want ug for the dose
    _ug.fromString("ug");
    //And ug/l for the result
    _ugl.fromString("ug/l");

    // Parameters
    _parameters.clear();
    _parameters.append(Descriptor("Ke",StdParamNames::getNameKe(),StdParamNames::getDescKe()));
    _parameters.append(Descriptor("V1",StdParamNames::getNameV1(),StdParamNames::getDescV1()));
    _parameters.append(Descriptor("K12",StdParamNames::getNameK12(),StdParamNames::getDescK12()));
    _parameters.append(Descriptor("K21",StdParamNames::getNameK21(),StdParamNames::getDescK21()));
}

AnalyticLinear2CompBolusClV::AnalyticLinear2CompBolusClV()
{
    _priority = 10;
    //We want ug for the dose
    _ug.fromString("ug");
    //And ug/l for the result
    _ugl.fromString("ug/l");

    // Parameters
    _parameters.clear();
    _parameters.append(Descriptor("CL",StdParamNames::getNameCL(),StdParamNames::getDescCL()));
    _parameters.append(Descriptor("V1",StdParamNames::getNameV1(),StdParamNames::getDescV1()));
    _parameters.append(Descriptor("V2",StdParamNames::getNameV2(),StdParamNames::getDescV2()));
    _parameters.append(Descriptor("Q",StdParamNames::getNameQ(),StdParamNames::getDescQ()));
}

//Create it here
ModelDescriptor AnalyticLinear2CompBolusK::descriptor()
{
    return ModelDescriptor ( A2C_BOLUS_K_ID,
                             MODEL_LINEAR_2COMP_BOLUS_K_ID,
                             tr("Linear Two-compartment Bolus (Analytic)"),
                             tr("Model using an analytic approach from the CHUV."),
                             "bolus" );
}

ModelDescriptor AnalyticLinear2CompBolusClV::descriptor()
{
    return ModelDescriptor ( A2C_BOLUS_CLV_ID,
                             MODEL_LINEAR_2COMP_BOLUS_CLV_ID,
                             tr("Linear Two-compartment Bolus (Analytic)"),
                             tr("Model using an analytic approach from the CHUV."),
                             "bolus" );
}

//Yeah!
Version AnalyticLinear2CompBolusK::version()
{
    return Version("0.1");
}

Version AnalyticLinear2CompBolusClV::version()
{
    return Version("0.1");
}

//Static (that's what the CHUV said)
Duration AnalyticLinear2CompBolusK::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
{
    //Fetch the parameters
    double Ke = parameters->get("Ke")->value();

    //Check for division by 0
    Q_ASSERT(Ke != 0.0);

    //Log(2) static value
#define LOG2H 0.693147180559945*3600000.0
    // t½ = ln(2) / (CL/V)
    return Duration(0,0,0,LOG2H / Ke);
#undef LOG2H
}

Duration AnalyticLinear2CompBolusClV::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
{
    //Fetch the parameters
    double CL = parameters->get("CL")->value();
    double V1 = 20;//parameters->get("V1")->value();

    //Check for division by 0
    Q_ASSERT(CL != 0.0);
    Q_ASSERT(V1 != 0.0);

    //Log(2) static value
#define LOG2H 0.693147180559945*3600000.0
    // t½ = ln(2) / (CL/V)
    return Duration(0,0,0,LOG2H / (CL/V1));
#undef LOG2H
}

bool AnalyticLinear2CompBolusK::fetchParameters(const ParameterSet* &parameters, double &CL,double &V1,double &V2,double &Q) {
    V1 = parameters->get("V1")->value();
    double K12 = parameters->get("K12")->value();
    double K21 = parameters->get("K21")->value();
    double Ke = parameters->get("Ke")->value();
    Q = K12*V1;
    CL = Ke*V1;
    V2 = K21/Q;
    return true;
}

DataSet AnalyticLinear2CompBolusK::steadystateImpl(const Duration &start, const Duration &duration, const ParameterSet* &parameters, const SharedDosage &dosage, const Duration &halflife, int multiplier, uint points, Residuals &residual, bool uniform, bool computeResidual)
{
    Q_UNUSED(uniform);
    Q_UNUSED(halflife);
    Q_UNUSED(multiplier);

    //The result
    DataSet data(points, _ugl);

    //Get the dose
    double D = dosage->dose().valueIn(_ug);

    //Get the interval
    double I = dosage->interval().toHours();

    //Get the start time
    double T = start.toHours();

    //Get the delta
    double delta = 0.0;
    if (points > 1)
        delta = duration.toHours() / (points - 1);
    else
        T += duration.toHours();

    //Get the parameters
    double CL, V1, V2, Q;

    fetchParameters(parameters, CL, V1, V2, Q);

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);
    Q_ASSERT(V2 != 0.0);

    //Compute the parameters
    double k12 = Q/V1;
    double k21 = Q/V2;
    double ke   = CL/V1;

    //Compute the invariants
    double sumK = k12 + k21 + ke;
    double root = sqrt(sumK*sumK - 4*k21*ke);

    //Check for invalid operations
    Q_ASSERT(!isnan(root));
    Q_ASSERT(!isinf(root));
    Q_ASSERT(root != 0.0);

    //Compute the invariants
    double Beta  = (sumK - root)/2;

    //Check for division by 0
    Q_ASSERT(Beta != 0.0);

    //Compute the invariants
    double Alpha = k21*ke / Beta;


    //Check for division by 0
    Q_ASSERT(Alpha != Beta);
    Q_ASSERT(k21 != Beta);
    Q_ASSERT(k21 != Alpha);

    //Compute the invariants
    double A = (Alpha - k21) / (V1 * (Alpha - Beta));
    double B = (Beta  - k21) / (V1 * (Beta - Alpha));

    double expAlI = exp(-Alpha*I);
    double expBeI = exp(-Beta*I);

    //Calculate each point
    double expAlT, expBeT;
    double part1, part2;

    for (uint i=0; i < points; i++) {
        expAlT = exp(-Alpha*T);
        expBeT = exp(-Beta*T);

        part1 = (A * expAlT) / (1 - expAlI);
        part2 = (B * expBeT) / (1 - expBeI);

        data[i] = D*(part1 + part2);
        T += delta;
    }

    //Recalibrate start time
    T -= delta;

    double B1 = k12*D/V1/root;
    double A1 = -k12*D/V1/root;

    //Compute the residual
    if (computeResidual) {
        residual[0] = Residual(data[points-1]);
        residual[1] = Residual(B1*exp(-Beta*T)/(1-exp(-Beta*I)) + A1*exp(-Alpha*T)/(1-exp(-Alpha*T)));
    }

    return data;
}

bool AnalyticLinear2CompBolusClV::fetchParameters(const ParameterSet* &parameters, double &CL,double &V1,double &V2,double &Q) {
    V1 = parameters->get("V1")->value();
    CL = parameters->get("CL")->value();
    V2 = parameters->get("V2")->value();
    Q = parameters->get("Q")->value();
    return true;
}

//Simple formula application
DataSet AnalyticLinear2CompBolusK::predict(const Duration &start, const Duration &until, const ParameterSet* &parameters, uint points, const SharedDosage &dosage, Residuals &residuals, bool uniform)
{
    Q_UNUSED(uniform);

    //Fetch the parameters
    double CL, V1, V2, Q;

    fetchParameters(parameters, CL, V1, V2, Q);

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);
    Q_ASSERT(V2 != 0.0);

    //Compute related parameters
    double k12 = Q/V1;
    double k21 = Q/V2;
    double Ke  = CL/V1;

    //Fetch the dosage
    double D = dosage->dose().valueIn(_ug);

    double sumK = k12 + k21 + Ke;
    double root = sqrt(sumK*sumK - 4*k21*Ke);

    //Check the validity of root
    Q_ASSERT(!isnan(root));
    Q_ASSERT(!isinf(root));
    Q_ASSERT(root != 0.0);

    double Beta = (sumK - root)/2;
    double Alpha = (sumK + root)/2;
    // double Alpha = k21*Ke/Beta; // Same as the line above
    double resid1 = residuals[0].value + D/V1;
    double resid2 = residuals[1].value;


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

    double eht1;
    double eht2;

    double B = (-k12+k21-Ke+root)*resid1 +2*k21*resid2;
    double A = (k12-k21+Ke+root)*resid1 - 2*k21*resid2;

    //Calculate each point
    for (uint i=0; i < points; i++) {
        eht1 = exp(-Beta*T);
        eht2 = exp(-Alpha*T);
        data[i] = (A*eht2 + B*eht1)/(2*root);
        T += delta;
    }

    //Recalibrate start time
    T -= delta;

    double BB2 = 2*k12*resid1 + (k12-k21+Ke+root)*resid2;
    double A2 = -2*k12*resid1 +(-k12+k21-Ke+root)*resid2;

    //Set the new residuals
    residuals[0] = Residual(data[points-1]);
    residuals[1] = Residual((A2*eht2 + BB2*eht1)/(2*root));

    return data;
}
