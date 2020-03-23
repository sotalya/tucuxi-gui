#include "core.h"
#include "core_errors.h"
#include "units.h"
#include "coremessagehandler.h"
#include "stdparamnames.h"
#include "knownmodels.h"
#include "version.h"

#include "rk4michaelismenten2comp.h"

#include <math.h>

RK4MichaelisMenten2CompModel::RK4MichaelisMenten2CompModel():RK4GenericModel(2)
{
    //We want ug for the dose
    _ug.fromString("ug");
    //And ug/l for the result
    _ugl.fromString("ug/l");

}

Version RK4MichaelisMenten2CompModel::version()
{
    return Version("0.1");
}

//Copied on the analytic model
Duration RK4MichaelisMenten2CompModel::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
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


RK4MichaelisMenten2CompBolusK::RK4MichaelisMenten2CompBolusK()
{
    // Parameters
    _parameters.clear();
    _parameters.append(Descriptor("Vm",StdParamNames::getNameVM(),StdParamNames::getDescVM()));
    _parameters.append(Descriptor("Km",StdParamNames::getNameKM(),StdParamNames::getDescKM()));
    _parameters.append(Descriptor("V1",StdParamNames::getNameV1(),StdParamNames::getDescV1()));
    _parameters.append(Descriptor("K12",StdParamNames::getNameK12(),StdParamNames::getDescK12()));
    _parameters.append(Descriptor("K21",StdParamNames::getNameK21(),StdParamNames::getDescK21()));
}

RK4MichaelisMenten2CompBolusClV::RK4MichaelisMenten2CompBolusClV()
{
    // Parameters
    _parameters.clear();
    _parameters.append(Descriptor("Vm",StdParamNames::getNameVM(),StdParamNames::getDescVM()));
    _parameters.append(Descriptor("Km",StdParamNames::getNameKM(),StdParamNames::getDescKM()));
    _parameters.append(Descriptor("V1",StdParamNames::getNameV1(),StdParamNames::getDescV1()));
    _parameters.append(Descriptor("V2",StdParamNames::getNameV2(),StdParamNames::getDescV2()));
    _parameters.append(Descriptor("Q",StdParamNames::getNameQ(),StdParamNames::getDescQ()));
}

void RK4MichaelisMenten2CompBolusK::yDeriv (double , const QVector<double> &yn, const SharedDosage &, QVector<double> &result)
{
    //Check for division by 0
    Q_ASSERT(V1 != 0.0);

    result[0] = -Vm/V1*yn.at(0)/(Km+yn.at(0))-K12*yn.at(0)+K21*yn.at(1);
    result[1] = -K21*yn.at(0)-K12*yn.at(1);

    //Check for result validity
    Q_ASSERT(!isnan(result[0]));
    Q_ASSERT(!isinf(result[0]));
    Q_ASSERT(!isnan(result[1]));
    Q_ASSERT(!isinf(result[1]));
}

QVector<double> RK4MichaelisMenten2CompBolusK::y0 ()
{
    //Check for division by 0
    Q_ASSERT(V1 != 0.0);

    QVector<double> initDosage(2);
    initDosage[0] = D/V1;
    initDosage[1] = 0.0;
    return initDosage;
}

void RK4MichaelisMenten2CompBolusK::getParameters(const ParameterSet* &parameters)
{
    //Fetch the parameters
    GETPARAMETER(V1,"V1");
    GETPARAMETER(K21,"K21");
    GETPARAMETER(K12,"K12");
    GETPARAMETER(Vm,"Vm");
    GETPARAMETER(Km,"Km");

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);
}

void RK4MichaelisMenten2CompBolusClV::getParameters(const ParameterSet* &parameters)
{
    //Fetch the parameters
    GETPARAMETER(V1,"V1");
    GETPARAMETER(V2,"V2");
    GETPARAMETER(Q,"Q");
    GETPARAMETER(Vm,"Vm");
    GETPARAMETER(Km,"Km");

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);
    Q_ASSERT(V2 != 0.0);

    K12 = Q/V1;
    K21 = Q/V2;
}

