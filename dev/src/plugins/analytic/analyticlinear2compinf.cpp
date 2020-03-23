#include "analyticlinear2compinf.h"

#include "core_errors.h"
#include "coremessagehandler.h"
#include "stdparamnames.h"
#include "knownmodels.h"
#include "version.h"

#include <QStringList>

#include <math.h>


AnalyticLinear2CompInfK::AnalyticLinear2CompInfK()
{
    _priority = 10;
    //Set the parameters
    _parameters.clear();
    _parameters.append(Descriptor("K12",StdParamNames::getNameK12(),StdParamNames::getDescK12()));
    _parameters.append(Descriptor("K21",StdParamNames::getNameK21(),StdParamNames::getDescK21()));
    _parameters.append(Descriptor("Ke",StdParamNames::getNameKe(),StdParamNames::getDescKe()));
    _parameters.append(Descriptor("V1",StdParamNames::getNameV1(),StdParamNames::getDescV1()));
}

AnalyticLinear2CompInfClV::AnalyticLinear2CompInfClV()
{
    _priority = 10;
    //Set the parameters
    _parameters.clear();
    _parameters.append(Descriptor("CL",StdParamNames::getNameCL(),StdParamNames::getDescCL()));
    _parameters.append(Descriptor("V1",StdParamNames::getNameV1(),StdParamNames::getDescV1()));
    _parameters.append(Descriptor("V2",StdParamNames::getNameV2(),StdParamNames::getDescV2()));
    _parameters.append(Descriptor("Q",StdParamNames::getNameQ(),StdParamNames::getDescQ()));
}

//Create it here
ModelDescriptor AnalyticLinear2CompInfK::descriptor()
{
    return ModelDescriptor ( A2C_INFUS_K_ID,
                             MODEL_LINEAR_2COMP_INFUS_K_ID,
                             tr("Linear Two-compartment Infusion (Analytic)"),
                             tr("Model using an analytic approach from the CHUV."),
                             "infu" );
}

//Create it here
ModelDescriptor AnalyticLinear2CompInfClV::descriptor()
{
    return ModelDescriptor ( A2C_INFUS_CLV_ID,
                             MODEL_LINEAR_2COMP_INFUS_CLV_ID,
                             tr("Linear Two-compartment Infusion (Analytic)"),
                             tr("Model using an analytic approach from the CHUV."),
                             "infu" );
}

//Yeah!
Version AnalyticLinear2CompInfK::version()
{
    return Version("0.1");
}

//Yeah!
Version AnalyticLinear2CompInfClV::version()
{
    return Version("0.1");
}

//Static (that's what the CHUV said)
Duration AnalyticLinear2CompInfK::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
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
Duration AnalyticLinear2CompInfClV::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
{
    //Fetch the parameters
    double CL = parameters->get("CL")->value();
    double V1 = parameters->get("V1")->value();

    //Check for division by 0
    Q_ASSERT(CL != 0.0);
    Q_ASSERT(V1 != 0.0);

    //Log(2) static value
#define LOG2H 0.693147180559945*3600000.0
    // t½ = ln(2) / (CL/V)
    return Duration(0,0,0,LOG2H / (CL/V1));
#undef LOG2H
}

#include <iostream>


bool AnalyticLinear2CompInfK::fetchParameters(const ParameterSet* &parameters, double &V1,double &k12,double &k21,double &Ke)
{
    //Fetch the parameters
    k12 = parameters->get("K12")->value();
    k21 = parameters->get("K21")->value();
    Ke = parameters->get("Ke")->value();
    V1 = parameters->get("V1")->value();

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);

    return true;
}

