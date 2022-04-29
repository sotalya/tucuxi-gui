//@@license@@

#include "analyticlineargenericinf.h"

AnalyticLinearGenericInf::AnalyticLinearGenericInf()
{
    _priority = 10;

    //Default unit for the dose
    _ug.fromString("ug");

    //Default unit for the results
    _ugl.fromString("ug/l");
}

DataSet AnalyticLinearGenericInf::predict(const Duration &start, const Duration &until, const ParameterSet* &parameters, uint points, const SharedDosage &dosage, Residuals &residuals, bool uniform)
{
    //Convert the start time (T), the infusion end time (Tinf) and the prediction duration (D)
    double Tinf = dosage->tinf().toHours();
    double T = start.toHours();
    double D = until.toHours();

    //Check for division by 0
    Q_ASSERT(D != 0.0);

    //If the infusion end time (Tinf) doesn't occure during the prediction, simply compute
    if (uniform || points < 3 || Tinf <= T || Tinf >= T + D)
        return compute(start, until, parameters, points, dosage, residuals);

    //Compute the number of points prio and post infusion
    uint prioPoints = ((Tinf - T) * (points - 1) / D) + 1;
    uint postPoints = points - prioPoints;

    //Prepare the prio and post infusion datasets
    DataSet prioInfu(prioPoints, _ugl);
    DataSet postInfu(postPoints, _ugl);

    //Use temporary residuals for the prio infu prediction
    Residuals temp = residuals;

    //Compute the prio and post infusion predictions
    prioInfu = compute(start, dosage->tinf() - start, parameters, prioPoints, dosage, temp);
    postInfu = compute(dosage->tinf(), until - dosage->tinf() + start, parameters, postPoints, dosage, residuals);

    //Concatenate the two and return the complete prediction
    return prioInfu + postInfu;
}

DataSet AnalyticLinearGenericInf::steadystateImpl(const Duration &start, const Duration &duration, const ParameterSet* &parameters, const SharedDosage &dosage, const Duration &halflife, int multiplier, uint points, Residuals &residual, bool uniform, bool computeResidual)
{
    Q_UNUSED(halflife);
    Q_UNUSED(multiplier);

    //Convert the start time (T), the infusion end time (Tinf) and the prediction duration (D)
    double Tinf = dosage->tinf().toHours();
    double T = start.toHours();
    double D = duration.toHours();

    //Check for division by 0
    Q_ASSERT(D != 0.0);

    //If the infusion end time (Tinf) doesn't occure during the prediction, simply compute
    if (uniform || points < 3 || Tinf <= T || Tinf >= T + D)
        return computeSteadyState(start, duration, parameters, dosage, points, residual, computeResidual);

    //Compute the number of points prio and post infusion
    uint prioPoints = ((Tinf - T) * (points - 1) / D) + 1;
    uint postPoints = points - prioPoints;

    //Prepare the prio and post infusion datasets
    DataSet prioInfu(prioPoints, _ugl);
    DataSet postInfu(postPoints, _ugl);

    //Use temporary residuals for the prio infu prediction
    Residuals temp = residual;

    //Compute the prio and post infusion predictions
    prioInfu = computeSteadyState(start, dosage->tinf() - start, parameters, dosage, prioPoints, temp, computeResidual);
    postInfu = computeSteadyState(dosage->tinf(), duration - dosage->tinf() + start, parameters, dosage, postPoints, residual, computeResidual);

    //Concatenate the two and return the complete prediction
    return prioInfu + postInfu;
}
