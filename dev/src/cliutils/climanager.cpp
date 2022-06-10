//@@license@@

#include "climanager.h"

#include <signal.h>
#include <iostream>
#include <cstdlib>

/** \defgroup cli Command Line Interface (CLI)
   The CLI is one of the two available human-machine interfaces for the Core library, the other being the GUI.
   The Command Line Interface provides an access to the Core library through the console.
   The parameters are parsed, and the different associated action executed.
   Here are the different CLI's component.
   \li \ref main "The main function" : This function will receive the command line argument, parse the global option, deal with the Security part and finally run an action.
   \li Errors : This object is an interface to retrieve informations about an error retruned by an action.
   \li CliMessageHandler : This singleton handles the output to the console and log file, all the messages and error are sent to it.
   \li Action : This interface gives a generalization about what a function have to do.
   \li \ref actions "The actions" : Described in the specification, one objet is used for each action, but an action might use multiple object itself.
  */

#include <QApplication>
#include <QLocale>
#include <QString>
#include <QMap>
#include <QList>
#include <QObject>
#include <QDir>
#include <QDebug>

#include "core/core.h"
#include "core/settings.h"
#include "apputils/src/appcore.h"
#include "apputils/src/securitymanager.h"
#include "action.h"
#include "cliutils/actions/useraction.h"
#include "cliutils/actions/listaction.h"
#include "cliutils/actions/patientaction.h"
#include "cliutils/actions/predictionaction.h"
#include "cliutils/actions/importaction.h"
#include "cliutils/actions/restaction.h"
#include "cliutils/actions/sniffaction.h"
#include "cliutils/actions/exportaction.h"
#include "cliutils/actions/addaction.h"
#include "core/utils/version.h"
#include "core/corerepository.h"
#include "admin/src/stdadminrepository.h"
#include "core/interfaces/abstractrepository.h"
#include "apputils/src/apputilsrepository.h"
#include "core/processingfactory.h"
#include "databases/ephemeraldb/ephemeraldb.h"
#include "core/utils/logging.h"
#include "errors_cliutils.h"
#include "rest/restlogger.h"

#include "rest/network/mirthrequest.h"

namespace Tucuxi {
namespace cliutils {
// Print a message when Tucuxi is stopped by a signal
static void signalHandler (int id)
{
//    STRACE("main",INTERRUPTED,QObject::tr("Interrupted by signal ")+QString::number(id));
}

// Print a stack trace
void traceHandler (int)
{
    //Give the error
//    STRACE("main",INTERRUPTED,QObject::tr("A SIGSEGV signal has been received"));
}

//Load the actions one by one, and put them in a QMap with their name as key
QList<QString> loadActions ()
{
    QList<QString> actions;
    actions.append(UserAction::name());
    actions.append(ListAction::name());
    actions.append(PatientAction::name());
    actions.append(PredictionAction::name());
    actions.append(ImportAction::name());
    actions.append(RestAction::name());
    actions.append(ExportAction::name());
    actions.append(AddAction::name());
    return actions;
}


CliManager::CliManager(QObject* parent)
    :/* m_cmh(0),*/ QObject(parent), m_app(0)
{
}

CliManager::~CliManager() {
    if (m_app)
        delete(m_app);
}


void CliManager::setLanguage(CliCommand* query) {

    //Set the current language
    if (!query->language.isEmpty() && query->language != CORE->language())
        CORE->setLanguage(query->language);

    //Set the translators
    QStringList components;
    components  << "core" << "cli" << "chart";

//    if (!CORE->setTranslators(components))
//        LOG(QtWarningMsg, NOEZERROR, QObject::tr("An error occured during the installation of the translation files"));
}

CommandLineParseResult CliManager::parseCommandLine(QCommandLineParser &parser, CliCommand *query) {
    DBAccount account;

    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);

    //NON-POSITIONALS