DataSet AnalyticLinear2CompInfK::computeSteadyState(const Duration &start, const Duration &duration, const ParameterSet* &parameters, const SharedDosage &dosage, uint points, Residuals &residual, bool computeResidual)
{
    //The result
    DataSet data(points, _ugl);

    //Get the dose
    double D = dosage->dose().valueIn(_ug);

    //Get the interval
    double I = dosage->interval().toHours();

    //Get the infusion time
    double F = dosage->tinf().toHours();

    //Get the start time
    double T = start.toHours();

    //Get the delta
    double delta = 0.0;
    if (points > 1)
        delta = duration.toHours() / (points - 1);
    else
        T += duration.toHours();

    //Get the parameters
    double k12, k21, k, V1;

    fetchParameters(parameters, V1, k12, k21, k);

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);
    Q_ASSERT(F  != 0.0);

    //Compute the invariants
    double sumK = k12 + k21 + k;
    double root = sqrt(sumK*sumK - 4*k21*k);

    //Check for invalid operations
    Q_ASSERT(!isnan(root));
    Q_ASSERT(!isinf(root));
    Q_ASSERT(root != 0.0);

    //Compute the invariants
    double Beta  = (sumK - root)/2;

    //Check for division by 0
    Q_ASSERT(Beta != 0.0);

    //Compute the invariants
    double Alpha = k21*k / Beta;

    //Check for division by 0
    Q_ASSERT(Alpha != Beta);
    Q_ASSERT(Alpha != 0.0);
    Q_ASSERT(k21 != Beta);
    Q_ASSERT(k21 != Alpha);

    //Compute the invariants
    double A = (Alpha - k21) / (V1 * (Alpha - Beta));
    double B = (Beta  - k21) / (V1 * (Beta - Alpha));

    double expAlI = exp(-Alpha*I);
    double expBeI = exp(-Beta*I);

    double expAlF = exp(-Alpha*F);
    double expBeF = exp(-Beta*F);

    double AAl = A / Alpha;
    double BBe = B / Beta;

    double firstTerm = D/F;

    //Calculate each point
    double expAlT, expBeT, expAlTF, expBeTF;
    double part1, part2, common1, common2;

    for (uint i=0; i < points; i++) {
        expAlT = exp(-Alpha*T);
        expBeT = exp(-Beta*T);

        expAlTF = exp(-Alpha*(T - F));
        expBeTF = exp(-Beta*(T - F));

        common1 = (expAlTF * (1 - expAlF)) / (1 - expAlI);
        common2 = (expBeTF * (1 - expBeF)) / (1 - expBeI);

        part1 = AAl * (T > F ? common1 : ((1 - expAlT) + (expAlI * common1)));
        part2 = BBe * (T > F ? common2 : ((1 - expBeT) + (expBeI * common2)));

        data[i] = firstTerm*(part1 + part2);
        T += delta;
    }

    //Recalibrate start time
    T -= delta;

    //Compute the residual
    if (computeResidual)
        residual[0] = Residual(data[points-1]);

    return data;
}

bool AnalyticLinear2CompInfClV::fetchParameters(const ParameterSet* &parameters, double &V1,double &k12,double &k21,double &Ke)
{
    //Fetch the parameters
    double CL = parameters->get("CL")->value();
    double V2 = parameters->get("V2")->value();
    double Q = parameters->get("Q")->value();
    V1 = parameters->get("V1")->value();

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);
    Q_ASSERT(V2 != 0.0);

    //Compute related parameters
    k12 = Q/V1;
    k21 = Q/V2;
    Ke  = CL/V1;
    return true;
}

