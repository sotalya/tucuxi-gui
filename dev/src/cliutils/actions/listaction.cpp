//@@license@@

#include "listaction.h"

#include "core/pluginmanager.h"
#include "apputils/src/drugmanager.h"
#include "core/utils/ezutils.h"
#include "apputils/src/appcore.h"
#include "core/utils/logging.h"
#include "errors_cliutils.h"

using namespace Tucuxi::Gui::Core;

namespace Tucuxi {
namespace cliutils {
QString ListAction::name()
{
    return "list";
}

QString ListAction::help()
{
    QString str;
    str = tr("   NAME") + "\n" +
          tr("       EzeCHieL List Command") + "\n" + "\n" +
          tr("   SYNOPSIS") + "\n" +
             "       " + QString("ezechiel-cli [OPTIONS] list %1USERNAME %2PASSWORD SUBCOMMAND").arg(_CMD_PARAM_USERNAME, _CMD_PARAM_PASSWORD) + "\n" + "\n" +
          tr("   DESCRIPTION") + "\n" +
          tr("       The EzeCHieL List Command can be used to retrieve lists of data, such as the drugs list, the models list or the engines list.") + "\n" + "\n" +
          tr("   SUBCOMMANDS") + "\n" +
             "       drugs" + "\n" +
          tr("              Display the available drugs. If a model is given, only the drug compatible with this model will be displayed.")  + "\n" + "\n" +
             "       models [DRUG]" + "\n" +
          tr("              Display the available models. If the drug identifier DRUG is given, only the models for this drug are shown.")  + "\n" + "\n" +
             "       percentile_engines" + "\n" +
          tr("              Display the available percentile engines.")  + "\n" + "\n" +
             "       post_engines" + "\n" +
          tr("              Display the available a posteriori engines.")  + "\n" + "\n" +
             "       reverse_engines" + "\n" +
          tr("              Display the available reverse engines.")  + "\n" + "\n" +
             "       options ENGINE|MODEL" + "\n" +
          tr("              Display the options for the given model or engine.")  + "\n" + "\n" +
          "\n";

    return str;
}

//Depanding on the first parameter, fetch the wanted data
bool ListAction::run(const QStringList &args)
{
    _errors.clear();

    //If the list is empty, error
    if (args.isEmpty()) {
        _errors = tr("No element to list given");
        return false;
    }

    //No more than two elements
    if (args.size() > 2) {
        _errors = tr("Too much parameters given");
        return false;
    }

    //Fetch the first and possibly the second parameters
    QString first = args.at(0);
    QString second;
    if (args.size() > 1)
        second = args.at(1);

    //Used to know if too much data is given
    bool isTooMuch = false;

    //The first parameter define what is to be listed
    if (first == "models") {
        QList<ModelDescriptor> result;

        //Build the resulting list depending on the second parameter value
        if (second.isEmpty())
            result = CORE->pluginManager()->models();
        else
            foreach (QString mid, CORE->pluginManager()->filter(APPCORE->drugManager()->models(second)))
                result.append(CORE->pluginManager()->model(mid));

        foreach (ModelDescriptor md, result)
            LOG(QtDebugMsg, NOEZERROR, QString(md.toString()));

    } else if (first == "drugs") {
        QList<Descriptor> result;

        //Build the resulting list depending on the second parameter value
        if (second.isEmpty())
            result = APPCORE->drugManager()->descriptions();
        else
            result = APPCORE->drugManager()->descriptionsForModel(second);

        foreach (Descriptor d, result)
            LOG(QtDebugMsg, NOEZERROR, QString(d.toString()));

    } else if (first == "options") {
        listOptions(second);
    } else //Error
        _errors = tr("Unknow element to list given : %1").arg(first);

    //If there is too much data, error
    if (isTooMuch)
        _errors = tr("A second parameter is given, but not awaited : %2").arg(second);

    //If there is an error, return false
    return _errors.isEmpty();
}

//Lists the engines options
void ListAction::listOptions(const QString &arg)
{
    QScopedPointer<Optionable> engine;

    //Check if an engine ID is given
    if (arg.isEmpty()) {
        _errors = tr("You need to specify the model or engine ID for which the options must be displayed");
        return;
    }

    //Load the corresponding engine
    if (CORE->pluginManager()->modelExists(arg)) {
    } else {
        _errors = tr("Could not find any model or engine with the ID '%1'").arg(arg);
        return;
    }

    //List the engine options
    foreach (Option option, engine->getOptions())
        LOG(QtDebugMsg, NOEZERROR, QString(option.infos.toString()));
}

} //namespace cliutils
} //namespace Tucuxi
