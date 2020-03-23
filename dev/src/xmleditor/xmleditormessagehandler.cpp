#include "xmleditormessagehandler.h"
#include "version.h"

#include <cstdlib>
#include <signal.h>

#include <QDateTime>
#include <QDebug>
#include <QMessageBox>
#include <QMainWindow>
#include <QStatusBar>

//Used to print the trace on linux
#ifdef Q_OS_LINUX
#include <execinfo.h>
#endif

//Default constructor
XmlEditorMessageHandler::XmlEditorMessageHandler () : log(0), logFile(0)
{
    //Initiate the standard and error streams
    std = new QTextStream(stdout);
    err = new QTextStream(stderr);
}

//Default destructor
XmlEditorMessageHandler::~XmlEditorMessageHandler ()
{
    //Flush the streams
    flush();

    //Close and delete the log file
    if (logFile) {
        if (logFile->isOpen())
            logFile->close();
        delete logFile;
    }
}

//Prints a core error message
void XmlEditorMessageHandler::error(CoreError e, int line, const char *file, const char *object, QString msg)
{
    error(CoreErrorInfo::name(e), CoreErrorInfo::desc(e), e, line, file, object, msg);
    exit(e);
}

//Prints an XML editor error message
void XmlEditorMessageHandler::error(XmlEditorError e, int line, const char *file, const char *object, QString msg)
{
    error(XmlEditorErrorInfo::name(e), XmlEditorErrorInfo::desc(e), e, line, file, object, msg);
    exit(e);
}

//Prints the call stack
void XmlEditorMessageHandler::trace(int line, const char *file, const char *object, QString msg)
{
    QString stack = trace();

    //If the TRACE flag is enabled for the std, print it
    if (outFlags.testFlag(TRACE))
        *std << stack;

    //If the  TRACE flag is enable for the log, print it
    if (logFlags.testFlag(TRACE))
        writeLog(stack);

    //Send the corresponding core error
    error(CoreErrorInfo::name(INTERRUPTED), CoreErrorInfo::desc(INTERRUPTED), INTERRUPTED, line, file, object, msg);

    //On windows, exit, for the others, kill all threads in this process
#ifdef Q_OS_WIN
    exit(0);
#else
    kill(0,9);
#endif
}

//Prints a warning message
void XmlEditorMessageHandler::warning (int line, const char *file, const char *object, QString msg)
{
    //Console output
    if (outFlags.testFlag(WARNINGS))
        *std << color(WARNINGS) << "[" << object << "] " << tr("WARNING") << color() << " " << msg << "\n" << debugInfo(outFlags,line,file,object);
    std->flush();

    //Log file output
    if (logFlags.testFlag(WARNINGS))
        writeLog(color(WARNINGS,true) + "[" + object + "] " + tr("WARNING") + color(COLOR,true) + " " + msg + "<br />\n" + debugInfo(logFlags,line,file,object) + "<br />\n");
}

//Prints a debug message
void XmlEditorMessageHandler::debug (int line, const char *file, const char *object, QString msg)
{
    //Console output
    if (outFlags.testFlag(DEBUGS))
        *std << color(DEBUGS) << "[" << object << "] " << color() << msg << "\n";
    if (outFlags.testFlag(DEBUGSLINE))
        *std << debugInfo(outFlags,line,file,object);
    std->flush();

    //Log file output
    if (logFlags.testFlag(DEBUGS))
        writeLog(color(DEBUGS,true) + "[" + object + "] " + color(COLOR,true) + msg + "<br />\n");
    if (logFlags.testFlag(DEBUGSLINE))
        writeLog(debugInfo(logFlags,line,file,object) + "<br />\n");
}

//Prints a simple message
void XmlEditorMessageHandler::message (QString msg)
{
    *std << color(MESSAGES) << msg << color() << "\n";
    std->flush();
}

//Sets the log file
void XmlEditorMessageHandler::setLogFile (QString file)
{
    qDebug() << "Opening the log file " + file;

    //Close the previous log file if any
    if (logFile) {
        if (logFile->isOpen()) {
            log->flush();
            logFile->close();
        }
        delete logFile;
    }

    //Set the new log file name
    logFile = new QFile(file);

    //Open the log
    if (!logFile->open(QIODevice::WriteOnly))
        ERROR(FILENOTFOUND, tr("Cannot open the specified log file") + " : " + file);

    //Set the stream
    log = new QTextStream(logFile);

    //Print a welcome message
    *log << "\n"<< tr("XML Editor version ") << CORE->version(Core::Editor).toString() << " (build " << CORE->buildID() << "), " << __DATE__ << "<br />\n";
    *log << tr("XML Editor's user interface, launched on ") << QDateTime::currentDateTime().toString() << "<br />\n";
}

