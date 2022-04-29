//@@license@@

#include "analyticlinear2compextra.h"

#include "core_errors.h"
#include "coremessagehandler.h"
#include "stdparamnames.h"
#include "knownmodels.h"
#include "version.h"

#include <QStringList>

#include <math.h>


AnalyticLinear2CompExtraK::AnalyticLinear2CompExtraK()
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
    _parameters.append(Descriptor("Ka",StdParamNames::getNameKA(),StdParamNames::getDescKA()));
    _parameters.append(Descriptor("F",StdParamNames::getNameF(),StdParamNames::getDescF()));
}

AnalyticLinear2CompExtraClV::AnalyticLinear2CompExtraClV()
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
    _parameters.append(Descriptor("Q",StdParamNames::getNameQ(),StdParamNames::getDescQ()));
    _parameters.append(Descriptor("Ka",StdParamNames::getNameKA(),StdParamNames::getDescKA()));
    _parameters.append(Descriptor("F",StdParamNames::getNameF(),StdParamNames::getDescF()));
}

//Create it here
ModelDescriptor AnalyticLinear2CompExtraK::descriptor()
{
    return ModelDescriptor ( A2C_EXTRA_K_ID,
                             MODEL_LINEAR_2COMP_EXTRA_K_ID,
                             tr("Linear Two-compartment Extravascular (Analytic)"),
                             tr("Model using an analytic approach from the CHUV."),
                             "extra" );
}

ModelDescriptor AnalyticLinear2CompExtraClV::descriptor()
{
    return ModelDescriptor ( A2C_EXTRA_CLV_ID,
                             MODEL_LINEAR_2COMP_EXTRA_CLV_ID,
                             tr("Linear Two-compartment Extravascular (Analytic)"),
                             tr("Model using an analytic approach from the CHUV."),
                             "extra" );
}

//Yeah!
Version AnalyticLinear2CompExtraK::version()
{
    return Version("0.1");
}

Version AnalyticLinear2CompExtraClV::version()
{
    return Version("0.1");
}


//Static (that's what the CHUV said)
Duration AnalyticLinear2CompExtraK::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
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

Duration AnalyticLinear2CompExtraClV::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
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

bool AnalyticLinear2CompExtraK::fetchParameters(const ParameterSet* &parameters, double &CL,double &V1, double &V2, double &Q, double &Ka, double &F) {

    double Ke = parameters->get("Ke")->value();
    V1 = parameters->get("V1")->value();
    double K12 = parameters->get("K12")->value();
    double K21 = parameters->get("K21")->value();
    Ka = parameters->get("Ka")->value();
    F = parameters->get("F")->value();
    Q = K12*V1;
    CL = Ke*V1;
    V2 = K21/Q;

    return true;
}

