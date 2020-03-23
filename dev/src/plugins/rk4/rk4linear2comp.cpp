#include "rk4linear2comp.h"

#include "core.h"
#include "core_errors.h"
#include "units.h"
#include "coremessagehandler.h"
#include "stdparamnames.h"
#include "knownmodels.h"
#include "version.h"

#include <math.h>

RK4Linear2CompModel::RK4Linear2CompModel():RK4GenericModel(2)
{
    //We want ug for the dose
    _ug.fromString("ug");
    //And ug/l for the result
    _ugl.fromString("ug/l");    
}

Version RK4Linear2CompModel::version()
{
    return Version("0.1");
}

//Copied on the analytic model
Duration RK4Linear2CompModel::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
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


RK4Linear2CompBolusK::RK4Linear2CompBolusK()
{    
    _parameters.clear();
    _parameters.append(Descriptor("Ke",StdParamNames::getNameKe(),StdParamNames::getDescKe()));
    _parameters.append(Descriptor("K12",StdParamNames::getNameK12(),StdParamNames::getDescK12()));
    _parameters.append(Descriptor("V1",StdParamNames::getNameV2(),StdParamNames::getDescV1()));
    _parameters.append(Descriptor("K21",StdParamNames::getNameK21(),StdParamNames::getDescK21()));
}

RK4Linear2CompBolusClV::RK4Linear2CompBolusClV ()
{
    _parameters.clear();
    _parameters.append(Descriptor("CL",StdParamNames::getNameCL(),StdParamNames::getDescCL()));
    _parameters.append(Descriptor("V1",StdParamNames::getNameV1(),StdParamNames::getDescV1()));
    _parameters.append(Descriptor("V2",StdParamNames::getNameV2(),StdParamNames::getDescV2()));
    _parameters.append(Descriptor("Q",StdParamNames::getNameQ(),StdParamNames::getDescQ()));
}

void RK4Linear2CompBolusK::yDeriv (double , const QVector<double> &yn, const SharedDosage &, QVector<double> &result)
{
    result[0] = (-K12-Ke)*yn.at(0)+K21*yn.at(1);
    result[1] = -K21*yn.at(1)+K12*yn.at(0);
}

QVector<double> RK4Linear2CompBolusK::y0 ()
{
    //Check for division by 0
    Q_ASSERT(V1 != 0.0);

    QVector<double> initDosage(2);
    initDosage[0] = D/V1;
    initDosage[1] = 0.0;
    return initDosage;
}

ModelDescriptor RK4Linear2CompBolusK::descriptor()
{
    return ModelDescriptor(RK4_LIN_2C_BOLUS_K_ID,
                           MODEL_LINEAR_2COMP_BOLUS_K_ID,
                           tr("Linear Two-compartement Bolus (RK4)"),
                           tr("Model using the classic Runge-Kutta method, 4th order."),
                           "bolus");
}

ModelDescriptor RK4Linear2CompBolusClV::descriptor()
{
    return ModelDescriptor(RK4_LIN_2C_BOLUS_CLV_ID,
                           MODEL_LINEAR_2COMP_BOLUS_CLV_ID,
                           tr("Linear Two-compartement Bolus (RK4)"),
                           tr("Model using the classic Runge-Kutta method, 4th order."),
                           "bolus");
}

void RK4Linear2CompBolusK::getParameters(const ParameterSet* &parameters)
{
    GETPARAMETER(Ke,"Ke");
    GETPARAMETER(K12,"K12");
    GETPARAMETER(K21,"K21");
    GETPARAMETER(V1,"V1");

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);
}

void RK4Linear2CompBolusClV::getParameters(const ParameterSet* &parameters)
{
    GETPARAMETER(CL,"CL");
    GETPARAMETER(Q,"Q");
    GETPARAMETER(V2,"V2");
    GETPARAMETER(V1,"V1");

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);

    Ke = CL/V1;
    K12 = Q/V1;
    K21 = Q/V2;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Infusion, bi-compartimental model
//////////////////////////////////////////////////////////////////////////////////////////////////
RK4Linear2CompiInfK::RK4Linear2CompiInfK()
{
    _parameters.clear();
    _parameters.append(Descriptor("Ke",StdParamNames::getNameKe(),StdParamNames::getDescKe()));
    _parameters.append(Descriptor("K12",StdParamNames::getNameK12(),StdParamNames::getDescK12()));
    _parameters.append(Descriptor("V1",StdParamNames::getNameV2(),StdParamNames::getDescV1()));
    _parameters.append(Descriptor("K21",StdParamNames::getNameK21(),StdParamNames::getDescK21()));
}