//Returns the log file name
QString XmlEditorMessageHandler::getLogFile()
{
    return logFile->fileName();
}

//Sets the flags for the console
void XmlEditorMessageHandler::setConsoleOutput (OutputFlags flags)
{
    outFlags = flags;
}

//Sets the flags for the log file
void XmlEditorMessageHandler::setLogOutput (OutputFlags flags)
{
    logFlags = flags;
}

//Flushes the streams
void XmlEditorMessageHandler::flush ()
{
    std->flush();
    err->flush();
    if (log)
        log->flush();
}

//Writes in the log file
void XmlEditorMessageHandler::writeLog (QString msg)
{
    //Skip if not available
    if (!log)
        return;

    //Write the message and flush
    *log << msg;
    log->flush();
}

//Sets up the debug info
QString XmlEditorMessageHandler::debugInfo (OutputFlags flags, int line, const char *file, const char*)
{
    //Test DEBUGS flag
    if (! flags.testFlag(DEBUGS))
        return "";

    //Construct and return the corresponding output
    return tr("At line %2 from file %1\n").arg(file).arg(line);
}

//Prints an error message and quit
void XmlEditorMessageHandler::error (QString e, QString desc, int, int line, const char* file, const char* object, QString msg)
{
    //Console output
    if (outFlags.testFlag(ERRORS)) {
        *err << color(ERRORS) << "[" << object << "] " << e << color() << " : " << color(MESSAGES) << msg << color() << "\n" << debugInfo(outFlags,line,file,object);

        //Stack trace if wanted
        if (outFlags.testFlag(ALWAYSTRACE))
            *std << trace();

        //Additionnal error description
        if (outFlags.testFlag(GENERIC))
            *err << tr("Generic description") << " : " << desc << "\n";
    }

    //Log file output
    if (logFlags.testFlag(ERRORS)) {
        writeLog(color(ERRORS,true) + "[" + object + "] " + e + color(COLOR,true) + " : " + msg + "<br />\n" + debugInfo(logFlags,line,file,object));

        //Stack trace if wanted
        if (logFlags.testFlag(ALWAYSTRACE))
            writeLog(trace());

        //Additionnal error description
        if (outFlags.testFlag(GENERIC))
            writeLog(tr("Generic description") + " : " + desc + "<br />\n");
    }

    //Flush the streams
    flush();
}

//Produces a trace of the call stack (Linux only)
QString XmlEditorMessageHandler::trace ()
{
    QString str = "";

    //Trace only on linux
#ifdef Q_OS_LINUX

    //Stack trace
    void *pointers[MAX_TRACE_SIZE];
    size_t size;

    //Retrieve the call pointers
    size = backtrace(pointers, MAX_TRACE_SIZE);

    //Retrieve the call names
    char **text;
    text = backtrace_symbols(pointers, size);

    //Convert to QString
    for (size_t i=0; i<size; i++)
        str += text[i] + QString("\n");
    str += "\n";
#endif

    return str;
}

//Return the color start/end flags for the Linux terminal or in HTML
QString XmlEditorMessageHandler::color(OutputFlag msg_type, bool html)
{
    //Set the HTML colors, for the logs or the web
    if (outFlags.testFlag(HTMLCOLOR) || html) {
        switch (msg_type) {
            //Reset color
        case COLOR : return "</span>";
            //Debug color (blue)
        case DEBUGS : return "<span style='color:#2222FF'>";
            //Warning color (yellow)
        case WARNINGS : return "<span style='color:orange'>";
            //Error color (red)
        case ERRORS : return "<span style='color:red'>";
            //Messages color (light white)
        case MESSAGES: return "<span style='color:white'>";
            //Default, return nothing
        default : return "";
        }
    }

    //Set the console color (Linux only)
#ifndef Q_OS_LINUX
    return "";
#else
    //Test the color flag first
    if (!outFlags.testFlag(COLOR))
        return "";

    //Set the terminal color
    switch (msg_type) {
        //Reset color
    case COLOR : return "\033[0m";
        //Debug color (blue)
    case DEBUGS : return "\033[1;34m";
        //Warning color (yellow)
    case WARNINGS : return "\033[1;33m";
        //Error color (red)
    case ERRORS : return "\033[1;31m";
        //Messages color (light white)
    case MESSAGES: return "\033[1;37m";
        //Default, return nothing
    default : return "";
    }
#endif
}