DataSet AnalyticLinear2CompExtraK::steadystateImpl(const Duration &start, const Duration &duration, const ParameterSet* &parameters, const SharedDosage &dosage, const Duration &halflife, int multiplier, uint points, Residuals &residual, bool uniform, bool computeResidual)
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
    double CL, V1, V2, Q, ka, F;

    fetchParameters(parameters, CL, V1, V2, Q, ka, F);

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);
    Q_ASSERT(V2 != 0.0);

    //Compute the parameters
    double k12 = Q/V1;
    double k21 = Q/V2;
    double k   = CL/V1;

    //Compute the invariants
    double sumK = k12 + k21 + k;
    double root = sqrt(sumK*sumK - 4*k21*k);

    //Check for invalid operations
    Q_ASSERT(!isnan(root));
    Q_ASSERT(!isinf(root));
    Q_ASSERT(root != 0.0);

    //Compute the invariants
    double Beta  = (sumK - root)/2;
    Q_ASSERT(Beta != 0.0);

    double Alpha = k21*k / Beta;
    Q_ASSERT(Alpha != Beta);

    //Check for invalid operations
    Q_ASSERT(k21 != Beta);
    Q_ASSERT(k21 != Alpha);
    Q_ASSERT(ka != Beta);
    Q_ASSERT(ka != Alpha);

    //Compute the invariants
    double A = (ka * (k21 - Alpha)) / (V1 * (ka - Alpha) * (Beta - Alpha));
    double B = (ka * (k21 - Beta)) / (V1 * (ka - Beta) * (Alpha - Beta));

    double expAlI = exp(-Alpha*I);
    double expBeI = exp(-Beta*I);
    double expKaI = exp(-ka*I);

    //Calculate each point
    double expAlT, expBeT, expKaT;
    double part1, part2, part3;

    for (uint i=0; i < points; i++) {
        expAlT = exp(-Alpha*T);
        expBeT = exp(-Beta*T);
        expKaT = exp(-ka*T);

        part1 = (A * expAlT) / (1 - expAlI);
        part2 = (B * expBeT) / (1 - expBeI);
        part3 = ((A + B) * expKaT) / (1 - expKaI);

        data[i] = D*(part1 + part2 - part3);
        T += delta;
    }

    //Recalibrate start time
    T -= delta;

    //Compute the residual
    if (computeResidual)
        residual[0] = Residual(data[points-1]);

    return data;
}

bool AnalyticLinear2CompExtraClV::fetchParameters(const ParameterSet* &parameters, double &CL,double &V1, double &V2, double &Q, double &Ka, double &F) {

    CL = parameters->get("CL")->value();
    V1 = parameters->get("V1")->value();
    V2 = parameters->get("V2")->value();
    Ka = parameters->get("Ka")->value();
    F = parameters->get("F")->value();
    Q = parameters->get("Q")->value();

    return true;
}

