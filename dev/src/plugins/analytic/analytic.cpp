#include "analytic.h"

#include "ezxmlreader.h"
#include "core_errors.h"
#include "coremessagehandler.h"
#include "convert.h"

#include <QtPlugin>


#include "analyticlinear1compbolus.h"
#include "analyticlinear1compextra.h"
#include "analyticlinear1compinf.h"
#include "analyticlinear2compbolus.h"
#include "analyticlinear2compextra.h"
#include "analyticlinear2compinf.h"
#include "analyticlinear3compbolus.h"
#include "analyticlinear3compextra.h"
#include "analyticlinear3compinf.h"
#include "analyticnonlinear1compgammaextra.h"


Analytic::Analytic() : _pluginId(ANALYTIC_ID)
{
}

// Set the possible options and parameters
void Analytic::init (Core *core)
{
    //Set the core
    Core::setInstance(core);

    //Create the models
    QList<DrugModel*> models;
    models.append(new AnalyticLinear1CompBolusK());
    models.append(new AnalyticLinear1CompBolusClV());
    models.append(new AnalyticLinear1CompExtraK());
    models.append(new AnalyticLinear1CompExtraClV());
    models.append(new AnalyticLinear1CompInfK());
    models.append(new AnalyticLinear1CompInfClV());
    models.append(new AnalyticLinear2CompBolusK());
    models.append(new AnalyticLinear2CompBolusClV());
    models.append(new AnalyticLinear2CompExtraK());
    models.append(new AnalyticLinear2CompExtraClV());
    models.append(new AnalyticLinear2CompInfClV());
    models.append(new AnalyticLinear3CompBolusK());
    models.append(new AnalyticLinear3CompBolusClV());
    models.append(new AnalyticLinear3CompExtraK());
    models.append(new AnalyticLinear3CompExtraClV());
    models.append(new AnalyticLinear3CompInfK());
    models.append(new AnalyticLinear3CompInfClV());
    models.append(new AnalyticLinear2CompInfK());
    models.append(new AnalyticNonlinear1CompGammaExtraK());
    models.append(new AnalyticNonlinear1CompGammaExtraClV());

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
QString Analytic::pluginId()
{
    return _pluginId;
}

//Create the corresponding new model
DrugModel *Analytic::newModel(QString id)
{
    if (id == _models.at(0).id)
        return new AnalyticLinear1CompBolusK();
    else if (id == _models.at(1).id)
        return new AnalyticLinear1CompBolusClV();
    else if (id == _models.at(2).id)
        return new AnalyticLinear1CompExtraK();
    else if (id == _models.at(3).id)
        return new AnalyticLinear1CompExtraClV();
    else if (id == _models.at(4).id)
        return new AnalyticLinear1CompInfK();
    else if (id == _models.at(5).id)
        return new AnalyticLinear1CompInfClV();
    else if (id == _models.at(6).id)
        return new AnalyticLinear2CompBolusK();
    else if (id == _models.at(7).id)
        return new AnalyticLinear2CompBolusClV();
    else if (id == _models.at(8).id)
        return new AnalyticLinear2CompExtraK();
    else if (id == _models.at(9).id)
        return new AnalyticLinear2CompExtraClV();
    else if (id == _models.at(10).id)
        return new AnalyticLinear2CompInfClV();
    else if (id == _models.at(11).id)
        return new AnalyticLinear3CompBolusK();
    else if (id == _models.at(12).id)
        return new AnalyticLinear3CompBolusClV();
    else if (id == _models.at(13).id)
        return new AnalyticLinear3CompExtraK();
    else if (id == _models.at(14).id)
        return new AnalyticLinear3CompExtraClV();
    else if (id == _models.at(15).id)
        return new AnalyticLinear3CompInfK();
    else if (id == _models.at(16).id)
        return new AnalyticLinear3CompInfClV();
    else if (id == _models.at(17).id)
        return new AnalyticLinear2CompInfK();
    else if (id == _models.at(18).id)
        return new AnalyticNonlinear1CompGammaExtraK();
    else if (id == _models.at(19).id)
        return new AnalyticNonlinear1CompGammaExtraClV();
    else
        return 0;
}

//List created in init()
QList<ModelDescriptor> Analytic::models()
{
    return _models;
}

//Returns the models priorities
int Analytic::priority(const QString &id) const
{
    return _priorities.value(id, -1);
}
