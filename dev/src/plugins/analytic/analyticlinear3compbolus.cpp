//@@license@@

#include "analyticlinear3compbolus.h"

#include "core_errors.h"
#include "coremessagehandler.h"
#include "core.h"
#include "stdparamnames.h"
#include "knownmodels.h"
#include "version.h"

#include <math.h>

#define PI 3.14159265358979323846

AnalyticLinear3CompBolusK::AnalyticLinear3CompBolusK()
{
    _priority = 10;

    //We want ug for the dose
    _ug.fromString("ug");
    //And ug/l for the result
    _ugl.fromString("ug/l");

    // Parameters
    _parameters.clear();
    _parameters.append(Descriptor("Ke",StdParamNames::getNameKe(),StdParamNames::getDescCL()));
    _parameters.append(Descriptor("V1",StdParamNames::getNameV1(),StdParamNames::getDescV1()));
    _parameters.append(Descriptor("K12",StdParamNames::getNameK12(),StdParamNames::getDescK12()));
    _parameters.append(Descriptor("K21",StdParamNames::getNameK21(),StdParamNames::getDescK21()));
    _parameters.append(Descriptor("K13",StdParamNames::getNameK13(),StdParamNames::getDescK13()));
    _parameters.append(Descriptor("K31",StdParamNames::getNameK31(),StdParamNames::getDescK31()));
}

AnalyticLinear3CompBolusClV::AnalyticLinear3CompBolusClV()
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
    _parameters.append(Descriptor("V3",StdParamNames::getNameV3(),StdParamNames::getDescV3()));
    _parameters.append(Descriptor("Q2",StdParamNames::getNameQ2(),StdParamNames::getDescQ2()));
    _parameters.append(Descriptor("Q3",StdParamNames::getNameQ3(),StdParamNames::getDescQ3()));
}

//Create it here
ModelDescriptor AnalyticLinear3CompBolusK::descriptor()
{
    return ModelDescriptor ( A3C_BOLUS_K_ID,
                             MODEL_LINEAR_3COMP_BOLUS_K_ID,
                             tr("Linear Three-compartment Bolus (Analytic) (to be implemented)"),
                             tr("Model using an analytic approach from the CHUV."),
                             "bolus" );
}

ModelDescriptor AnalyticLinear3CompBolusClV::descriptor()
{
    return ModelDescriptor ( A3C_BOLUS_CLV_ID,
                             MODEL_LINEAR_3COMP_BOLUS_CLV_ID,
                             tr("Linear Three-compartment Bolus (Analytic) (to be implemented)"),
                             tr("Model using an analytic approach from the CHUV."),
                             "bolus" );
}

//Yeah!
Version AnalyticLinear3CompBolusK::version()
{
    return Version("0.1");
}

Version AnalyticLinear3CompBolusClV::version()
{
    return Version("0.1");
}

//Static (that's what the CHUV said)
Duration AnalyticLinear3CompBolusK::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
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

Duration AnalyticLinear3CompBolusClV::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
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

bool AnalyticLinear3CompBolusK::fetchParameters(const ParameterSet* &parameters, double &CL, double &V1, double &V2, double &V3, double &Q2, double &Q3) {

    double Ke = parameters->get("Ke")->value();
    V1 = parameters->get("V1")->value();
    double K12 = parameters->get("K12")->value();
    double K21 = parameters->get("K21")->value();
    double K13 = parameters->get("K13")->value();
    double K31 = parameters->get("K31")->value();

    CL = Ke*V1;
    Q2 = K12*V1;
    V2 = Q2/K21;
    Q3 = K13/V1;
    V3 = Q3/K31;

    return true;
}

