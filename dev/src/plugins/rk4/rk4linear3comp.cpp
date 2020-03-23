#include "rk4linear3comp.h"

#include "core.h"
#include "core_errors.h"
#include "units.h"
#include "coremessagehandler.h"
#include "stdparamnames.h"
#include "knownmodels.h"
#include "version.h"

#include <math.h>

RK4Linear3CompModel::RK4Linear3CompModel():RK4GenericModel(3)
{
    //We want ug for the dose
    _ug.fromString("ug");
    //And ug/l for the result
    _ugl.fromString("ug/l");    
}

Version RK4Linear3CompModel::version()
{
    return Version("0.1");
}

//Copied on the analytic model
Duration RK4Linear3CompModel::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
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

RK4Linear3CompBolusK::RK4Linear3CompBolusK()
{
    // Parameters
    _parameters.clear();
    _parameters.append(Descriptor("Ke",StdParamNames::getNameKe(),StdParamNames::getDescKe()));
    _parameters.append(Descriptor("V1",StdParamNames::getNameV1(),StdParamNames::getDescV1()));
    _parameters.append(Descriptor("K12",StdParamNames::getNameK12(),StdParamNames::getDescK12()));
    _parameters.append(Descriptor("K21",StdParamNames::getNameK21(),StdParamNames::getDescK21()));
    _parameters.append(Descriptor("K13",StdParamNames::getNameK13(),StdParamNames::getDescK13()));
    _parameters.append(Descriptor("K31",StdParamNames::getNameK31(),StdParamNames::getDescK31()));
}

RK4Linear3CompBolusClV::RK4Linear3CompBolusClV()
{
    // Parameters
    _parameters.clear();
    _parameters.append(Descriptor("CL",StdParamNames::getNameCL(),StdParamNames::getDescCL()));
    _parameters.append(Descriptor("V1",StdParamNames::getNameV1(),StdParamNames::getDescV1()));
    _parameters.append(Descriptor("V2",StdParamNames::getNameV2(),StdParamNames::getDescV2()));
    _parameters.append(Descriptor("V3",StdParamNames::getNameV3(),StdParamNames::getDescV3()));
    _parameters.append(Descriptor("Q2",StdParamNames::getNameQ2(),StdParamNames::getDescQ2()));
    _parameters.append(Descriptor("Q3",StdParamNames::getNameQ3(),StdParamNames::getDescQ3()));
}

void RK4Linear3CompBolusK::yDeriv (double , const QVector<double> &yn, const SharedDosage &, QVector<double> &result)
{
    result[0] = (-K12-K13-Ke)*yn.at(0)+K21*yn.at(1)+K31*yn.at(2);
    result[1] = -K21*yn.at(1)+K12*yn.at(0);
    result[2] = -K31*yn.at(2)+K13*yn.at(0);
}

QVector<double> RK4Linear3CompBolusK::y0 ()
{
    QVector<double> initDosage(3);
    initDosage[0] = D/V1;
    initDosage[1] = 0.0;
    initDosage[2] = 0.0;
    return initDosage;
}

ModelDescriptor RK4Linear3CompBolusK::descriptor()
{
    return ModelDescriptor(RK4_LIN_3C_BOLUS_K_ID,
                           MODEL_LINEAR_3COMP_BOLUS_K_ID,
                           tr("Linear Three-compartment Bolus (RK4)"),
                           tr("Model using the classic Runge-Kutta method, 4th order."),
                           "bolus");
}

ModelDescriptor RK4Linear3CompBolusClV::descriptor()
{
    return ModelDescriptor(RK4_LIN_3C_BOLUS_CLV_ID,
                           MODEL_LINEAR_3COMP_BOLUS_CLV_ID,
                           tr("Linear Three-compartment Bolus (RK4)"),
                           tr("Model using the classic Runge-Kutta method, 4th order."),
                           "bolus");
}

void RK4Linear3CompBolusK::getParameters(const ParameterSet* &parameters)
{
    //Fetch the parameters
    GETPARAMETER(Ke,"Ke");
    GETPARAMETER(V1,"V1");
    GETPARAMETER(K12,"K12");
    GETPARAMETER(K21,"K21");
    GETPARAMETER(K13,"K13");
    GETPARAMETER(K31,"K31");

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);
}

void RK4Linear3CompBolusClV::getParameters(const ParameterSet* &parameters)
{

    //Fetch the parameters
    GETPARAMETER(CL,"CL");
    GETPARAMETER(V1,"V1");
    GETPARAMETER(V2,"V2");
    GETPARAMETER(V3,"V3");
    GETPARAMETER(Q2,"Q2");
    GETPARAMETER(Q3,"Q3");

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);
    Q_ASSERT(V2 != 0.0);
    Q_ASSERT(V3 != 0.0);

    //Calculate the useful constants
    K12 = Q2 / V1;
    K21 = Q2 / V2;
    K13 = Q3 / V1;
    K31 = Q3 / V3;
    Ke = CL / V1;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// Infusion, bi-compartimental model