//Compute the prediction for the given interval
DataSet AnalyticLinear2CompInfK::compute(const Duration &start, const Duration &until, const ParameterSet* &parameters, uint points, const SharedDosage &dosage, Residuals &residuals)
{
    //Parameters
    double k12;
    double k21;
    double Ke;
    double V1;


    fetchParameters(parameters,V1,k12,k21,Ke);

    //Fetch the dosage
    double D = dosage->dose().valueIn(_ug);
    double Tinf = dosage->tinf().toHours();

    //Check for division by 0
    Q_ASSERT(Tinf != 0.0);

    double deltaD = (D/V1)/Tinf;

    double sumK = k12 + k21 + Ke;
    double root = sqrt(sumK*sumK - 4*k21*Ke);

    //Check the validity of root
    Q_ASSERT(!isnan(root));
    Q_ASSERT(!isinf(root));
    Q_ASSERT(root != 0.0);

    double Beta = (sumK - root)/2;
    double Alpha = (sumK + root)/2;
    // double Alpha = k21*Ke/Beta; // Same as the line above
    double resid1 = residuals[0].value;
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

    double divider = root * (-sumK + root) * (sumK + root);

    //Check for division by 0
    Q_ASSERT(divider != 0.0);

    double residInfu1;
    double residInfu2;
    {
        double T = Tinf;
        eht1 = exp(-Beta*T);
        eht2 = exp(-Alpha*T);
        residInfu1 = (
                2*deltaD*exp(Beta*T)*k21*(
                        eht1*(-k12-k21+Ke-root) + exp(-2*Beta*T)*(k12+k21-Ke+root)
                        + exp(root*T-Alpha*T)*(k12+k21-Ke-root)+exp(-Alpha*T-Beta*T)*(-k12-k21+Ke+root)
                    )
                )/divider;

        residInfu2 = (
        2*deltaD*exp(Beta*T)*k12*(
                        eht1*(-k12-k21-Ke-root) + exp(-2*Beta*T)*(k12+k21+Ke+root)
                        + exp(root*T-Alpha*T)*(k12+k21+Ke-root)+exp(-Alpha*T-Beta*T)*(-k12-k21-Ke+root)
            )
        )/divider;
    }

    double Ainfu = -k12-k21+Ke-root;
    double Binfu = k12+k21-Ke-root;

    double BPostInfu = (-k12+k21-Ke+root)*residInfu1 +2*k21*residInfu2;
    double APostInfu = (k12-k21+Ke+root)*residInfu1 - 2*k21*residInfu2;
    //Calculate each point
    for (uint i=0; i < points; i++) {
        eht1 = exp(-Beta*T);
        eht2 = exp(-Alpha*T);
        data[i] = (A*eht2 + B*eht1)/(2*root);
        // Now data corresponds to the result of residuals.
        // So add the infusion part...
        if (T<=Tinf)
        data[i] += (
                    2*deltaD*exp(Beta*T)*k21*(
                        Ainfu*(eht1 - exp(-2*Beta*T))
                        + Binfu *(exp(root*T-Alpha*T) - exp(-Alpha*T-Beta*T))
                        )
                    )/divider;
        else {
            // We add the effect of the infusion
            eht1 = exp(-Beta*(T-Tinf));
            eht2 = exp(-Alpha*(T-Tinf));
            data[i] += (APostInfu*eht2 + BPostInfu*eht1)/(2*root);
        }

        T += delta;
    }

    //Recalibrate start time
    T -= delta;

    //Set the new residuals
    // For the first compartment it corresponds to the last data
    residuals[0] = Residual(data[points-1]);

    // For the second compartment it requires some calculation
    // Effect of the old residual
    double BB2 = 2*k12*resid1 + (k12-k21+Ke+root)*resid2;
    double A2 = -2*k12*resid1 +(-k12+k21-Ke+root)*resid2;
    double r1 =(A2*eht2 + BB2*eht1)/(2*root);

    // Effect of the infu:
    if (start.toHours() + until.toHours() >= Tinf) {
        // Calculate it from the residual of Infusion at end of infusion
        BB2 = 2*k12*residInfu1 + (k12-k21+Ke+root)*residInfu2;
        A2 = -2*k12*residInfu1 +(-k12+k21-Ke+root)*residInfu2;
        r1 += (A2*eht2 + BB2*eht1)/(2*root);
    } else {
        // Calculate it during infusion
        eht1 = exp(-Beta*T);
        eht2 = exp(-Alpha*T);
        r1 += (2*deltaD*exp(Beta*T)*k12*
                 (
                    eht1*(-k12-k21-Ke-root) + exp(-2*Beta*T)*(k12+k21+Ke+root)
                    + exp(root*T-Alpha*T)*(k12+k21+Ke-root)+exp(-Alpha*T-Beta*T)*(-k12-k21-Ke+root)
                 )
              )/divider;
    }

    residuals[1] = Residual(r1);

    return data;
}