ModelDescriptor RK4MichaelisMenten2CompBolusK::descriptor()
{
    return ModelDescriptor(RK4_MM_2C_BOLUS_K_ID,
                           MODEL_MM_2COMP_BOLUS_K_ID,
                           tr("Michaelis-Menten Two-compartment Bolus (RK4)"),
                           tr("Model using the classic Runge-Kutta method, 4th order."),
                           "bolus");
}

ModelDescriptor RK4MichaelisMenten2CompBolusClV::descriptor()
{
    return ModelDescriptor(RK4_MM_2C_BOLUS_CLV_ID,
                           MODEL_MM_2COMP_BOLUS_CLV_ID,
                           tr("Michaelis-Menten Two-compartment Bolus (RK4)"),
                           tr("Model using the classic Runge-Kutta method, 4th order."),
                           "bolus");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Infusion, bi-compartimental model
//////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief RK4MichaelisMenten2CompInfClV::RK4MichaelisMenten2CompInfClV
RK4MichaelisMenten2CompInfK::RK4MichaelisMenten2CompInfK()
{
    _parameters.clear();
    _parameters.append(Descriptor("Vm",StdParamNames::getNameVM(),StdParamNames::getDescVM()));
    _parameters.append(Descriptor("Km",StdParamNames::getNameKM(),StdParamNames::getDescKM()));
    _parameters.append(Descriptor("V1",StdParamNames::getNameV1(),StdParamNames::getDescV1()));
    _parameters.append(Descriptor("K12",StdParamNames::getNameK12(),StdParamNames::getDescK12()));
    _parameters.append(Descriptor("K21",StdParamNames::getNameK21(),StdParamNames::getDescK21()));
}

RK4MichaelisMenten2CompInfClV::RK4MichaelisMenten2CompInfClV()
{
    // Parameters
    _parameters.clear();
    _parameters.append(Descriptor("Vm",StdParamNames::getNameVM(),StdParamNames::getDescVM()));
    _parameters.append(Descriptor("Km",StdParamNames::getNameKM(),StdParamNames::getDescKM()));
    _parameters.append(Descriptor("V1",StdParamNames::getNameV1(),StdParamNames::getDescV1()));
    _parameters.append(Descriptor("V2",StdParamNames::getNameV2(),StdParamNames::getDescV2()));
    _parameters.append(Descriptor("Q",StdParamNames::getNameQ(),StdParamNames::getDescQ()));
}

void RK4MichaelisMenten2CompInfK::yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result)
{
    double tinf = dosage->tinf().toHours();

    //Check for division by 0
    Q_ASSERT(tinf != 0.0);
    Q_ASSERT(V1   != 0.0);

    double K0 = D/tinf;

    if (tn <= tinf){
        result[0] = -Vm/V1*yn.at(0)/(Km+yn.at(0))-K12*yn.at(0)+K21*yn.at(1)+K0/V1;
        result[1] = -K21*yn.at(0)-K12*yn.at(1);
    }
    else{
        result[0] = -Vm/V1*yn.at(0)/(Km+yn.at(0))-K12*yn.at(0)+K21*yn.at(1);
        result[1] = -K21*yn.at(0)-K12*yn.at(1);
    }

    //Check for result validity
    Q_ASSERT(!isnan(result[0]));
    Q_ASSERT(!isinf(result[0]));
    Q_ASSERT(!isnan(result[1]));
    Q_ASSERT(!isinf(result[1]));
}

QVector<double> RK4MichaelisMenten2CompInfK::y0 ()
{
    QVector<double> initDosage(2);
    initDosage[0] = 0.0;
    initDosage[1] = 0.0;
    return initDosage;
}

ModelDescriptor RK4MichaelisMenten2CompInfK::descriptor()
{
    return ModelDescriptor(RK4_MM_2C_INFUS_K_ID,
                           MODEL_MM_2COMP_INFUS_K_ID,
                           tr("Michaelis-Menten Two-compartment Infusion (RK4)"),
                           tr("Model using the classic Runge-Kutta method, 4th order."),
                           "infu");
}