//////////////////////////////////////////////////////////////////////////////////////////////////
RK4Linear3CompInfK::RK4Linear3CompInfK()
{
    // Parameters
    _parameters.clear();
    _parameters.append(Descriptor("Ke",StdParamNames::getNameKe(),StdParamNames::getDescKe()));
    _parameters.append(Descriptor("V1",StdParamNames::getNameV1(),StdParamNames::getDescV1()));
    _parameters.append(Descriptor("K12",StdParamNames::getNameK12(),StdParamNames::getDescK12()));
    _parameters.append(Descriptor("K21",StdParamNames::getNameK21(),StdParamNames::getDescK21()));
    _parameters.append(Descriptor("K13",StdParamNames::getNameK13(),StdParamNames::getDescK13()));
    _parameters.append(Descriptor("K31",StdParamNames::getNameK31(),StdParamNames::getDescK31()));
}

RK4Linear3CompInfClV::RK4Linear3CompInfClV()
{
    // Parameters
    _parameters.clear();
    _parameters.append(Descriptor("CL",StdParamNames::getNameCL(),StdParamNames::getDescCL()));
    _parameters.append(Descriptor("V1",StdParamNames::getNameV1(),StdParamNames::getDescV1()));
    _parameters.append(Descriptor("V2",StdParamNames::getNameV2(),StdParamNames::getDescV2()));
    _parameters.append(Descriptor("V3",StdParamNames::getNameV3(),StdParamNames::getDescV3()));
    _parameters.append(Descriptor("Q2",StdParamNames::getNameQ2(),StdParamNames::getDescQ2()));
    _parameters.append(Descriptor("Q3",StdParamNames::getNameQ3(),StdParamNames::getDescQ3()));
}

void RK4Linear3CompInfK::yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result)
{
    double tinf = dosage->tinf().toHours();

    //Check for division by 0
    Q_ASSERT(tinf != 0.0);
    Q_ASSERT(V1   != 0.0);

    double K0 = D/tinf;

    if (tn <= tinf){
        result[0] = (-K12-K13-Ke)*yn.at(0)+K21*yn.at(1)+K31*yn.at(2) + K0/V1;
        result[1] = -K21*yn.at(1)+K12*yn.at(0);
        result[2] = -K31*yn.at(1)+K13*yn.at(0);
    }
    else{
        result[0] = (-K12-K13-Ke)*yn.at(0)+K21*yn.at(1)+K31*yn.at(2);
        result[1] = -K21*yn.at(1)+K12*yn.at(0);
        result[2] = -K31*yn.at(1)+K13*yn.at(0);
    }
}

ModelDescriptor RK4Linear3CompInfK::descriptor()
{
    return ModelDescriptor(RK4_LIN_3C_INFUS_K_ID,
                           MODEL_LINEAR_3COMP_INFUS_K_ID,
                           tr("Linear Three-compartment Infusion (RK4)"),
                           tr("Model using the classic Runge-Kutta method, 4th order."),
                           "infu");
}

ModelDescriptor RK4Linear3CompInfClV::descriptor()
{
    return ModelDescriptor(RK4_LIN_3C_INFUS_CLV_ID,
                           MODEL_LINEAR_3COMP_INFUS_CLV_ID,
                           tr("Linear Three-compartment Infusion (RK4)"),
                           tr("Model using the classic Runge-Kutta method, 4th order."),
                           "infu");
}

void RK4Linear3CompInfK::getParameters(const ParameterSet* &parameters)
{
    //Fetch the parameters
    GETPARAMETER(Ke,"Ke");
    GETPARAMETER(V1,"V1");
    GETPARAMETER(K12,"K12");
    GETPARAMETER(K21,"K21");
    GETPARAMETER(K13,"K13");
    GETPARAMETER(K31,"K31");

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);
}

void RK4Linear3CompInfClV::getParameters(const ParameterSet* &parameters)
{

    //Fetch the parameters
    GETPARAMETER(CL,"CL");
    GETPARAMETER(V1,"V1");
    GETPARAMETER(V2,"V2");
    GETPARAMETER(V3,"V3");
    GETPARAMETER(Q2,"Q2");
    GETPARAMETER(Q3,"Q3");

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);
    Q_ASSERT(V2 != 0.0);
    Q_ASSERT(V3 != 0.0);

    //Calculate the useful constants
    K12 = Q2 / V1;
    K21 = Q2 / V2;
    K13 = Q3 / V1;
    K31 = Q3 / V3;
    Ke = CL / V1;
}

