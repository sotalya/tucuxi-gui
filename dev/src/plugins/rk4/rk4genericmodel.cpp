//@@license@@

#include "rk4genericmodel.h"
#include "version.h"
#include <math.h>


RK4GenericModel::RK4GenericModel(int nbComp)
{
    _priority = 20;
    setNbComp(nbComp);

    //We want ug for the dose
    _ug.fromString("ug");
    //And ug/l for the result
    _ugl.fromString("ug/l");

    //The minimal point per hour option
    addOption("mpph", tr("Min. points per hour"), tr("Minimum number of points to be computed per hour of drug administration"), QVariant::Double, QVariant(60.0));
}

void RK4GenericModel::setNbComp(int nbComp)
{
    this->nbComp = nbComp;

    for(int i=0;i<4;i++)
        rk4points[i].resize(nbComp);
}

QVector<double> RK4GenericModel::y0 ()
{
    QVector<double> initDosage(nbComp);
    for(int i=0;i<nbComp;i++)
        initDosage[i] = 0.0;
    return initDosage;

}

Version RK4GenericModel::version()
{
    return Version("0.1");
}

//Do the wanted prediction with the wanted parameters
DataSet RK4GenericModel::predict(const Duration &start, const Duration &until, const ParameterSet* &parameters, uint points, const SharedDosage &dosage, Residuals &residuals, bool uniform)
{
    Q_UNUSED(uniform);

    //Fetch the parameters
    getParameters(parameters);

    //Create points
    DataSet sn(points,_ugl); // Resulting points, as asked

    //Minimal number of point per hour to calculate (option)
    double min_pph = option("mpph").value.toDouble();

    //Fetch the dosage (in ug)
    D = dosage->dose().valueIn(_ug);

    // Find the Cresid corresponding to t, as A(t0=0) = Dose only...
    double t = 0.0;

    //Current concentration and its preceding value
    QVector<double> oldc(nbComp);
    QVector<double> crtc(nbComp);
    QVector<double> last(nbComp);

    //First concentration
    for (int i=0;i<nbComp; i++)
        crtc[i] = residuals.at(i).value;

    QVector<double> initDosage = y0();
    for (int i=0;i<nbComp;i++)
        crtc[i] += initDosage.at(i);

    // //////////////////////////////////////////////////////////////////////////////////
    // 1. Skip the time from the beginning of cycle to the asked start, if needed, and
    //    calculate the first point of the prediction
    // //////////////////////////////////////////////////////////////////////////////////

    if (start.mSecs() > 0) {

        //Starting time
        double ts = start.toHours();

        //The number of points to pre-calculate : hours to pre-calculate * min_pph
        int compute_points = start.toHours() * min_pph;

        // ToDo //////////////////////////////////////////////////////////////////////////////////////////
        // There is an issue with a mmph < 3600 (and possibly when exactly 3600) when start < 1 hour. In
        // those cases, compute_points = 0 and we have a infinite delta, impacting the point 1. below. A
        // possible solution would be to force mpph to be at least 3600, requiring to add a min/max value
        // to all option, or find another solution in the computation below (eg. using a mppsec or msec).
        // Do not forget to remove the test/Q_ASSERT below when the solution is implemented.
        // ///////////////////////////////////////////////////////////////////////////////////////////////

        //Check for division by 0
        Q_ASSERT(compute_points != 0.0);

        //Time between two points
        const double delta = start.toHours() / compute_points; // 0 to ts in first cycle to show

        for (int i = 0; i < (compute_points - 1) && (t < ts); i++) {
            t += delta;
            oldc = crtc;
            rungeKutta4(t, oldc, dosage, delta, crtc);
        }

        //Calculate the point at the exact asked start
        rungeKutta4(start.toHours(), QVector<double>(crtc), dosage, delta, crtc);
    } else {

        //Calculate the point at the beginning of the cycle
        rungeKutta4(0.0, QVector<double>(crtc), dosage, 0.0, crtc);
    }

    // //////////////////////////////////////////////////////////////////////////////////
    // 2. Compute the points required points from the asked start to the end of the cycle
    // //////////////////////////////////////////////////////////////////////////////////

    //Initial condition : N = starting cycle of graph, tn = X, yn has been computed
    t = start.toHours();

    //Find the minimum of points to be calculated, as a multiple of the asked points
    int p2p = 0;
    if (points > 1)
        p2p = ceil(min_pph * until.toHours() / (points-1));
    else
        p2p = ceil(min_pph * until.toHours());

    //Check for division by 0
    Q_ASSERT(p2p != 0);

    //Delta in hours between two calculated points
    double delta = 0.0;
    if (points > 1)
        delta = until.toHours() / ((points-1) * p2p);
    else
        delta = until.toHours() / p2p;

    //Compute the points and insert them into the result
    for (uint i=0; i < points; i++) {

        //Keep compartment 0 as the one representing the concentration
        sn[i] = crtc.at(0);
        last  = crtc;

        //Calculate each point
        for (int j=0; j<p2p; j++) {
            t += delta;
            oldc = crtc;
            rungeKutta4(t,oldc,dosage,delta, crtc);
        }
    }

    //If computing only one point, keep the one corresponding to the end of the cycle
    if (points == 1) {
        sn[0] = crtc.at(0);
        last  = crtc;
    }

    //Set the new residual
    for(int i=0;i<nbComp;i++)
        residuals[i] = Residual(last.at(i));

    return sn;
}

/*
 * This is a quite literal transcription of a Runge-Kutta 4th order algorithm step.
 * The "f" function receives only the current t and y, it is its responsibility to
 * find any parameters needed from the current object to compute the next value.
 */
void RK4GenericModel::rungeKutta4 (double tn, const QVector<double> &yn, const SharedDosage &dosage, double h, QVector<double> &result)
{
    double hh = h / 2.;
    QVector<double> ynTemp(yn);

    //TODO Separate the steps for two compartement
    yDeriv(tn     ,           ynTemp, dosage, rk4points[0]);

    for(int i=0;i<nbComp;i++)
        ynTemp[i] = yn.at(i) + hh*rk4points[0][i];

    yDeriv(tn + hh, ynTemp, dosage, rk4points[1]);

    for(int i=0;i<nbComp;i++)
        ynTemp[i] = yn.at(i) + hh*rk4points[1][i];

    yDeriv(tn + hh, ynTemp, dosage, rk4points[2]);

    for(int i=0;i<nbComp;i++)
        ynTemp[i] = yn.at(i) + h*rk4points[2][i];

    yDeriv(tn + h ,  ynTemp, dosage, rk4points[3]);

    result.resize(yn.size());
    for(int i=0;i<nbComp;i++)
    {
        result[i] = yn.at(i) + (h / 6.0) * ( rk4points[0].at(i) + 2.0*rk4points[1].at(i) + 2.0*rk4points[2].at(i) + rk4points[3].at(i) );
        if (result[i] < 0.0)
            result[i] = 0.0;

        //Check the validity of the result
        Q_ASSERT(!isnan(result[i]));
        Q_ASSERT(!isinf(result[i]));
    }
}
