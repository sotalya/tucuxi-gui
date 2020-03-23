#include "rk4michaelismenten1comp.h"

#include "core.h"
#include "core_errors.h"
#include "units.h"
#include "coremessagehandler.h"
#include "stdparamnames.h"
#include "knownmodels.h"
#include "version.h"

#include <math.h>


RK4MichaelisMenten1CompModel::RK4MichaelisMenten1CompModel (): RK4GenericModel(1)
{
    //We want ug for the dose
    _ug.fromString("g");
    //And ug/l for the result
    _ugl.fromString("g/l");

    // Parameters
    _parameters.append(Descriptor("Vm",StdParamNames::getNameVM(),StdParamNames::getDescVM()));
    _parameters.append(Descriptor("Km",StdParamNames::getNameKM(),StdParamNames::getDescKM()));
    _parameters.append(Descriptor("V",StdParamNames::getNameV(),StdParamNames::getDescV()));
}

//Do the wanted prediction with the wanted parameters
void RK4MichaelisMenten1CompModel::getParameters(const ParameterSet* &parameters)
{
    //Fetch the parameters
    GETPARAMETER(V,"V");
    GETPARAMETER(Vm,"Vm");
    GETPARAMETER(Km,"Km");
}


Version RK4MichaelisMenten1CompModel::version()
{
    return Version("0.1");
}