RK4Linear2CompiInfClV::RK4Linear2CompiInfClV ()
{
    _parameters.clear();
    _parameters.append(Descriptor("CL",StdParamNames::getNameCL(),StdParamNames::getDescCL()));
    _parameters.append(Descriptor("V1",StdParamNames::getNameV1(),StdParamNames::getDescV1()));
    _parameters.append(Descriptor("V2",StdParamNames::getNameV2(),StdParamNames::getDescV2()));
    _parameters.append(Descriptor("Q",StdParamNames::getNameQ(),StdParamNames::getDescQ()));
}

void RK4Linear2CompiInfK::yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result)
{
    double tinf = dosage->tinf().toHours();

    //Check for division by 0
    Q_ASSERT(tinf != 0.0);
    Q_ASSERT(V1   != 0.0);

    double K0 = D/tinf;

    if (tn <= tinf){
        result[0] = (-K12-Ke)*yn.at(0)+K21*yn.at(1) + K0/V1;
        result[1] = -K21*yn.at(1)+K12*yn.at(0);
    }
    else{
        result[0] = (-K12-Ke)*yn.at(0)+K21*yn.at(1);
        result[1] = -K21*yn.at(1)+K12*yn.at(0);
    }
}


ModelDescriptor RK4Linear2CompiInfK::descriptor()
{
    return ModelDescriptor(RK4_LIN_2C_INFUS_K_ID,
                           MODEL_LINEAR_2COMP_INFUS_K_ID,
                           tr("Linear Two-compartment Infusion (RK4)"),
                           tr("Model using the classic Runge-Kutta method, 4th order."),
                           "infu");
}

ModelDescriptor RK4Linear2CompiInfClV::descriptor()
{
    return ModelDescriptor(RK4_LIN_2C_INFUS_CLV_ID,
                           MODEL_LINEAR_2COMP_INFUS_CLV_ID,
                           tr("Linear Two-compartment Infusion (RK4)"),
                           tr("Model using the classic Runge-Kutta method, 4th order."),
                           "infu");
}

void RK4Linear2CompiInfK::getParameters(const ParameterSet* &parameters)
{
    GETPARAMETER(Ke,"Ke");
    GETPARAMETER(K12,"K12");
    GETPARAMETER(K21,"K21");
    GETPARAMETER(V1,"V1");

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);
}

void RK4Linear2CompiInfClV::getParameters(const ParameterSet* &parameters)
{
    GETPARAMETER(CL,"CL");
    GETPARAMETER(Q,"Q");
    GETPARAMETER(V2,"V2");
    GETPARAMETER(V1,"V1");

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);

    Ke = CL/V1;
    K12 = Q/V1;
    K21 = Q/V2;
}