    const QCommandLineOption helpOption(QStringList() << "h" << "help", "Print a usage message briefly summarizing these command-line options, then exit.", "COMMAND");
    parser.addOption(helpOption);
//    const QCommandLineOption colorOption(QStringList() << "C" << "color", "Activate the output color (Linux/UNIX only).");
//    parser.addOption(colorOption);
//    const QCommandLineOption dbOption(QStringList() << "db" << "database", "Use the database file DATABASE instead of EzeCHieL internal database.", "FILE");
//    parser.addOption(dbOption);
    const QCommandLineOption logOption(QStringList() << "L" << "log", "Setup and use the log file FILE.", "FILE");
    parser.addOption(logOption);
    const QCommandLineOption tcpserverOption("tcpserver", "Enable EzeCHieL TCP server, used to retrieve data from external entitites.");
    parser.addOption(tcpserverOption);
    const QCommandLineOption languageOption(QStringList() << "T" << "language", "Force the language to LANGUAGE. The language code must be specified using the two letters code as defined in ISO 639-1.", "LANGUAGE");
    parser.addOption(languageOption);
    const QCommandLineOption versionOption(QStringList() << "v" << "version", "Print the version number of EzeCHieL, as well as the build ID.");
    parser.addOption(versionOption);
    const QCommandLineOption levelOption(QStringList() << "V" << "level", "Set the verbosity to LEVEL, ranging from 0 to 3 (0 = only errors, 1 = errors and warnings, 2 = errors, warnings, debugs and trace, 3 = all). The default is set to 1.", "LEVEL");
    parser.addOption(levelOption);
//    const QCommandLineOption HTMLcolorOption(QStringList() << "W" << "htmlcolor", "Activate the HTML output color (Linux/UNIX only).");
//    parser.addOption(HTMLcolorOption);
//    const QCommandLineOption scriptOption(QStringList() << "do", "Run script.", "FILE");
//    parser.addOption(scriptOption);
    //mirth connection options
    const QCommandLineOption basePathOption(QStringList() << "b" << "base-path",
                                    QCoreApplication::translate("main", "The base path of the server."), "path", "/services");
    parser.addOption(basePathOption);
    const QCommandLineOption disableMeldOption(QStringList() << "m" << "disable-meld",
                                    QCoreApplication::translate("main", "The host name of the server"));
    parser.addOption(disableMeldOption);
    const QCommandLineOption portOption(QStringList() << "p" << "port",
                                    QCoreApplication::translate("main", "The port number of the server."), "port", QString::number(8889));
    parser.addOption(portOption);
    const QCommandLineOption schemeOption(QStringList() << "s" << "scheme",
                                    QCoreApplication::translate("main", "The scheme of the server."), "scheme", "https");
    parser.addOption(schemeOption);
    const QCommandLineOption hostOption(QStringList() << "t" << "host",
                                    QCoreApplication::translate("main", "The host name of the server"), "host", "ezecs1");
    parser.addOption(hostOption);

    const QCommandLineOption logRestOption(QStringList() << "l" << "logrest",
                                    QCoreApplication::translate("main", "Set the path in which the REST logs will be stored."),
                                    "logrest",
                                    QApplication::applicationDirPath());
    parser.addOption(logRestOption);


    //POSITIONALS

    parser.addPositionalArgument("User", "The EzeCHieL User Command can be used to manage the EzeCHieL users as well as the encryption status of the database.");
    parser.addPositionalArgument("list", "The EzeCHieL List Command can be used to retrieve lists of data, such as the drugs list, the models list or the engines list.");
    parser.addPositionalArgument("Patient", "The EzeCHieL Patient Command can be used to manage patients and their associated data, such as the measures and covariates.");
    parser.addPositionalArgument("Prediction", "The EzeCHieL Prediction Command can be used to manage curves and their associated data, such as dosages, engines parameters and more. The 'compare' option in the 'percentile' and the 'points' sub-commands can be used to compare the resulting data with the values in a file. All dates must be specified using the format 'dd/MM/yyyy-HH:mm'.");
    parser.addPositionalArgument("Import", "The EzeCHieL Import Command can be used to import data from a valid import XML file. The PATH of the file can be either absolute, or relative to the application folder.");
    parser.addPositionalArgument("Rest", "");
    parser.addPositionalArgument("Export", "The EzeCHieL Export Command can be used to export various data such as information about the models. It can also be used to export data about an interpretation and its related data in an export XML file.");
    parser.addPositionalArgument("Add", "The EzeCHieL Add Command can be used to add data to existing curves and patients. The concentrations must be specified in [ug/l], the dose in [mg] and the intervals in seconds. All dates must be specified using the format 'dd/MM/yyyy-HH:mm'. The command is obsolete and will be replaced by similar actions in other commands.");
    parser.addPositionalArgument("Sniff", "Sniffs the requests the server contains");