//Copied on the analytic model
Duration RK4MichaelisMenten1CompModel::halflife(const Duration &, const ParameterSet* &/*parameters*/, const SharedDosage &, const Residuals &)
{
    // No clue
    return 1.0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// Bolus, mono-compartimental model
//////////////////////////////////////////////////////////////////////////////////////////////////
RK4MichaelisMenten1CompBolus::RK4MichaelisMenten1CompBolus ()
{
}


void RK4MichaelisMenten1CompBolus::yDeriv (double, const QVector<double> &yn, const SharedDosage &, QVector<double> &result)
{
    result[0] = -Vm*yn.at(0)/(Km+yn.at(0));

    //Check for result validity
    Q_ASSERT(!isnan(result[0]));
    Q_ASSERT(!isinf(result[0]));
}

QVector<double> RK4MichaelisMenten1CompBolus::y0 ()
{
    //Check for division by 0
    Q_ASSERT(V != 0.0);

    QVector<double> initDosage(1);
    initDosage[0] = D/V;
    return initDosage;
}

ModelDescriptor RK4MichaelisMenten1CompBolus::descriptor()
{
    return ModelDescriptor(RK4_MM_1C_BOLUS_ID,
                           MODEL_MM_1COMP_BOLUS_ID,
                           tr("Michaelis-Menten One-compartment Bolus (RK4)"),
                           tr("Model using the classic Michaelis-Menten Runge-Kutta method, 4th order."),
                           "bolus");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Infusion, mono-compartimental model
//////////////////////////////////////////////////////////////////////////////////////////////////
RK4MichaelisMenten1CompInf::RK4MichaelisMenten1CompInf ()
{
}

void RK4MichaelisMenten1CompInf::yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result)
{
    double tinf = dosage->tinf().toHours();

    //Check for division by 0
    Q_ASSERT(tinf != 0.0);
    Q_ASSERT(V    != 0.0);

    double K0 = D/tinf;
    double input;

    if (tn <= tinf)
        input = K0/V;
    else
        input = 0.0;

    result[0] = -Vm*yn.at(0)/(Km+yn.at(0)) + input;

    //Check for result validity
    Q_ASSERT(!isnan(result[0]));
    Q_ASSERT(!isinf(result[0]));
}

ModelDescriptor RK4MichaelisMenten1CompInf::descriptor()
{
    return ModelDescriptor(RK4_MM_1C_INFUS_ID,
                           MODEL_MM_1COMP_INFUS_ID,
                           tr("Michaelis-Menten One-compartment Infusion (RK4)"),
                           tr("Model using the Michaelis-Menten Runge-Kutta method, 4th order."),
                           "infu");
}

DataSet RK4MichaelisMenten1CompInf::predict(const Duration &start, const Duration &until, const ParameterSet* &parameters, uint points, const SharedDosage &dosage, Residuals &residuals, bool uniform)
{
    //Convert the start time (T), the infusion end time (Tinf) and the prediction duration (D)
    double Tinf = dosage->tinf().toHours();
    double T = start.toHours();
    double D = until.toHours();

    //Check for division by 0
    Q_ASSERT(D != 0.0);

    //If the infusion end time (Tinf) doesn't occure during the prediction, simply compute
    if (uniform || points < 3 || Tinf <= T || Tinf >= T + D)
        return RK4MichaelisMenten1CompModel::predict(start, until, parameters, points, dosage, residuals, uniform);

    //Compute the number of points prio and post infusion
    uint prioPoints = ((Tinf - T) * (points - 1) / D) + 1;
    uint postPoints = points - prioPoints;

    //Prepare the prio and post infusion datasets
    DataSet prioInfu(prioPoints, _ugl);
    DataSet postInfu(postPoints, _ugl);

    //Use temporary residuals for the prio infu prediction
    Residuals temp = residuals;

    //Compute the prio and post infusion predictions
    prioInfu = RK4MichaelisMenten1CompModel::predict(start, dosage->tinf() - start, parameters, prioPoints, dosage, temp, uniform);
    postInfu = RK4MichaelisMenten1CompModel::predict(dosage->tinf(), until - dosage->tinf() + start, parameters, postPoints, dosage, residuals, uniform);

    //Concatenate the two and return the complete prediction
    return prioInfu + postInfu;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Extravascular administrations with constant absorption, for alcohol, mono-compartimental model
//////////////////////////////////////////////////////////////////////////////////////////////////
RK4MichaelisMenten1CompExtraInf::RK4MichaelisMenten1CompExtraInf ()
{
    setNbComp(2);
    // Parameters
    _parameters.append(Descriptor("Ka",StdParamNames::getNameKA(),StdParamNames::getDescKA()));
    _parameters.append(Descriptor("F",StdParamNames::getNameF(),StdParamNames::getDescF()));
}


DataSet RK4MichaelisMenten1CompExtraInf::predict(const Duration &start, const Duration &until, const ParameterSet* &parameters, uint points, const SharedDosage &dosage, Residuals &residuals, bool uniform)
{
    //Convert the start time (T), the infusion end time (Tinf) and the prediction duration (D)
    double Tinf = dosage->tinf().toHours();
    double T = start.toHours();
    double D = until.toHours();

    //Check for division by 0
    Q_ASSERT(D != 0.0);

    //If the infusion end time (Tinf) doesn't occure during the prediction, simply compute
    if (uniform || points < 3 || Tinf <= T || Tinf >= T + D)
        return RK4MichaelisMenten1CompModel::predict(start, until, parameters, points, dosage, residuals, uniform);

    //Compute the number of points prio and post infusion
    uint prioPoints = ((Tinf - T) * (points - 1) / D) + 1;
    uint postPoints = points - prioPoints;

    //Prepare the prio and post infusion datasets
    DataSet prioInfu(prioPoints, _ugl);
    DataSet postInfu(postPoints, _ugl);

    //Use temporary residuals for the prio infu prediction
    Residuals temp = residuals;

    //Compute the prio and post infusion predictions
    prioInfu = RK4MichaelisMenten1CompModel::predict(start, dosage->tinf() - start, parameters, prioPoints, dosage, temp, uniform);
    postInfu = RK4MichaelisMenten1CompModel::predict(dosage->tinf(), until - dosage->tinf() + start, parameters, postPoints, dosage, residuals, uniform);

    //Concatenate the two and return the complete prediction
    return prioInfu + postInfu;
}


void RK4MichaelisMenten1CompExtraInf::yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result)
{

    double tinf = dosage->tinf().toHours();

    //Check for division by 0
    Q_ASSERT(tinf != 0.0);
    Q_ASSERT(V    != 0.0);

    double K0 = (F*D)/(V*tinf);
    double input;

    if (tn <= tinf)
        input = K0;
    else
        input = 0.0;


    //Check for division by 0
    Q_ASSERT(V != 0.0);

    result[0] = -Vm/V*yn.at(0)/(Km+yn.at(0)) + Ka*yn.at(1);
    result[1] = -Ka*yn.at(1) + input;

    //Check for result validity
    Q_ASSERT(!isnan(result[0]));
    Q_ASSERT(!isinf(result[0]));
    Q_ASSERT(!isnan(result[1]));
    Q_ASSERT(!isinf(result[1]));
}

QVector<double> RK4MichaelisMenten1CompExtraInf::y0 ()
{
    //Check for division by 0
    Q_ASSERT(V != 0.0);

    QVector<double> initDosage(2);
    initDosage[0] = 0.0;
    initDosage[1] = 0.0;
    return initDosage;
}

ModelDescriptor RK4MichaelisMenten1CompExtraInf::descriptor()
{
    return ModelDescriptor(RK4_MM_1C_EXTRAINF_ID,
                           MODEL_MM_1COMP_EXTRAINF_ID,
                           tr("Michaelis-Menten One-compartment Extravascular constant (RK4)"),
                           tr("Model using the Michaelis-Menten Runge-Kutta method, 4th order."),
                           "infu");
}

//Do the wanted prediction with the wanted parameters
void RK4MichaelisMenten1CompExtraInf::getParameters(const ParameterSet* &parameters)
{
    RK4MichaelisMenten1CompModel::getParameters(parameters);
    //Fetch the parameters
    GETPARAMETER(Ka,"Ka");
    GETPARAMETER(F,"F");
}





//////////////////////////////////////////////////////////////////////////////////////////////////
// Extravascular administrations, mono-compartimental model
//////////////////////////////////////////////////////////////////////////////////////////////////
RK4MichaelisMenten1CompExtra::RK4MichaelisMenten1CompExtra ()
{
    setNbComp(2);
    // Parameters
    _parameters.append(Descriptor("Ka",StdParamNames::getNameKA(),StdParamNames::getDescKA()));
    _parameters.append(Descriptor("F",StdParamNames::getNameF(),StdParamNames::getDescF()));
}

void RK4MichaelisMenten1CompExtra::yDeriv (double, const QVector<double> &yn, const SharedDosage &, QVector<double> &result)
{
    //Check for division by 0
    Q_ASSERT(V != 0.0);

    result[0] = -Vm/V*yn.at(0)/(Km+yn.at(0)) + Ka*yn.at(1);
    result[1] = -Ka*yn.at(1);

    //Check for result validity
    Q_ASSERT(!isnan(result[0]));
    Q_ASSERT(!isinf(result[0]));
    Q_ASSERT(!isnan(result[1]));
    Q_ASSERT(!isinf(result[1]));
}

QVector<double> RK4MichaelisMenten1CompExtra::y0 ()
{
    //Check for division by 0
    Q_ASSERT(V != 0.0);

    QVector<double> initDosage(2);
    initDosage[0] = 0.0;
    initDosage[1] = F*D/V;
    //initDosage[1] = F*D;
    return initDosage;
}

ModelDescriptor RK4MichaelisMenten1CompExtra::descriptor()
{
    return ModelDescriptor(RK4_MM_1C_EXTRA_ID,
                           MODEL_MM_1COMP_EXTRA_ID,
                           tr("Michaelis-Menten One-compartment Extravascular (RK4)"),
                           tr("Model using the Michaelis-Menten Runge-Kutta method, 4th order."),
                           "extra");
}

//Do the wanted prediction with the wanted parameters
void RK4MichaelisMenten1CompExtra::getParameters(const ParameterSet* &parameters)
{
    RK4MichaelisMenten1CompModel::getParameters(parameters);
    //Fetch the parameters
    GETPARAMETER(Ka,"Ka");
    GETPARAMETER(F,"F");
}





//////////////////////////////////////////////////////////////////////////////////////////////////
// Extravascular administrations, mono-compartimental model
//////////////////////////////////////////////////////////////////////////////////////////////////
RK4MichaelisMenten1CompExtraVmOverV::RK4MichaelisMenten1CompExtraVmOverV () : RK4MichaelisMenten1CompExtra()
{
}

void RK4MichaelisMenten1CompExtraVmOverV::yDeriv (double, const QVector<double> &yn, const SharedDosage &, QVector<double> &result)
{
    //Check for division by 0
    Q_ASSERT(V != 0.0);

    result[0] = -Vm/V*yn.at(0)/(Km+yn.at(0)) + Ka*yn.at(1);
    result[1] = -Ka*yn.at(1);

    //Check for result validity
    Q_ASSERT(!isnan(result[0]));
    Q_ASSERT(!isinf(result[0]));
    Q_ASSERT(!isnan(result[1]));
    Q_ASSERT(!isinf(result[1]));
}


ModelDescriptor RK4MichaelisMenten1CompExtraVmOverV::descriptor()
{
    return ModelDescriptor(RK4_MM_1C_EXTRA_VMV_ID,
                           MODEL_MM_1COMP_EXTRA_VMOVERV_ID,
                           tr("Michaelis-Menten One-compartment Extravascular (RK4)"),
                           tr("Model using the Michaelis-Menten Runge-Kutta method, 4th order."),
                           "extra");
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// Extravascular administrations, mono-compartimental model
//////////////////////////////////////////////////////////////////////////////////////////////////
RK4MichaelisMenten1CompInfVmOverV::RK4MichaelisMenten1CompInfVmOverV () : RK4MichaelisMenten1CompInf()
{
}

void RK4MichaelisMenten1CompInfVmOverV::yDeriv (double tn, const QVector<double> &yn, const SharedDosage &dosage, QVector<double> &result)
{
    double tinf = dosage->tinf().toHours();

    //Check for division by 0
    Q_ASSERT(tinf != 0.0);
    Q_ASSERT(V    != 0.0);

    double K0 = D/tinf;
    double input;

    if (tn <= tinf)
        input = K0/V;
    else
        input = 0.0;

    result[0] = -Vm/V*yn.at(0)/(Km+yn.at(0)) + input;

    //Check for result validity
    Q_ASSERT(!isnan(result[0]));
    Q_ASSERT(!isinf(result[0]));
}


ModelDescriptor RK4MichaelisMenten1CompInfVmOverV::descriptor()
{
    return ModelDescriptor(RK4_MM_1C_INF_VMV_ID,
                           MODEL_MM_1COMP_INFUS_VMOVERV_ID,
                           tr("Michaelis-Menten One-compartment Extravascular (RK4)"),
                           tr("Model using the Michaelis-Menten Runge-Kutta method, 4th order."),
                           "extra");
}



//////////////////////////////////////////////////////////////////////////////////////////////////
// Extravascular administrations, mono-compartimental model
//////////////////////////////////////////////////////////////////////////////////////////////////
RK4MichaelisMenten1CompBolusVmOverV::RK4MichaelisMenten1CompBolusVmOverV () : RK4MichaelisMenten1CompBolus()
{
}

void RK4MichaelisMenten1CompBolusVmOverV::yDeriv (double, const QVector<double> &yn, const SharedDosage &, QVector<double> &result)
{
    result[0] = -Vm/V*yn.at(0)/(Km+yn.at(0));

    //Check for result validity
    Q_ASSERT(!isnan(result[0]));
    Q_ASSERT(!isinf(result[0]));
}


ModelDescriptor RK4MichaelisMenten1CompBolusVmOverV::descriptor()
{
    return ModelDescriptor(RK4_MM_1C_BOLUS_VMV_ID,
                           MODEL_MM_1COMP_BOLUS_VMOVERV_ID,
                           tr("Michaelis-Menten One-compartment Extravascular (RK4)"),
                           tr("Model using the Michaelis-Menten Runge-Kutta method, 4th order."),
                           "extra");
}

