#include "rk4.h"

#include "ezxmlreader.h"
#include "core_errors.h"
#include "coremessagehandler.h"
#include "convert.h"
#include "rk4michaelismenten1comp.h"
#include "rk4michaelismenten2comp.h"
#include "rk4michaelismenten3comp.h"
#include "rk4linear1comp.h"
#include "rk4linear2comp.h"
#include "rk4linear3comp.h"

#include <QtPlugin>


RK4::RK4() : _pluginId(RK4_ID)
{
}

//Set the posible options and parameters
void RK4::init (Core *core)
{
    //Set the core
    Core::setInstance(core);

    //Create the models
    QList<DrugModel*> models;
    models.append(new RK4Linear1CompBolusK());
    models.append(new RK4Linear1CompBolusClV());
    models.append(new RK4Linear1CompInfK());
    models.append(new RK4Linear1CompInfClV());
    models.append(new RK4Linear1CompExtraK());
    models.append(new RK4Linear1CompExtraClV());
    models.append(new RK4Linear2CompBolusK());
    models.append(new RK4Linear2CompBolusClV());
    models.append(new RK4Linear2CompiInfK());
    models.append(new RK4Linear2CompiInfClV());
    models.append(new RK4Linear2CompExtraK());
    models.append(new RK4Linear2CompExtraClV());
    models.append(new RK4Linear3CompBolusK());
    models.append(new RK4Linear3CompBolusClV());
    models.append(new RK4Linear3CompInfK());
    models.append(new RK4Linear3CompInfClV());
    models.append(new RK4Linear3CompExtraK());
    models.append(new RK4Linear3CompExtraClV());
    models.append(new RK4MichaelisMenten1CompBolus());
    models.append(new RK4MichaelisMenten1CompInf());
    models.append(new RK4MichaelisMenten1CompExtra());
    models.append(new RK4MichaelisMenten2CompBolusK());
    models.append(new RK4MichaelisMenten2CompBolusClV());
    models.append(new RK4MichaelisMenten2CompInfK());
    models.append(new RK4MichaelisMenten2CompInfClV());
    models.append(new RK4MichaelisMenten2CompExtraK());
    models.append(new RK4MichaelisMenten2CompExtraClV());
    models.append(new RK4MichaelisMenten3CompBolusK());
    models.append(new RK4MichaelisMenten3CompBolusClV());
    models.append(new RK4MichaelisMenten3CompInfK());
    models.append(new RK4MichaelisMenten3CompInfClV());
    models.append(new RK4MichaelisMenten3CompExtraK());
    models.append(new RK4MichaelisMenten3CompExtraClV());
    models.append(new RK4MichaelisMenten1CompBolusVmOverV());
    models.append(new RK4MichaelisMenten1CompInfVmOverV());
    models.append(new RK4MichaelisMenten1CompExtraVmOverV());
    models.append(new RK4MichaelisMenten1CompExtraInf());

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
QString RK4::pluginId()
{
    return _pluginId;
}

//Use the model's sub-models
DrugModel *RK4::newModel(QString id)
{
    if (id == _models.at(0).id)
        return new RK4Linear1CompBolusK();
    else if (id == _models.at(1).id)
        return new RK4Linear1CompBolusClV();
    else if (id == _models.at(2).id)
        return new RK4Linear1CompInfK();
    else if (id == _models.at(3).id)
        return new RK4Linear1CompInfClV();
    else if (id == _models.at(4).id)
        return new RK4Linear1CompExtraK();
    else if (id == _models.at(5).id)
        return new RK4Linear1CompExtraClV();
    else if (id == _models.at(6).id)
        return new RK4Linear2CompBolusK();
    else if (id == _models.at(7).id)
        return new RK4Linear2CompBolusClV();
    else if (id == _models.at(8).id)
        return new RK4Linear2CompiInfK();
    else if (id == _models.at(9).id)
        return new RK4Linear2CompiInfClV();
    else if (id == _models.at(10).id)
        return new RK4Linear2CompExtraK();
    else if (id == _models.at(11).id)
        return new RK4Linear2CompExtraClV();
    else if (id == _models.at(12).id)
        return new RK4Linear3CompBolusK();
    else if (id == _models.at(13).id)
        return new RK4Linear3CompBolusClV();
    else if (id == _models.at(14).id)
        return new RK4Linear3CompInfK();
    else if (id == _models.at(15).id)
        return new RK4Linear3CompInfClV();
    else if (id == _models.at(16).id)
        return new RK4Linear3CompExtraK();
    else if (id == _models.at(17).id)
        return new RK4Linear3CompExtraClV();
    else if (id == _models.at(18).id)
        return new RK4MichaelisMenten1CompBolus();
    else if (id == _models.at(19).id)
        return new RK4MichaelisMenten1CompInf();
    else if (id == _models.at(20).id)
        return new RK4MichaelisMenten1CompExtra();
    else if (id == _models.at(21).id)
        return new RK4MichaelisMenten2CompBolusK();
    else if (id == _models.at(22).id)
        return new RK4MichaelisMenten2CompBolusClV();
    else if (id == _models.at(23).id)
        return new RK4MichaelisMenten2CompInfK();
    else if (id == _models.at(24).id)
        return new RK4MichaelisMenten2CompInfClV();
    else if (id == _models.at(25).id)
        return new RK4MichaelisMenten2CompExtraK();
    else if (id == _models.at(26).id)
        return new RK4MichaelisMenten2CompExtraClV();
    else if (id == _models.at(27).id)
        return new RK4MichaelisMenten3CompBolusK();
    else if (id == _models.at(28).id)
        return new RK4MichaelisMenten3CompBolusClV();
    else if (id == _models.at(29).id)
        return new RK4MichaelisMenten3CompInfK();
    else if (id == _models.at(30).id)
        return new RK4MichaelisMenten3CompInfClV();
    else if (id == _models.at(31).id)
        return new RK4MichaelisMenten3CompExtraK();
    else if (id == _models.at(32).id)
        return new RK4MichaelisMenten3CompExtraClV();
    else if (id == _models.at(33).id)
        return new RK4MichaelisMenten1CompBolusVmOverV();
    else if (id == _models.at(34).id)
        return new RK4MichaelisMenten1CompInfVmOverV();
    else if (id == _models.at(35).id)
        return new RK4MichaelisMenten1CompExtraVmOverV();
    else if (id == _models.at(36).id)
        return new RK4MichaelisMenten1CompExtraInf();
    else
        return 0;
}

//Build in init()
QList<ModelDescriptor> RK4::models()
{
    return _models;
}

//Returns the models priorities
int RK4::priority(const QString &id) const
{
    return _priorities.value(id, -1);
}