    if (!parser.parse(QCoreApplication::arguments())) {
            LOG(QtDebugMsg, Tucuxi::cliutils::NOEZERROR, parser.errorText());
            return CommandLineError;
    }

    if (parser.isSet(versionOption))
        parser.showVersion();

#ifdef CONFIG_CONNECTED
    LOG(QtDebugMsg, Tucuxi::cliutils::NOEZERROR, tr("Configured to connect to remote server"));
    MirthRequest::updateDefaultBasePath(parser.value(basePathOption));
    MirthRequest::updateDefaultPort(parser.value(portOption).toInt());
    MirthRequest::updateDefaultScheme(parser.value(schemeOption));
    MirthRequest::updateDefaultHost(parser.value(hostOption));
    RestLogger::getInstance()->setPath(parser.value(logRestOption));
#endif // CONFIG_CONNECTED

    if (parser.isSet(helpOption))
           return CommandLineHelpRequested;

    if (parser.isSet(languageOption)) {
        query->language = QString(parser.value(languageOption));        
    }

    setLanguage(query);
    LOG(QtDebugMsg, Tucuxi::cliutils::NOEZERROR, tr("Set language to %1:").arg(query->language));

//    if (parser.isSet(dbOption)) {
//        account.url = QString(parser.value(dbOption));
//    }

//    connect(account);

    QStringList positionalArguments = parser.positionalArguments();
    if (positionalArguments.isEmpty()) {
        LOG(QtDebugMsg, Tucuxi::cliutils::NOEZERROR, tr("No commands to execute."));
        return CommandLineError;
    }

    query->actionName = positionalArguments.takeFirst();

    if (!positionalArguments.isEmpty()) {
        query->actionArgs = positionalArguments;
    }

    bool _actionIsSuccess = CliManager::selectAction(query, false);

    if (!_actionIsSuccess)
    {
        QApplication::quit();
        exit(EXIT_FAILURE);
    }
    return CommandLineOk;
}

//void CliManager::connect(DBAccount &account)
//{
//    Response response;

//    Tucuxi::EphemeralDB* _db = dynamic_cast<Tucuxi::EphemeralDB*>(ABSTRACTREPO);

    //Connect to the database
//    if (!_db->connect(account)){}
//        LOG(QtCriticalMsg, INVALIDDATABASE, QObject::tr("Could not connect to the database '%1' (%2)").arg(account.url, APPUTILSREPO->errorString()));

//    if (!_db->connect(account)) {}
//        LOG(QtCriticalMsg, INVALIDDATABASE, QObject::tr("Could not connect to the database."));

    //Check the compatibility
//    bool isCompatible;

//    if ((response = REPO->isCompatible(isCompatible)).error != NoError)
//        EZERROR(INVALIDDATABASE, QObject::tr("The database '%1' compatibility could not be verified (%2)").arg(account.url, response.message));
//    if (!isCompatible)
//        EZERROR(INVALIDDATABASE, QObject::tr("The database '%1' is incompatible with EzeCHieL").arg(account.url));

