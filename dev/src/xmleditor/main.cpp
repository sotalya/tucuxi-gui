#include "xmleditormessagehandler.h"
#include "editorpanel.h"
#include "core.h"
#include "config.h"

#include <signal.h>

#include <QApplication>

#define LANGUAGE "language"

/** \defgroup xmleditor XML Editor
 * The XML Editor is an application used to create and edit Tucuxi's drugs XML files.
 * While being a standalone application, it needs to be linked against the core module.
 * The editor's objects are separated in sub-directory following their usage:
 * \li gui/ The GUI elements of the applications
 * \li logic/ The objects used to manipulate the XML files
 * \li interfaces/ The interfaces used by the application
 *
 * The main function will first initialize the application's context and then build the main window.
 */

//Local functions declarations
XmlEditorMessageHandler *initMsgHandler(int verbosity);
void initConfig();
void initPaths();
void initLanguages();
void signalHandler(int interruptId);

//Application main function
int main(int argc, char *argv[])
{
    //Initialize the application
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(core);

    //Initialize the default variables
    int verbosity = 2;

    // ToDo ///////////////////////////////////////////////////////
    // Check the main arguments and use them to set up the appli-
    // cation context. Set the corresponding default values above
    // and pass them to the corresponding functions (eg. verbosity)
    ///////////////////////////////////////////////////////////////

    //Initialize the message handler
    XmlEditorMessageHandler *msgHandler = initMsgHandler(verbosity);

    //Catch the interruption signals
    signal(SIGABRT, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGSEGV, signalHandler);
    signal(SIGFPE, signalHandler);

    //Initialize the application's context
    initConfig();
    initPaths();
    initLanguages();

    //Initialize the main window
    EditorPanel editor;

    //Show the main window
    editor.show();

    //Start the application
    int exitCode = app.exec();

    //Flush the message handler streams
    msgHandler->flush();

    //Uncatch the interruption signals
    signal(SIGABRT, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
    signal(SIGSEGV, SIG_DFL);
    signal(SIGFPE, SIG_DFL);

    //Return the exit code
    return exitCode;
}

//Initializes the messages handler
XmlEditorMessageHandler *initMsgHandler(int verbosity) {

    //Create a new XDE message handler
    XmlEditorMessageHandler *msgHandler = new XmlEditorMessageHandler();

    //Assign it to the Core
    CORE->setMessageHandler(msgHandler);

    //Set the message handler log file
    msgHandler->setLogFile(QDir::tempPath() + "/xmleditor.log");

    //Set the console verbosity
    switch (verbosity) {
    case 0:
        msgHandler->setConsoleOutput(XmlEditorMessageHandler::COLOR | XmlEditorMessageHandler::ERRORS);
        msgHandler->setLogOutput(XmlEditorMessageHandler::HTMLCOLOR | XmlEditorMessageHandler::ERRORS | XmlEditorMessageHandler::WARNINGS);
        break;
    case 1:
        msgHandler->setConsoleOutput(XmlEditorMessageHandler::COLOR | XmlEditorMessageHandler::ERRORS | XmlEditorMessageHandler::WARNINGS);
        msgHandler->setLogOutput(XmlEditorMessageHandler::HTMLCOLOR | XmlEditorMessageHandler::ERRORS | XmlEditorMessageHandler::WARNINGS | XmlEditorMessageHandler::DEBUGS | XmlEditorMessageHandler::GENERIC | XmlEditorMessageHandler::TRACE);
        break;
    case 2:
        msgHandler->setConsoleOutput(XmlEditorMessageHandler::COLOR | XmlEditorMessageHandler::ERRORS | XmlEditorMessageHandler::WARNINGS | XmlEditorMessageHandler::DEBUGS | XmlEditorMessageHandler::GENERIC | XmlEditorMessageHandler::TRACE);
        msgHandler->setLogOutput(XmlEditorMessageHandler::HTMLCOLOR | XmlEditorMessageHandler::ALL);
        break;
    case 3:
        msgHandler->setConsoleOutput(XmlEditorMessageHandler::COLOR | XmlEditorMessageHandler::ALL);
        msgHandler->setLogOutput(XmlEditorMessageHandler::HTMLCOLOR | XmlEditorMessageHandler::ALL);
        break;
    }

    return msgHandler;
}

//Initializes the configuration
void initConfig() {

    // ToDo ///////////////////////////////////////////////////////
    // Unset temporary configuration when in beta or release build,
    // and add a possibility to use application argument to set it
    ///////////////////////////////////////////////////////////////

    CORE->setTemporaryConf();

    // ToDo ///////////////////////////////////////////////////////
    // Review and refactor the whole configuration management and
    // use enum in config instead of the DEFINE used below
    ///////////////////////////////////////////////////////////////

    CONFIG->declare("main", LANGUAGE, QVariant("en"));
}

//Initializes the paths
void initPaths() {

    // ToDo ///////////////////////////////////////////////////////
    // When done with config refactor, check if this is still needed
    ///////////////////////////////////////////////////////////////

#ifdef Q_OS_MACX
    CORE->setPath(Core::ApplicationData, CORE->path(Core::Executable) + "../Resources/");
#else
    CORE->setPath(Core::ApplicationData, CORE->path(Core::Executable));
#endif
    CORE->setPath(Core::Drugs, "drugs", true);
    CORE->setPath(Core::CustomDrugs, "custom", true);
    CORE->setPath(Core::Translations, "trans", true);
    CORE->setPath(Core::Plugins, "plugins", true);
    CORE->setPath(Core::PluginsData, "data", true);
}

//Initializes the languages
void initLanguages() {

    //Get the last used language from the config
    QString language = CONFIG->getString(LANGUAGE);

    // ToDo ///////////////////////////////////////////////////////
    // Load the translation files depending on the current language
    // If the last used language is not available, try to load the
    // local language. If not available, used the english (en).
    ///////////////////////////////////////////////////////////////

    CORE->setLanguage(language);
}

//Manages the interruption signals
void signalHandler(int interruptId) {
    STRACE("sig handler", QObject::tr("Interrupted by signal %1").arg(interruptId));
}
