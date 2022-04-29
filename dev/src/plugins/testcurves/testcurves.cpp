//@@license@@

#include "testcurves.h"

#include "ezxmlreader.h"
#include "core_errors.h"
#include "coremessagehandler.h"
#include "convert.h"

#include <QtPlugin>



TestCurves::TestCurves() : _pluginId(TESTCURVES_ID)
{
}

// Set the possible options and parameters
void TestCurves::init (Core *core)
{
    //Set the core
    Core::setInstance(core);

    //Create the models
    QList<DrugModel*> models;
    models.append(new TestCurveConstant());
    models.append(new TestCurveProportional());
    models.append(new TestCurveConstantSum());

    //Store the descriptions
    for (int i = 0; i < models.size(); i++)
        _models.append(models.at(i)->descriptor());

    //Store the priorities
    for (int i = 0; i < models.size(); i++)
        _priorities.insert(models.at(i)->descriptor().id, models.at(i)->priority());

    //Delete the models
    qDeleteAll(models);
}

//Returns the plugin ID
QString TestCurves::pluginId()
{
    return _pluginId;
}

//Create the corresponding new model
DrugModel *TestCurves::newModel(QString id)
{
    if (id == _models.at(0).id)
        return new TestCurveConstant();
    else if (id == _models.at(1).id)
        return new TestCurveProportional();
    else if (id == _models.at(2).id)
        return new TestCurveConstantSum();
    return 0;
}

//List created in init()
QList<ModelDescriptor> TestCurves::models()
{
    return _models;
}

//Returns the models priorities
int TestCurves::priority(const QString &id) const
{
    return _priorities.value(id, -1);
}
















#include "knownmodels.h"










TestCurveConstant::TestCurveConstant()
{
    _priority = 10;
    //We want ug for the dose
    _ug.fromString("ug");
    //And ug/l for the result
    _ugl.fromString("ug/l");

    // Parameters
    _parameters.append(Descriptor("P","Parameter","Parameter"));
}

//Create it here
ModelDescriptor TestCurveConstant::descriptor()
{
    return ModelDescriptor ( MODEL_TEST_CONSTANT_ID,
                             TEST_CONSTANT_ID,
                             tr("Constant value"),
                             tr("Model returning a constant value"),
                             "bolus" );
}

//Returns the model priority level


//Yeah!
Version TestCurveConstant::version()
{
    return Version("0.1");
}


//Static (that's what the CHUV said)
Duration TestCurveConstant::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
{
    //Fetch the parameters
    double C = parameters->get("C")->value();

    //Check for division by 0
    Q_ASSERT(C != 0.0);

    //Log(2) static value
#define LOG2H 0.693147180559945*3600000.0
    // t½ = ln(2) / (CL/V)
    return Duration(0,0,0,10);
#undef LOG2H
}

//Simple formula application
DataSet TestCurveConstant::predict(const Duration &start, const Duration &until, const ParameterSet* &parameters, uint points, const SharedDosage &dosage, Residuals &residuals, bool uniform)
{
    Q_UNUSED(uniform);
    Q_UNUSED(dosage);

    //Fetch the parameters
    double P = parameters->get("P")->value();
    
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

    //Calculate each point
    for (uint i=0; i < points; i++) {
        data[i] = P;
        T += delta;
    }

    //Recalibrate start time
    T -= delta;

    //Set the new residual
    residuals[0] = Residual(data[points-1]);

    return data;
}








TestCurveProportional::TestCurveProportional()
{
    _priority = 10;
    //We want ug for the dose
    _ug.fromString("ug");
    //And ug/l for the result
    _ugl.fromString("ug/l");

    // Parameters
    _parameters.append(Descriptor("P","Parameter","Parameter"));
}


//Create it here
ModelDescriptor TestCurveProportional::descriptor()
{
    return ModelDescriptor ( MODEL_TEST_PROPORTIONAL_ID,
                             TEST_PROPORTIONAL_ID,
                             tr("Proportional value"),
                             tr("Model returning a \"constant\" value that depends on the dosage"),
                             "bolus" );
}


//Returns the model priority level


//Yeah!
Version TestCurveProportional::version()
{
    return Version("0.1");
}


//Static (that's what the CHUV said)
Duration TestCurveProportional::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
{
    Q_UNUSED(parameters);

    return Duration(0,0,0,10);
}

//Simple formula application
DataSet TestCurveProportional::predict(const Duration &start, const Duration &until, const ParameterSet* &parameters, uint points, const SharedDosage &dosage, Residuals &residuals, bool uniform)
{
    Q_UNUSED(uniform);

    //Fetch the parameters
    double P = parameters->get("P")->value();

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

    //Calculate each point
    for (uint i=0; i < points; i++) {
        data[i] = D * P;
        T += delta;
    }

    //Recalibrate start time
    T -= delta;

    //Set the new residual
    residuals[0] = Residual(data[points-1]);

    return data;
}









TestCurveConstantSum::TestCurveConstantSum()
{
    _priority = 10;
    //We want ug for the dose
    _ug.fromString("ug");
    //And ug/l for the result
    _ugl.fromString("ug/l");

    // Parameters
    _parameters.append(Descriptor("P1","Parameter 1","Parameter 1"));
    _parameters.append(Descriptor("P2","Parameter 2","Parameter 2"));
}


//Create it here
ModelDescriptor TestCurveConstantSum::descriptor()
{
    return ModelDescriptor ( MODEL_TEST_CONSTANT_SUM_ID,
                             TEST_CONSTANT_SUM_ID,
                             tr("Constant sum value"),
                             tr("Model returning a constant value that is the sum of the two parameters"),
                             "bolus" );
}




//Yeah!
Version TestCurveConstantSum::version()
{
    return Version("0.1");
}


//Static (that's what the CHUV said)
Duration TestCurveConstantSum::halflife(const Duration &, const ParameterSet* &parameters, const SharedDosage &, const Residuals &)
{
    Q_UNUSED(parameters);

    return Duration(0,0,0,10);
}

//Simple formula application
DataSet TestCurveConstantSum::predict(const Duration &start, const Duration &until, const ParameterSet* &parameters, uint points, const SharedDosage &dosage, Residuals &residuals, bool uniform)
{
    Q_UNUSED(uniform);
    Q_UNUSED(dosage);

    //Fetch the parameters
    double P1 = parameters->get("P1")->value();
    double P2 = parameters->get("P2")->value();

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

    //Calculate each point
    for (uint i=0; i < points; i++) {
        data[i] = P1 + P2;
        T += delta;
    }

    //Recalibrate start time
    T -= delta;

    //Set the new residual
    residuals[0] = Residual(data[points-1]);

    return data;
}