//Simple formula application
DataSet AnalyticLinear2CompExtraK::predict(const Duration &start, const Duration &until, const ParameterSet* &parameters, uint points, const SharedDosage &dosage, Residuals &residuals, bool uniform)
{
    Q_UNUSED(uniform);

    //Fetch the parameters
    double CL, V1, V2, Q, ka, F;

    fetchParameters(parameters, CL, V1, V2, Q, ka, F);
    //Check for division by 0
    Q_ASSERT(V1 != 0.0);
    Q_ASSERT(V2 != 0.0);

    //Compute related parameters
    double k12 = Q/V1;
    double k21 = Q/V2;
    double ke  = CL/V1;

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
    double sumK = k12 + k21 + ke;
    double rootK = sqrt(sumK*sumK - 4*k21*ke);

    //Check the validity of rootK
    Q_ASSERT(!isnan(rootK));
    Q_ASSERT(!isinf(rootK));
    Q_ASSERT(rootK != 0.0);

    double Beta = (sumK - rootK)/2;
    double Alpha = (sumK + rootK)/2;

    double resid1 = residuals[0].value;
    double resid2 = residuals[1].value;
    double resid3 = residuals[2].value + F*D/V1;


    double A = pow(k12,3) * ka * resid1 +
            (k21 - ka) * (pow(k21 - ke,2) *
                          (-ke * resid1 + ka * (resid1 + resid3)) +
                          ((ka - ke) * (ke * resid1 - k21 * (resid1 + 2 * resid2)) + ka * (-k21 + ke) * resid3) * rootK) +
            pow(k12,2) * (k21 * (-ke * resid1 + ka * (3 * resid1 + resid3)) +
                          ka * (-ka * (resid1 + resid3) + resid1 * (3 * ke + rootK))) +
            k12 * (pow(k21,2) * (3 * ka * resid1 - 2 * ke * resid1 + 2 * ka * resid3) -
                   k21 * (2 * pow(ka,2) * (resid1 + resid3) - 2 * ka * ke * (resid1 + resid3) +
                          ka * (2 * resid2 + resid3) * rootK + ke * resid1 * (2 * ke + rootK)) -
                   ka * (ka * (resid1 + resid3) * (2 * ke + rootK) -
                         ke * resid1 * (3 * ke + 2 * rootK)));

    double B = pow(k12,3) * ka * resid1 +
            (k21 - ka)*(pow(k21 - ke,2) *
                        (-ke * resid1 + ka * (resid1 + resid3)) + ((ka - ke) * (-ke * resid1 + k21 * (resid1 + 2 * resid2)) + ka * (k21 - ke) * resid3) * rootK) +
            pow(k12,2) * (k21 * (-ke * resid1 + ka * (3 * resid1 + resid3)) -
                          ka * (ka * (resid1 + resid3) + resid1 * (-3 * ke + rootK))) +
            k12 * (pow(k21,2) * (3 * ka * resid1 - 2 * ke * resid1 + 2 * ka * resid3) +
                   ka * (ke * resid1 * (3 * ke - 2 * rootK) -
                         ka * (resid1 + resid3) * (2 * ke - rootK)) +
                   k21 * (-2 * pow(ka,2) * (resid1 + resid3) + 2 * ka * ke * (resid1 + resid3) +
                          ka * (2 * resid2 + resid3) * rootK + ke * resid1 * (-2 * ke + rootK)));

    double C = - 2 * (k21 - ka) * ka * (pow(k12,2) + pow(k21-ke,2) + 2 * k12 * (k21 + ke)) * resid3;

    double divider = pow((k12 + k21 - 2 * ka + ke) * rootK , 2) - pow(pow(k12,2) + pow(k21 - ke,2) + 2 * k12 * (k21 + ke) , 2);

    //Check for division by 0
    Q_ASSERT(divider != 0.0);

    //Calculate each point
    for (uint i=0; i < points; i++) {
        double eht1 = exp(-Beta*T);
        double ehtz = exp(-Alpha*T);
        double ehtKa = exp(-ka*T);
        data[i] = -2*(B * eht1 + A * ehtz + C * ehtKa)/divider;

        T += delta;
    }

    //Recalibrate start time
    T -= delta;

    // Set the new * residuals
    residuals[0] = Residual(data[points-1]);

    {
        double A = -(pow(k12,2) + pow(k21 - ke,2) +
                      2 * k12 * (k21 + ke)) * ((k21 - ka) * (ka - ke) * resid2 +
                     k12 * ka * (resid2 +
                        resid3)) + ((k12 * ka + (k21 - ka) * (ka -
                           ke)) * (2 * k12 * resid1 + (k12 - k21 + ke) * resid2) +
                     k12 * ka * (k12 + k21 - 2 * ka + ke) * resid3) * rootK;

        double B = -(pow(k12,2) + pow(k21 - ke,2) +
                     2 * k12 * (k21 + ke)) * ((k21 - ka) * (ka - ke) * resid2 +
                    k12 * ka * (resid2 +
                       resid3)) - ((k12 * ka + (k21 - ka) * (ka -
                          ke)) * (2 * k12 * resid1 + (k12 - k21 + ke) * resid2) +
                    k12 * ka * (k12 + k21 - 2 * ka + ke) * resid3) * rootK;

        double C = 2 * k12 * ka * (pow(k12,2) + pow(k21 - ke,2) + 2 *k12 *(k21 + ke)) *resid3;

        double divider = -pow(pow(k12,2) + pow(k21 - ke,2) + 2 * k12 * (k21 + ke),2) + pow(k12 + k21 - 2 * ka + ke,2) * pow(rootK,2);

        //Check for division by 0
        Q_ASSERT(divider != 0.0);

        double eht1 = exp(-Beta*T);
        double ehtz = exp(-Alpha*T);
        double ehtKa = exp(-ka*T);
        residuals[1] = Residual(2*(B * eht1 + A * ehtz + C * ehtKa)/divider);
    }

    residuals[2] = Residual(exp(-ka*T) * resid3);

    return data;
}
