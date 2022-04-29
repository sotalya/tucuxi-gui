//@@license@@

#include "analyticlinear3compextra.h"

#include "core_errors.h"
#include "coremessagehandler.h"
#include "stdparamnames.h"
#include "knownmodels.h"
#include "version.h"

#include <QStringList>

#include <math.h>

#define PI 3.14159265358979323846

AnalyticLinear3CompExtraK::AnalyticLinear3CompExtraK()
{
    _priority = 10;
    //We want mg for the dose
    _ug.fromString("ug");
    //And ug/l for the result
    _ugl.fromString("ug/l");

    // Parameters
    _parameters.clear();
    _parameters.append(Descriptor("Ke",StdParamNames::getNameKe(),StdParamNames::getDescKe()));
    _parameters.append(Descriptor("V1",StdParamNames::getNameV1(),StdParamNames::getDescV1()));
    _parameters.append(Descriptor("K12",StdParamNames::getNameK12(),StdParamNames::getDescK12()));
    _parameters.append(Descriptor("K21",StdParamNames::getNameK21(),StdParamNames::getDescK21()));
    _parameters.append(Descriptor("K13",StdParamNames::getNameK13(),StdParamNames::getDescK13()));
    _parameters.append(Descriptor("K31",StdParamNames::getNameK31(),StdParamNames::getDescK31()));
    _parameters.append(Descriptor("Ka",StdParamNames::getNameKA(),StdParamNames::getDescKA()));
    _parameters.append(Descriptor("F",StdParamNames::getNameF(),StdParamNames::getDescF()));
}

AnalyticLinear3CompExtraClV::AnalyticLinear3CompExtraClV()
{
    _priority = 10;
    //We want mg for the dose
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
    _parameters.append(Descriptor("Ka",StdParamNames::getNameKA(),StdParamNames::getDescKA()));
    _parameters.append(Descriptor("F",StdParamNames::getNameF(),StdParamNames::getDescF()));
}

//Create it here
ModelDescriptor AnalyticLinear3CompExtraK::descriptor()
{
    return ModelDescriptor ( A3C_EXTRA_K_ID,
                             MODEL_LINEAR_3COMP_EXTRA_K_ID,
                             tr("Linear Three-compartment Extravascular (Analytic) (to be implemented)"),
                             tr("Model using an analytic approach from the CHUV."),
                             "extra" );
}

ModelDescriptor AnalyticLinear3CompExtraClV::descriptor()
{
    return ModelDescriptor ( A3C_EXTRA_CLV_ID,
                             MODEL_LINEAR_3COMP_EXTRA_CLV_ID,
                             tr("Linear Three-compartment Extravascular (Analytic) (to be implemented)"),
                             tr("Model using an analytic approach from the CHUV."),
                             "extra" );
}

//Yeah!
Version AnalyticLinear3CompExtraK::version()
{
    return Version("0.1");
}

Version AnalyticLinear3CompExtraClV::version()
{
    return Version("0.1");
}

//Static (that's what the CHUV said)
Duration AnalyticLinear3CompExtraK::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
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

Duration AnalyticLinear3CompExtraClV::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
{
    //Fetch the parameters
    double CL = parameters->get("CL")->value();
    double V1 = parameters->get("V")->value();

    //Check for division by 0
    Q_ASSERT(CL != 0.0);
    Q_ASSERT(V1 != 0.0);

    //Log(2) static value
#define LOG2H 0.693147180559945*3600000.0
    // t½ = ln(2) / (CL/V)
    return Duration(0,0,0,LOG2H / (CL/V1));
#undef LOG2H
}

bool AnalyticLinear3CompExtraK::fetchParameters(const ParameterSet* &parameters, double &CL,double &V1, double &V2, double &V3, double &Q2, double &Q3, double &Ka, double &F) {
    double Ke = parameters->get("Ke")->value();
    V1 = parameters->get("V1")->value();
    double K12 = parameters->get("K12")->value();
    double K21 = parameters->get("K21")->value();
    double K13 = parameters->get("K13")->value();
    double K31 = parameters->get("K31")->value();
    Ka = parameters->get("Ka")->value();
    F = parameters->get("F")->value();

    CL = Ke*V1;
    Q2 = K12*V1;
    V2 = Q2/K21;
    Q3 = K13/V2;
    V3 = Q3/K31;

    return true;
}