DataSet AnalyticLinear3CompBolusK::steadystateImpl(const Duration &start, const Duration &duration, const ParameterSet* &parameters, const SharedDosage &dosage, const Duration &halflife, int multiplier, uint points, Residuals &residual, bool uniform, bool computeResidual)
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
    double CL, V1, V2, V3, Q2, Q3;

    fetchParameters(parameters, CL, V1, V2, V3, Q2, Q3);

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);
    Q_ASSERT(V2 != 0.0);
    Q_ASSERT(V3 != 0.0);

    //Compute the parameters
    double k   = CL/V1;
    double k12 = Q2/V1;
    double k21 = Q2/V2;
    double k13 = Q3/V1;
    double k31 = Q3/V3;

    //Compute the invariants
    double a0 = k * k21 * k31;
    double a1 = k*k31 + k21*k31 + k21*k13 + k*k21 + k31*k12;
    double a2 = k + k12 + k13 +k21 + k31;

    double p = a1 - ((a2*a2) / 3);
    double q = (2 * pow(a2, 3) / 27) - (a1 * a2 / 3) + a0;

    double r1 = sqrt(-(pow(p, 3) / 27));

    //Check for invalid operations
    Q_ASSERT(!isnan(r1));
    Q_ASSERT(!isinf(r1));

    //Compute the invariants
    double r2 = 2 * pow(r1, 1/3);

    double Phi   = acos(-q / (2*r1)) / 3;
    double Alpha = -(cos(Phi)*r2 - a2/3);
    double Beta  = -(cos(Phi + (2*PI)/3)*r2 - a2/3);
    double Gamma = -(cos(Phi + (4*PI)/3)*r2 - a2/3);

    //Check for division by 0
    Q_ASSERT(Alpha != Beta);
    Q_ASSERT(Alpha != Gamma);
    Q_ASSERT(Gamma != Beta);

    //Compute the invariants
    double A = ((k21 - Alpha) * (k31 - Alpha)) / (V1 * (Alpha - Beta) * (Alpha - Gamma));
    double B = ((k21 - Beta)  * (k31 - Beta )) / (V1 * (Beta - Alpha) * (Beta  - Gamma));
    double C = ((k21 - Gamma) * (k31 - Gamma)) / (V1 * (Gamma - Beta) * (Gamma - Alpha));

    double expAlI = exp(-Alpha*I);
    double expBeI = exp(-Beta*I);
    double expGaI = exp(-Gamma*I);

    //Calculate each point
    double expAlT, expBeT, expGaT;
    double part1, part2, part3;

    for (uint i=0; i < points; i++) {
        expAlT = exp(-Alpha*T);
        expBeT = exp(-Beta*T);
        expGaT = exp(-Gamma*T);

        part1 = (A * expAlT) / (1 - expAlI);
        part2 = (B * expBeT) / (1 - expBeI);
        part3 = (C * expGaT) / (1 - expGaI);

        data[i] = D*(part1 + part2 + part3);
        T += delta;
    }

    //Recalibrate start time
    T -= delta;

    //Compute the residual
    if (computeResidual)
        residual[0] = Residual(data[points-1]);

    return data;
}

bool AnalyticLinear3CompBolusClV::fetchParameters(const ParameterSet* &parameters, double &CL,double &V1,double &V2,double &V3,double &Q2, double &Q3) {
    CL = parameters->get("CL")->value();
    V1 = parameters->get("V1")->value();
    V2 = parameters->get("V2")->value();
    V3 = parameters->get("V3")->value();
    Q2 = parameters->get("Q2")->value();
    Q3 = parameters->get("Q3")->value();
    return true;
}

//Simple formula application
DataSet AnalyticLinear3CompBolusK::predict(const Duration &start, const Duration &until, const ParameterSet* &parameters, uint points, const SharedDosage &dosage, Residuals &residuals, bool uniform)
{
    Q_UNUSED(uniform);

    //Fetch the parameters

    double CL, V1, V2, V3, Q2, Q3;

    fetchParameters(parameters,CL,V1,V2,V3,Q2,Q3);
    //Check for division by 0
    Q_ASSERT(V1 != 0.0);
    Q_ASSERT(V2 != 0.0);
    Q_ASSERT(V3 != 0.0);

    //TODO: Implement the correct 3 component model below this point

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
    double H1  = CL/V1;
    double Hz = CL/V2;
    //Concentration at time 0 : Dose/V1
    double C0 = D/V1;
    double firstPart = C0*0.2;
    double secondPart = C0*(1.0-0.2);
    double eht1;
    double ehtz;

    //Calculate each point
    for (uint i=0; i < points; i++) {
        eht1 = exp(-H1*T);
        ehtz = exp(-Hz*T);
        data[i] = firstPart * eht1 + secondPart * ehtz + residuals.at(0).value * eht1 + residuals.at(1).value * ehtz;
        T += delta;
    }

    //Recalibrate start time
    T -= delta;

    //ToDo set the new residuals
    residuals[0] = Residual(firstPart * eht1 + residuals.at(0).value * eht1);
    residuals[1] = Residual(secondPart * ehtz + residuals.at(1).value * ehtz);

    return data;
}