ModelDescriptor RK4MichaelisMenten2CompInfClV::descriptor()
{
    return ModelDescriptor(RK4_MM_2C_INFUS_CLV_ID,
                           MODEL_MM_2COMP_INFUS_CLV_ID,
                           tr("Michaelis-Menten Two-compartment Infusion (RK4)"),
                           tr("Model using the classic Runge-Kutta method, 4th order."),
                           "infu");
}

void RK4MichaelisMenten2CompInfK::getParameters(const ParameterSet* &parameters)
{
    //Fetch the parameters
    GETPARAMETER(V1,"V1");
    GETPARAMETER(K21,"K21");
    GETPARAMETER(K12,"K12");
    GETPARAMETER(Vm,"Vm");
    GETPARAMETER(Km,"Km");

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);
}

void RK4MichaelisMenten2CompInfClV::getParameters(const ParameterSet* &parameters)
{
    //Fetch the parameters
    GETPARAMETER(V1,"V1");
    GETPARAMETER(V2,"V2");
    GETPARAMETER(Q,"Q");
    GETPARAMETER(Vm,"Vm");
    GETPARAMETER(Km,"Km");

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);
    Q_ASSERT(V2 != 0.0);

    K12 = Q/V1;
    K21 = Q/V2;
}

DataSet RK4MichaelisMenten2CompInfK::predict(const Duration &start, const Duration &until, const ParameterSet* &parameters, uint points, const SharedDosage &dosage, Residuals &residuals, bool uniform)
{
    //Convert the start time (T), the infusion end time (Tinf) and the prediction duration (D)
    double Tinf = dosage->tinf().toHours();
    double T = start.toHours();
    double D = until.toHours();

    //Check for division by 0
    Q_ASSERT(D != 0.0);

    //If the infusion end time (Tinf) doesn't occure during the prediction, simply compute
    if (uniform || points < 3 || Tinf <= T || Tinf >= T + D)
        return RK4MichaelisMenten2CompModel::predict(start, until, parameters, points, dosage, residuals, uniform);

    //Compute the number of points prio and post infusion
    uint prioPoints = ((Tinf - T) * (points - 1) / D) + 1;
    uint postPoints = points - prioPoints;

    //Prepare the prio and post infusion datasets
    DataSet prioInfu(prioPoints, _ugl);
    DataSet postInfu(postPoints, _ugl);

    //Use temporary residuals for the prio infu prediction
    Residuals temp = residuals;

    //Compute the prio and post infusion predictions
    prioInfu = RK4MichaelisMenten2CompModel::predict(start, dosage->tinf() - start, parameters, prioPoints, dosage, temp, uniform);
    postInfu = RK4MichaelisMenten2CompModel::predict(dosage->tinf(), until - dosage->tinf() + start, parameters, postPoints, dosage, residuals, uniform);

    //Concatenate the two and return the complete prediction
    return prioInfu + postInfu;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Extravascular administrations, bi-compartimental model
//////////////////////////////////////////////////////////////////////////////////////////////////
RK4MichaelisMenten2CompExtraK::RK4MichaelisMenten2CompExtraK()
{
    setNbComp(3);
    _parameters.clear();
    _parameters.append(Descriptor("Vm",StdParamNames::getNameVM(),StdParamNames::getDescVM()));
    _parameters.append(Descriptor("Km",StdParamNames::getNameKM(),StdParamNames::getDescKM()));
    _parameters.append(Descriptor("V1",StdParamNames::getNameV1(),StdParamNames::getDescV1()));
    _parameters.append(Descriptor("K12",StdParamNames::getNameK12(),StdParamNames::getDescK12()));
    _parameters.append(Descriptor("K21",StdParamNames::getNameK21(),StdParamNames::getDescK21()));
    _parameters.append(Descriptor("Ka",StdParamNames::getNameKA(),StdParamNames::getDescKA()));
    _parameters.append(Descriptor("F",StdParamNames::getNameF(),StdParamNames::getDescF()));
}

RK4MichaelisMenten2CompExtraClV::RK4MichaelisMenten2CompExtraClV()
{
    setNbComp(3);
    _parameters.clear();
    _parameters.append(Descriptor("Vm",StdParamNames::getNameVM(),StdParamNames::getDescVM()));
    _parameters.append(Descriptor("Km",StdParamNames::getNameKM(),StdParamNames::getDescKM()));
    _parameters.append(Descriptor("V1",StdParamNames::getNameV1(),StdParamNames::getDescV1()));
    _parameters.append(Descriptor("V2",StdParamNames::getNameV2(),StdParamNames::getDescV2()));
    _parameters.append(Descriptor("Q",StdParamNames::getNameQ(),StdParamNames::getDescQ()));
    _parameters.append(Descriptor("Ka",StdParamNames::getNameKA(),StdParamNames::getDescKA()));
    _parameters.append(Descriptor("F",StdParamNames::getNameF(),StdParamNames::getDescF()));
}

void RK4MichaelisMenten2CompExtraK::yDeriv (double, const QVector<double> &yn, const SharedDosage &/*dosage*/, QVector<double> &result)
{
    //Check for division by 0
    Q_ASSERT(V1 != 0.0);

    result[0] = -Vm/V1*yn.at(0)/(Km+yn.at(0))-K12*yn.at(0)+K21*yn.at(1)+Ka*yn.at(2);
    result[1] = -K21*yn.at(0)-K12*yn.at(1);
    result[2] = -Ka*yn.at(2);

    //Check for result validity
    Q_ASSERT(!isnan(result[0]));
    Q_ASSERT(!isinf(result[0]));
    Q_ASSERT(!isnan(result[1]));
    Q_ASSERT(!isinf(result[1]));
    Q_ASSERT(!isnan(result[2]));
    Q_ASSERT(!isinf(result[2]));
}

QVector<double> RK4MichaelisMenten2CompExtraK::y0 ()
{
    //Check for division by 0
    Q_ASSERT(V1 != 0.0);

    QVector<double> initDosage(3);
    initDosage[0] = 0.0;
    initDosage[1] = 0.0;
    initDosage[2] = F*D/V1;
    return initDosage;
}

ModelDescriptor RK4MichaelisMenten2CompExtraK::descriptor()
{
    return ModelDescriptor(RK4_MM_2C_EXTRA_K_ID,
                           MODEL_MM_2COMP_EXTRA_K_ID,
                           tr("Michaelis-Menten Two-compartment Extravascular (RK4)"),
                           tr("Model using the Michaelis-Menten Two-compartment Extravascular with the classic Runge-Kutta method, 4th order."),
                           "extra");
}

ModelDescriptor RK4MichaelisMenten2CompExtraClV::descriptor()
{
    return ModelDescriptor(RK4_MM_2C_EXTRA_CLV_ID,
                           MODEL_MM_2COMP_EXTRA_CLV_ID,
                           tr("Michaelis-Menten Two-compartment Extravascular (RK4)"),
                           tr("Model using the Michaelis-Menten Two-compartment Extravascular with the classic Runge-Kutta method, 4th order."),
                           "extra");
}

void RK4MichaelisMenten2CompExtraK::getParameters(const ParameterSet* &parameters)
{    
    //Fetch the parameters
    GETPARAMETER(V1,"V1");
    GETPARAMETER(K21,"K21");
    GETPARAMETER(K12,"K12");
    GETPARAMETER(Vm,"Vm");
    GETPARAMETER(Km,"Km");

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);
    GETPARAMETER(Ka,"Ka");
    GETPARAMETER(F,"F");
}

void RK4MichaelisMenten2CompExtraClV::getParameters(const ParameterSet* &parameters)
{
    //Fetch the parameters
    GETPARAMETER(V1,"V1");
    GETPARAMETER(V2,"V2");
    GETPARAMETER(Q,"Q");
    GETPARAMETER(Vm,"Vm");
    GETPARAMETER(Km,"Km");

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);
    Q_ASSERT(V2 != 0.0);

    GETPARAMETER(Ka,"Ka");
    GETPARAMETER(F,"F");

    K12 = Q/V1;
    K21 = Q/V2;
}
