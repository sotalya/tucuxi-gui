
#include "analyticnonlinear1compgammaextra.h"

#include <boost/math/special_functions/gamma.hpp>

#include "core_errors.h"
#include "coremessagehandler.h"
#include "stdparamnames.h"
#include "knownmodels.h"
#include "version.h"

#include <QStringList>

#include <math.h>


AnalyticNonlinear1CompGammaExtraK::AnalyticNonlinear1CompGammaExtraK()
{
    _priority = 10;
    //We want mg for the dose
    _ug.fromString("ug");
    //And ug/l for the result
    _ugl.fromString("ug/l");

    // Parameters
    _parameters.clear();
    _parameters.append(Descriptor("Ke",StdParamNames::getNameKe(),StdParamNames::getDescKe()));
    _parameters.append(Descriptor("V",StdParamNames::getNameV(),StdParamNames::getDescV()));
    _parameters.append(Descriptor("F",StdParamNames::getNameF(),StdParamNames::getDescF()));
    _parameters.append(Descriptor("A",StdParamNames::getNameA(),StdParamNames::getDescA()));
    _parameters.append(Descriptor("Ga1",StdParamNames::getNameGA1(),StdParamNames::getDescGA1()));
    _parameters.append(Descriptor("Gb1",StdParamNames::getNameGB1(),StdParamNames::getDescGB1()));
    _parameters.append(Descriptor("Ga2",StdParamNames::getNameGA2(),StdParamNames::getDescGA2()));
    _parameters.append(Descriptor("Gb2",StdParamNames::getNameGB2(),StdParamNames::getDescGB2()));
    _parameters.append(Descriptor("R",StdParamNames::getNameR(),StdParamNames::getDescR()));
}

AnalyticNonlinear1CompGammaExtraClV::AnalyticNonlinear1CompGammaExtraClV()
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
    _parameters.append(Descriptor("A",StdParamNames::getNameA(),StdParamNames::getDescA()));
    _parameters.append(Descriptor("Ga1",StdParamNames::getNameGA1(),StdParamNames::getDescGA1()));
    _parameters.append(Descriptor("Gb1",StdParamNames::getNameGB1(),StdParamNames::getDescGB1()));
    _parameters.append(Descriptor("Ga2",StdParamNames::getNameGA2(),StdParamNames::getDescGA2()));
    _parameters.append(Descriptor("Gb2",StdParamNames::getNameGB2(),StdParamNames::getDescGB2()));
    _parameters.append(Descriptor("F",StdParamNames::getNameF(),StdParamNames::getDescF()));
    _parameters.append(Descriptor("R",StdParamNames::getNameR(),StdParamNames::getDescR()));
}

//Create it here
ModelDescriptor AnalyticNonlinear1CompGammaExtraK::descriptor()
{
    return ModelDescriptor ( A1C_GAMMA_EXTRA_K_ID,
                             MODEL_NONLINEAR_1COMP_GAMMA_EXTRA_K_ID,
                             tr("Linear One-compartment Extravascular (Analytic)"),
                             tr("Model using an analytic approach from the CHUV."),
                             "extra" );
}

//Create it here
ModelDescriptor AnalyticNonlinear1CompGammaExtraClV::descriptor()
{
    return ModelDescriptor ( A1C_GAMMA_EXTRA_CLV_ID,
                             MODEL_NONLINEAR_1COMP_GAMMA_EXTRA_CLV_ID,
                             tr("Linear One-compartment Extravascular (Analytic)"),
                             tr("Model using an analytic approach from the CHUV."),
                             "extra" );
}

//Yeah!
Version AnalyticNonlinear1CompGammaExtraK::version()
{
    return Version("0.1");
}

Version AnalyticNonlinear1CompGammaExtraClV::version()
{
    return Version("0.1");
}

//Static (that's what the CHUV said)
Duration AnalyticNonlinear1CompGammaExtraK::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
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

//Static (that's what the CHUV said)
Duration AnalyticNonlinear1CompGammaExtraClV::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
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

bool AnalyticNonlinear1CompGammaExtraK::fetchParameters(const ParameterSet* &parameters, double &Ke, double &V, double &A, double &Ga1, double &Gb1, double &Ga2, double &Gb2, double &F, double &R) {

    //Fetch the parameters
    Ke = parameters->get("Ke")->value();
    A = parameters->get("A")->value();
    V = parameters->get("V")->value();
    Ga1 = parameters->get("Ga1")->value();
    Gb1 = parameters->get("Gb1")->value();
    Ga2 = parameters->get("Ga2")->value();
    Gb2 = parameters->get("Gb2")->value();
    F = parameters->get("F")->value();
    R = parameters->get("R")->value();

    //Check for division by 0
    Q_ASSERT(Ke != 0.0);

    return true;
}

bool AnalyticNonlinear1CompGammaExtraClV::fetchParameters(const ParameterSet* &parameters, double &Ke, double &V, double &A, double &Ga1, double &Gb1, double &Ga2, double &Gb2, double &F, double &R) {

    //Fetch the parameters
    double CL = parameters->get("CL")->value();
    V = parameters->get("V")->value();
    A = parameters->get("A")->value();
    Ga1 = parameters->get("Ga1")->value();
    Gb1 = parameters->get("Gb1")->value();
    Ga2 = parameters->get("Ga2")->value();
    Gb2 = parameters->get("Gb2")->value();
    F = parameters->get("F")->value();
    R = parameters->get("R")->value();

    //Check for division by 0
    Q_ASSERT(V != 0.0);
    Q_ASSERT(CL != 0.0);

    Ke = CL/V;
    return true;
}

//Simple formula application
DataSet AnalyticNonlinear1CompGammaExtraK::predict(const Duration &start, const Duration &until, const ParameterSet* &parameters, uint points, const SharedDosage &dosage, Residuals &residuals, bool uniform)
{
    Q_UNUSED(uniform);

    //parameters
    double Ke, V, A, Ga1, Gb1, Ga2, Gb2, F, R;

    fetchParameters(parameters, Ke, V, A, Ga1, Gb1, Ga2, Gb2, F, R);

    //Check for division by 0
    Q_ASSERT(Ke != 0.0);
    Q_ASSERT(Ke != Gb1);
    Q_ASSERT(Ke < Gb1);
    Q_ASSERT(Ke != Gb2);

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
    double eht;
    //Calculate each point
    for (uint i=0; i < points; i++) {

        eht = exp(-Ke*T);
        double firstpart = F * D / V * A;
        double secondpart = R * pow(Gb1/(Gb1 - Ke),Ga1) * boost::math::gamma_p(Ga1,(Gb1 - Ke)*T);
        double thirdpart = (1 - R) * pow(Gb2/(Gb2 - Ke), Ga2) * boost::math::gamma_p(Ga2,(Gb2 - Ke)*T);

        data[i] = residuals.at(0).value + firstpart * (secondpart + thirdpart) * eht;

        T += delta;
    }

    //Recalibrate start time
    T -= delta;

    //Set the new residual
    residuals[0] = Residual(data[points-1]);

    return data;
}

