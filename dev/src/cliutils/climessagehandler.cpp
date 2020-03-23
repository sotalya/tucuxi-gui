#include "climessagehandler.h"
#include "logging.h"
#include "errors_cliutils.h"

//Trace only on linux
#ifdef Q_OS_LINUX
#include <execinfo.h>
#endif

#include <cstdlib>

#include <QDateTime>
#include <QDebug>


namespace ezechiel {
namespace cliutils {
/* Initiate the standard and error streams
  */
CliMessageHandler::CliMessageHandler ()
    :  log(0),
      logFile(0)
{
    //Streams
    std = new QTextStream(stdout);
    err = new QTextStream(stderr);
}

/* Flush all the streams
   If open, close the log file
  */
CliMessageHandler::~CliMessageHandler ()
{
    //Flush streams
    flush();
    //Log file close & flush
    if (logFile) {
        if (logFile->isOpen())
            logFile->close();
        delete logFile;
    }
}

// Call the internal error()
void CliMessageHandler::error (CliError e, int line, const char *file, const char *object, QString msg)
{
    error (CliErrorInfo::name(e),CliErrorInfo::desc(e),e,line,file,object,msg);
}

// Call the internal error()
void CliMessageHandler::error (CoreError e, int line, const char *file, const char *object, QString msg)
{
    error (CoreErrorInfo::name(e),CoreErrorInfo::desc(e),e,line,file,object,msg);
}

/* If the TRACE flag is enabled
      Create the stack trace
      Print the stack trace
   Print the error message
   */
void CliMessageHandler::trace(CliError e, int line, const char *file, const char *object, QString msg)
{
    QString stack = trace();
    //Print the stack trace
    if (outFlags.testFlag(TRACE))
        *std << stack;
    if (logFlags.testFlag(TRACE))
        writeLog(stack);
    //Launch the error
    error(e,line,file,object,msg);
}

void CliMessageHandler::trace(CoreError e, int line, const char *file, const char *object, QString msg)
{
    QString stack = trace();
    //Print the stack trace
    if (outFlags.testFlag(TRACE))
        *std << stack;
    if (logFlags.testFlag(TRACE))
        writeLog(stack);
    //Launch the error
    error(e,line,file,object,msg);
}

QString CliMessageHandler::trace ()
{
    QString str = "";
    //Trace only on linux
#ifdef Q_OS_LINUX
    //Stack trace
//    void *pointers[MAX_TRACE_SIZE];
//    size_t size;
    //Retrieve the call pointers
//    size = backtrace(pointers, 10);
    //Retrieve the call names
//    char **text;
//    text = backtrace_symbols(pointers, size);
    //Convert to QString
//    for (uint i=0; i<size; i++)
//        str += text[i] + QString("\n");
//    str += "\n";
#endif
    return str;
}

void CliMessageHandler::warning (int line, const char *file, const char *object, QString msg)
{
    //Console output
    if (outFlags.testFlag(WARNINGS)) {
        *std << color(WARNINGS) << "[" << object << "] " << tr("WARNING") << color() << " " << msg << "\n" << debugInfo(outFlags,line,file,object);
        std->flush();
    }
    //Log file output
    if (logFlags.testFlag(WARNINGS))
        writeLog(QString("[") + object + "] " + tr("WARNING") + msg + "\n" + debugInfo(logFlags,line,file,object));
}

void CliMessageHandler::debug (int line, const char *file, const char *object, QString msg)
{
    //Console output
    if (outFlags.testFlag(DEBUGS))
        *std << color(DEBUGS) << "[" << object << "] " << color() << msg << "\n";
    if (outFlags.testFlag(DEBUGSLINE))
        *std << debugInfo(outFlags,line,file,object);
    //Log file output
    if (logFlags.testFlag(DEBUGS))
        writeLog(QString("[") + object + "] " + msg + "\n");
    if (logFlags.testFlag(DEBUGSLINE))
        writeLog(debugInfo(logFlags,line,file,object));
}

//The messages are only printed on the console, important informations cannot be written in the log file
void CliMessageHandler::message (QString msg)
{
    *std << color(MESSAGES) << msg << color() << "\n";
    std->flush();
}

/* If the log file was open
      Close it
   Set the log file name
   Open the log file
   Associate the log stream with the log file
 */
void CliMessageHandler::setLogFile (QString file)
{
    qDebug() << "Opening the log file : "+file;
    //Close the old log file
    if (logFile) {
        if (logFile->isOpen()) {
            log->flush();
            logFile->close();
            delete logFile;
        }
    }
    //Set the new log file name
    logFile = new QFile(file);
    //Open the log
    if (!logFile->open(QIODevice::Append))
        LOG(QtCriticalMsg, FILENOTFOUND, tr("Cannot open the specified log file") + " : " + file);
    //Set the stream
    log = new QTextStream(logFile);
    //Welcome message
    *log<<"\n"<<tr("Ezechiel version : ")<<CORE->buildID()<<", "<<__DATE__<<"\n";
    *log<<tr("Ezechiel CLI interface, launched on ")<<QDateTime::currentDateTime().toString()<<"\n";
}

// Set the flags
void CliMessageHandler::setConsoleOutput (OutputFlags flags)
{
    outFlags = flags;
}

// Set the flags
void CliMessageHandler::setLogOutput (OutputFlags flags)
{
    logFlags = flags;
}

//Flush the 3, and the 2, and the 1-2-3-4 \o/ \\o o// ~o~ .o/
void CliMessageHandler::flush ()
{
    std->flush();
    err->flush();
    if (log)
        log->flush();
}

/* If the textstream is invalid
      return
   Write in the log stream
 */
void CliMessageHandler::writeLog (QString msg)
{
    if (!log)
        return;
    *log<<msg;
}

/* If the debug flag is present
      Return the values
  */
QString CliMessageHandler::debugInfo (OutputFlags flags, int line, const char *file, const char*)
{
    //Test DEBUGS flag
    if (! flags.testFlag(DEBUGS))
        return "";
    //Construct & return the output
    return tr("from")+" "+file+" "+tr("at line")+" "+QString::number(line)+"\n";
}

// Prints the error message and then quit
void CliMessageHandler::error (QString e, QString desc, int num, int line, const char* file, const char* object, QString msg)
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
        std->flush();
        err->flush();
    }
    //Log file output
    if (logFlags.testFlag(ERRORS)) {
        writeLog(QString("[") + object + "] " + e + " : " + msg + "\n" + debugInfo(logFlags,line,file,object));
        //Stack trace if wanted
        if (logFlags.testFlag(ALWAYSTRACE))
            writeLog(trace());
        //Additionnal error description
        if (outFlags.testFlag(GENERIC))
            writeLog(tr("Generic description") + " : " + desc + "\n");
    }
    //Flush the streams
    flush();
    //Quit, with the given error code
    exit(num);
}

//Return the color start/end flags for the Linux terminal or in HTML
QString CliMessageHandler::color(OutputFlag msg_type)
{
    //Web
    if (outFlags.testFlag(HTMLCOLOR)) {
        switch (msg_type) {
        //Reset color
        case COLOR : return "</span>";
            //Debug color (blue)
        case DEBUGS : return "<span style='color:#2222FF'>";
            //Warning color (yellow)
        case WARNINGS : return "<span style='color:yellow'>";
            //Error color (red)
        case ERRORS : return "<span style='color:red'>";
            //Messages color (light white)
        case MESSAGES: return "<span style='color:white'>";
            //Default, return nothing
        default : return "";
        }
    }

    //Linux terminal color
    //If not on linux, return an empty string
#ifndef Q_OS_LINUX
    return "";
#else
    //If the COLOR flag is not activated in the outFlags, no color
    if (!outFlags.testFlag(COLOR))
        return "";
    //Terminal colors
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

} //namespace cliutils
} //namespace ezechiel