DataSet RK4Linear3CompInfK::predict(const Duration &start, const Duration &until, const ParameterSet* &parameters, uint points, const SharedDosage &dosage, Residuals &residuals, bool uniform)
{
    //Convert the start time (T), the infusion end time (Tinf) and the prediction duration (D)
    double Tinf = dosage->tinf().toHours();
    double T = start.toHours();
    double D = until.toHours();

    //Check for division by 0
    Q_ASSERT(D != 0.0);

    //If the infusion end time (Tinf) doesn't occure during the prediction, simply compute
    if (uniform || points < 3 || Tinf <= T || Tinf >= T + D)
        return RK4Linear3CompModel::predict(start, until, parameters, points, dosage, residuals, uniform);

    //Compute the number of points prio and post infusion
    uint prioPoints = ((Tinf - T) * (points - 1) / D) + 1;
    uint postPoints = points - prioPoints;

    //Prepare the prio and post infusion datasets
    DataSet prioInfu(prioPoints, _ugl);
    DataSet postInfu(postPoints, _ugl);

    //Use temporary residuals for the prio infu prediction
    Residuals temp = residuals;

    //Compute the prio and post infusion predictions
    prioInfu = RK4Linear3CompModel::predict(start, dosage->tinf() - start, parameters, prioPoints, dosage, temp, uniform);
    postInfu = RK4Linear3CompModel::predict(dosage->tinf(), until - dosage->tinf() + start, parameters, postPoints, dosage, residuals, uniform);

    //Concatenate the two and return the complete prediction
    return prioInfu + postInfu;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Extravascular administrations, bi-compartimental model
//////////////////////////////////////////////////////////////////////////////////////////////////
RK4Linear3CompExtraK::RK4Linear3CompExtraK()
{
    setNbComp(4);
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

RK4Linear3CompExtraClV::RK4Linear3CompExtraClV()
{
    setNbComp(4);
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

//Do the wanted prediction with the wanted parameters
void RK4Linear3CompExtraK::getParameters(const ParameterSet* &parameters)
{    

    //Fetch the parameters    
    GETPARAMETER(Ke,"Ke");
    GETPARAMETER(V1,"V1");
    GETPARAMETER(K12,"K12");
    GETPARAMETER(K21,"K21");
    GETPARAMETER(K13,"K13");
    GETPARAMETER(K31,"K31");
    GETPARAMETER(Ka,"Ka");
    GETPARAMETER(F,"F");

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);

}

void RK4Linear3CompExtraClV::getParameters(const ParameterSet* &parameters)
{

    //Fetch the parameters
    GETPARAMETER(CL,"CL");
    GETPARAMETER(V1,"V1");
    GETPARAMETER(V2,"V2");
    GETPARAMETER(V3,"V3");
    GETPARAMETER(Q2,"Q2");
    GETPARAMETER(Q3,"Q3");
    GETPARAMETER(Ka,"Ka");
    GETPARAMETER(F,"F");

    //Check for division by 0
    Q_ASSERT(V1 != 0.0);
    Q_ASSERT(V2 != 0.0);
    Q_ASSERT(V3 != 0.0);

    //Calculate the useful constants
    K12 = Q2 / V1;
    K21 = Q2 / V2;
    K13 = Q3 / V1;
    K31 = Q3 / V3;
    Ke = CL / V1;

}


void RK4Linear3CompExtraK::yDeriv (double, const QVector<double> &yn, const SharedDosage &/*dosage*/, QVector<double> &result)
{
    result[0] = (-K12-K13-Ke)*yn.at(0)+K21*yn.at(1)+K31*yn.at(1)+Ka*yn.at(3);
    result[1] = -K21*yn.at(1)+K12*yn.at(0);
    result[2] = -K31*yn.at(2)+K13*yn.at(0);
    result[3] = -Ka*yn.at(3);
}

QVector<double> RK4Linear3CompExtraK::y0 ()
{
    //Check for division by 0
    Q_ASSERT(V1 != 0.0);

    QVector<double> initDosage(4);
    initDosage[0] = 0.0;
    initDosage[1] = 0.0;
    initDosage[2] = 0.0;
    initDosage[3] = F*D/V1;
    return initDosage;
}


ModelDescriptor RK4Linear3CompExtraK::descriptor()
{
    return ModelDescriptor(RK4_LIN_3C_EXTRA_K_ID,
                           MODEL_LINEAR_3COMP_EXTRA_K_ID,
                           tr("Linear Three-compartment Extravascular (RK4)"),
                           tr("Model using the classic Runge-Kutta method, 4th order."),
                           "extra");
}

ModelDescriptor RK4Linear3CompExtraClV::descriptor()
{
    return ModelDescriptor(RK4_LIN_3C_EXTRA_CLV_ID,
                           MODEL_LINEAR_3COMP_EXTRA_CLV_ID,
                           tr("Linear Three-compartment Extravascular (RK4)"),
                           tr("Model using the classic Runge-Kutta method, 4th order."),
                           "extra");
}