    //Check the version
//    QString version;

//    if ((response = APPUTILSREPO->getVersion(version)).error != NoError)
//        EZERROR(INVALIDDATABASE, QObject::tr("Failed to retrieve the database '%1' version (%2)").arg(account.url, response.message));
//    if (Version(version) != CORE->version(Core::Data))
//        EZERROR(OUTDATED, QObject::tr("The database '%1' is outdated (awaited '%2', got '%3'). Please update it.").arg(account.url, CORE->version(Core::Data).toString(), version));
//}

bool CliManager::selectAction(CliCommand* query, bool subaction_override) {

    //Switch between the available actions
    LOG(QtDebugMsg, Tucuxi::cliutils::NOEZERROR, tr("selecting action: %1").arg(query->actionName));
    Action *action;
    if (query->actionName == UserAction::name())
        action = new UserAction();
    else if (query->actionName == ListAction::name())
        action = new ListAction();
    else if (query->actionName == PatientAction::name())
        action = new PatientAction();
    else if (query->actionName == PredictionAction::name())
        action = new PredictionAction();
    else if (query->actionName == ImportAction::name())
        action = new ImportAction();
    else if (query->actionName == RestAction::name())
        action = new RestAction();
    else if (query->actionName == ExportAction::name())
        action = new ExportAction();
    else if (query->actionName == AddAction::name())
        action = new AddAction();
    else if (query->actionName == SniffAction::name())
        action = new SniffAction();
    else {
        return false;
        //ERROR macro doesnt work with static methods.
        //throw the error somehow
//        EZERROR(SYNTAXERROR, QObject::tr("Unkown command '%1'").arg(query->actionName));
    }

    //Run the action or display its help
    if (!query->actionArgs.isEmpty() && query->actionArgs.at(0) == "help") {

        //Display the action's help
        LOG(QtDebugMsg, NOEZERROR, action->help());

    } else {

        //Authenticate the user if necessary
        if (query->actionName != UserAction::name() && !subaction_override) {
            LOG(QtDebugMsg, Tucuxi::cliutils::NOEZERROR, tr("Authenticating user"));

            //Check if enough parameters
//            if (query->actionArgs.size() < 2 || !query->actionArgs.at(0).startsWith(Action::usernameParam()) || !query->actionArgs.at(1).startsWith(Action::passwordParam())) {
//                LOG(QtDebugMsg, Tucuxi::cliutils::DATAMISSING, tr("Missing username and/or password for command '%1', please run the help for more information").arg(query->actionName));
//                return false;
//            }

            //Get the username and password
//            query->actionArgs[0].remove(Action::usernameParam());
//            QString username = query->actionArgs.takeFirst();
//            LOG(QtDebugMsg, Tucuxi::cliutils::NOEZERROR, tr("Parsed username: %1").arg(username));
//            query->actionArgs[0].remove(Action::passwordParam());
//            QString password = query->actionArgs.takeFirst();
//            LOG(QtDebugMsg, Tucuxi::cliutils::NOEZERROR, tr("Parsed password"));

            //Try to authenticate the user
//            if (!APPCORE->securityManager()->authenticate(username, password)) {
//                EZERROR(AUTHENTICATIONFAILED, QObject::tr("The authentication failed, please check your username and password and try again"));
//                return false;
//            }
        }

        //Run the action and check for errors
        if (!action->run(query->actionArgs)) {
            QString toprint = "";
            foreach (QString arg, query->actionArgs) {
               toprint = toprint + " " + arg;
            }
            LOG(QtCriticalMsg, Tucuxi::cliutils::NOEZERROR, tr("Action %1 failed with args: %2").arg(query->actionName).arg(toprint));
            return false;
        }
        LOG(QtDebugMsg, Tucuxi::cliutils::NOEZERROR, tr("Action %1 completed").arg(query->actionName));
    }

    return true;
}

void CliManager::initializeCore() {

    //Catch the interruption signals
    signal(SIGABRT, &signalHandler);
    signal(SIGTERM, &signalHandler);
    signal(SIGINT, &signalHandler);
    signal(SIGSEGV, &traceHandler);
}

int CliManager::execute(QCoreApplication &app, int argc, char *argv[]) {
    Q_UNUSED(app);
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    initializeCore();
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("Tucuxi-cli", "Command line interface to the Tucuxi TDM Software."));
    CliCommand query;
    query.verbocity = 1;
    query.language = "";
    query.logfile = "";
    query.dburl = "";
    query.actionName = "";
    query.actionArgs = {};


    CORE->setProcessingInterface(ProcessingFactory::createProcessing());

    switch (parseCommandLine(parser, &query)) {
    case CommandLineOk:
        break;
    case CommandLineError:
        LOG(QtCriticalMsg, Tucuxi::cliutils::ACTIONERROR, tr("Command line error: %1 ").arg(query.actionName));
        parser.showHelp();
        return EXIT_FAILURE;
    case CommandLineHelpRequested:
        parser.showHelp();
        Q_UNREACHABLE();
    }

    return 0;
}

} //namespace cliutils
} //namespace Tucuxi