DataSet RK4Linear2CompiInfK::predict(const Duration &start, const Duration &until, const ParameterSet* &parameters, uint points, const SharedDosage &dosage, Residuals &residuals, bool uniform)
{
    //Convert the start time (T), the infusion end time (Tinf) and the prediction duration (D)
    double Tinf = dosage->tinf().toHours();
    double T = start.toHours();
    double D = until.toHours();

    //Check for division by 0
    Q_ASSERT(D != 0.0);

    //If the infusion end time (Tinf) doesn't occure during the prediction, simply compute
    if (uniform || points < 3 || Tinf <= T || Tinf >= T + D)
        return RK4Linear2CompModel::predict(start, until, parameters, points, dosage, residuals, uniform);

    //Compute the number of points prio and post infusion
    uint prioPoints = ((Tinf - T) * (points - 1) / D) + 1;
    uint postPoints = points - prioPoints;

    //Prepare the prio and post infusion datasets
    DataSet prioInfu(prioPoints, _ugl);
    DataSet postInfu(postPoints, _ugl);

    //Use temporary residuals for the prio infu prediction
    Residuals temp = residuals;

    //Compute the prio and post infusion predictions
    prioInfu = RK4Linear2CompModel::predict(start, dosage->tinf() - start, parameters, prioPoints, dosage, temp, uniform);
    postInfu = RK4Linear2CompModel::predict(dosage->tinf(), until - dosage->tinf() + start, parameters, postPoints, dosage, residuals, uniform);

    //Concatenate the two and return the complete prediction
    return prioInfu + postInfu;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Extravascular administrations, bi-compartimental model
//////////////////////////////////////////////////////////////////////////////////////////////////
RK4Linear2CompExtraK::RK4Linear2CompExtraK()
{
    setNbComp(3);
    _parameters.clear();
    _parameters.append(Descriptor("Ke",StdParamNames::getNameKe(),StdParamNames::getDescKe()));
    _parameters.append(Descriptor("K12",StdParamNames::getNameK12(),StdParamNames::getDescK12()));
    _parameters.append(Descriptor("V1",StdParamNames::getNameV2(),StdParamNames::getDescV1()));
    _parameters.append(Descriptor("K21",StdParamNames::getNameK21(),StdParamNames::getDescK21()));
    _parameters.append(Descriptor("Ka",StdParamNames::getNameKA(),StdParamNames::getDescKA()));
    _parameters.append(Descriptor("F",StdParamNames::getNameF(),StdParamNames::getDescF()));
}

RK4Linear2CompExtraClV::RK4Linear2CompExtraClV ()
{
    setNbComp(3);
    _parameters.clear();
    _parameters.append(Descriptor("CL",StdParamNames::getNameCL(),StdParamNames::getDescCL()));
    _parameters.append(Descriptor("V1",StdParamNames::getNameV1(),StdParamNames::getDescV1()));
    _parameters.append(Descriptor("V2",StdParamNames::getNameV2(),StdParamNames::getDescV2()));
    _parameters.append(Descriptor("Q",StdParamNames::getNameQ(),StdParamNames::getDescQ()));
    _parameters.append(Descriptor("Ka",StdParamNames::getNameKA(),StdParamNames::getDescKA()));
    _parameters.append(Descriptor("F",StdParamNames::getNameF(),StdParamNames::getDescF()));
}

void RK4Linear2CompExtraK::yDeriv (double /*tn*/, const QVector<double> &yn, const SharedDosage &/*dosage*/, QVector<double> &result)
{
    result[0] = (-K12-Ke)*yn.at(0)+K21*yn.at(1)+Ka*yn.at(2);
    result[1] = -K21*yn.at(1)+K12*yn.at(0);
    result[2] = -Ka*yn.at(2);
}

QVector<double> RK4Linear2CompExtraK::y0 ()
{
    //Check for division by 0
    Q_ASSERT(V1 != 0.0);

    QVector<double> initDosage(3);
    initDosage[0] = 0.0;
    initDosage[1] = 0.0;
    initDosage[2] = F*D/V1;
    return initDosage;
}

ModelDescriptor RK4Linear2CompExtraK::descriptor()
{
    return ModelDescriptor(RK4_LIN_2C_EXTRA_K_ID,
                           MODEL_LINEAR_2COMP_EXTRA_K_ID,
                           tr("Linear Two-compartment Extravascular (RK4)"),
                           tr("Model using the classic Runge-Kutta method, 4th order."),
                           "extra");
}

ModelDescriptor RK4Linear2CompExtraClV::descriptor()
{
    return ModelDescriptor(RK4_LIN_2C_EXTRA_CLV_ID,
                           MODEL_LINEAR_2COMP_EXTRA_CLV_ID,
                           tr("Linear Two-compartment Extravascular (RK4)"),
                           tr("Model using the classic Runge-Kutta method, 4th order."),
                           "extra");
}

//Do the wanted prediction with the wanted parameters
void RK4Linear2CompExtraK::getParameters(const ParameterSet* &parameters)
{
    //Fetch the parameters

    GETPARAMETER(Ke,"Ke");
    GETPARAMETER(K12,"K12");
    GETPARAMETER(K21,"K21");
    GETPARAMETER(V1,"V1");
    GETPARAMETER(F,"F");
    GETPARAMETER(Ka,"Ka");

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);
}

void RK4Linear2CompExtraClV::getParameters(const ParameterSet* &parameters)
{
    GETPARAMETER(CL,"CL");
    GETPARAMETER(Q,"Q");
    GETPARAMETER(V2,"V2");
    GETPARAMETER(V1,"V1");
    GETPARAMETER(F,"F");
    GETPARAMETER(Ka,"Ka");

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);

    Ke = CL/V1;
    K12 = Q/V1;
    K21 = Q/V2;
}