DataSet AnalyticLinear3CompExtraK::steadystateImpl(const Duration &start, const Duration &duration, const ParameterSet* &parameters, const SharedDosage &dosage, const Duration &halflife, int multiplier, uint points, Residuals &residual, bool uniform, bool computeResidual)
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
    double CL, V1, V2, V3, Q2, Q3, ka, F;

    fetchParameters(parameters, CL, V1, V2, V3, Q2, Q3, ka, F);

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

    Q_ASSERT(ka != Alpha);
    Q_ASSERT(ka != Beta);
    Q_ASSERT(ka != Gamma);

    //Compute the invariants
    double A = (ka * (k21 - Alpha) * (k31 - Alpha)) / (V1 * (ka - Alpha) * (Alpha - Beta) * (Alpha - Gamma));
    double B = (ka * (k21 - Beta)  * (k31 - Beta )) / (V1 * (ka - Beta)  * (Beta - Alpha) * (Beta  - Gamma));
    double C = (ka * (k21 - Gamma) * (k31 - Gamma)) / (V1 * (ka - Gamma) * (Gamma - Beta) * (Gamma - Alpha));

    double expAlI = exp(-Alpha*I);
    double expBeI = exp(-Beta*I);
    double expGaI = exp(-Gamma*I);
    double expKaI = exp(-ka*I);

    //Calculate each point
    double expAlT, expBeT, expGaT, expKaT;
    double part1, part2, part3, part4;

    for (uint i=0; i < points; i++) {
        expAlT = exp(-Alpha*T);
        expBeT = exp(-Beta*T);
        expGaT = exp(-Gamma*T);
        expKaT = exp(-ka*T);

        part1 = (A * expAlT) / (1 - expAlI);
        part2 = (B * expBeT) / (1 - expBeI);
        part3 = (C * expGaT) / (1 - expGaI);
        part4 = ((A+B+C) * expKaT) / (1 - expKaI);

        data[i] = D*(part1 + part2 + part3 - part4);
        T += delta;
    }

    //Recalibrate start time
    T -= delta;

    //Compute the residual
    if (computeResidual)
        residual[0] = Residual(data[points-1]);

    return data;
}

bool AnalyticLinear3CompExtraClV::fetchParameters(const ParameterSet* &parameters, double &CL,double &V1, double &V2, double &V3, double &Q2, double &Q3, double &Ka, double &F) {
    CL = parameters->get("CL")->value();
    V1 = parameters->get("V1")->value();
    V2 = parameters->get("V2")->value();
    V3 = parameters->get("V3")->value();
    Q2 = parameters->get("Q2")->value();
    Q3 = parameters->get("Q3")->value();
    Ka = parameters->get("Ka")->value();
    F = parameters->get("F")->value();

    return true;
}


//Simple formula application
DataSet AnalyticLinear3CompExtraK::predict(const Duration &start, const Duration &until, const ParameterSet* &parameters, uint points, const SharedDosage &dosage, Residuals &residuals, bool uniform)
{
    Q_UNUSED(uniform);

    //Fetch the parameters
    double CL,V1,V2,V3,Q2,Q3,Ka,F;

    fetchParameters(parameters,CL,V1,V2,V3,Q2,Q3,Ka,F);

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
    double H1 = CL/V1;
    double Hz = CL/V2;
    double FDKaV1 = F*D*Ka/V1;

    //Check for division by 0
    Q_ASSERT(Ka != H1);
    Q_ASSERT(Ka != Hz);

    double firstPart  = FDKaV1 * 0.2/(Ka - H1);
    double secondPart = FDKaV1 * 0.8/(Ka - Hz);
    double thirdPart  = FDKaV1 * (0.2/(Ka - H1) + 0.8/(Ka - Hz));
    double eht1;
    double ehtz;
    double ehtKa;

    //Calculate each point
    for (uint i=0; i < points; i++) {
        eht1 = exp(-H1*T);
        ehtz = exp(-Hz*T);
        ehtKa = exp(-Ka*T);
        data[i] = firstPart * eht1 + secondPart * ehtz - thirdPart * ehtKa
                  + residuals.at(0).value * eht1
                  + residuals.at(1).value * ehtz;
        T += delta;
    }

    //Recalibrate start time
    T -= delta;

    //ToDO : set the new residuals
    residuals[0] = Residual(firstPart * eht1 - FDKaV1 * (0.2/(Ka - H1)) * ehtKa + residuals.at(0).value * eht1);
    residuals[1] = Residual(secondPart * ehtz - FDKaV1 * (0.8/(Ka - Hz)) * ehtKa + residuals.at(1).value * ehtz);

    return data;
}

