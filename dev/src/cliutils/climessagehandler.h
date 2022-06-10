//@@license@@

#ifndef CLIMESSAGEHANDLER_H
#define CLIMESSAGEHANDLER_H

#include "core.h"
#include "cli_errors.h"
#include "core_errors.h"
#include "coremessagehandler.h"

#include <typeinfo>

#include <QString>
#include <QObject>
#include <QTextStream>
#include <QMutex>
#include <QFlags>
#include <QFile>

using namespace Tucuxi::Gui::Core::Core;

//! Use this macro to send an error and quit (both CLI and Core errors)
//! doesnt work in static methods
//#undef ERROR
//#define ERROR(e,m) static_cast<CliMessageHandler*>(CORE->messageHandler())->error(e,__LINE__,__FILE__,typeid(this).name(),QString(m))
////! Use this macro to send a warning message
//#undef WARNING
//#define WARNING(m) static_cast<CliMessageHandler*>(CORE->messageHandler())->warning(__LINE__,__FILE__,typeid(this).name(),QString(m))
////! Use this macro to send a debug message
//#undef DEBUG
//#define DEBUG(m) static_cast<CliMessageHandler*>(CORE->messageHandler())->debug(__LINE__,__FILE__,typeid(this).name(),QString(m))
////! Use this macro to send a result message
//#undef MESSAGE
//#define MESSAGE(m) static_cast<CliMessageHandler*>(CORE->messageHandler())->message(QString(m))
////! Print the message followed by a stack trace, and quit
//#undef TRACE
//#define TRACE(e,m) static_cast<CliMessageHandler*>(CORE->messageHandler())->trace(e,__LINE__,__FILE__,typeid(this).name(),QString(m))

//// Re-definition of the macro for non-object
//#undef SERROR
//#define SERROR(n,e,m) static_cast<CliMessageHandler*>(CORE->messageHandler())->error(e,__LINE__,__FILE__,n,QString(m))
//#undef SWARNING
//#define SWARNING(n,m) static_cast<CliMessageHandler*>(CORE->messageHandler())->warning(__LINE__,__FILE__,n,QString(m))
//#undef SDEBUG
//#define SDEBUG(n,m) static_cast<CliMessageHandler*>(CORE->messageHandler())->debug(__LINE__,__FILE__,n,QString(m))
//#undef STRACE
//#define STRACE(n,e,m) static_cast<CliMessageHandler*>(CORE->messageHandler())->trace(e,__LINE__,__FILE__,n,QString(m))

//Maximum trace size (number of callings)
//#define MAX_TRACE_SIZE 100

//! \brief The CLI message handler
/*! \ingroup cli
   This message handler is able to print either on the console or on a file, or both.
   In addition to the debug, warning and error message needed to be handled, a call backtrace and simple messages are handled.
   The trace() function will print the call stack, while the message() function will simple print the given message in the console.
   Each kind of message can be enabled or disabled independantly in the console or in the log file.
   To enable the log file output, simply set the log file output messages to show (setLogOutput()) and define the log file (setLogFile()).
   In the console and in a Linux environment, the message are printed in color.
  */

namespace Tucuxi {
namespace cliutils {
class CliMessageHandler : public Tucuxi::Gui::Core::CoreMessageHandler
{
    Q_OBJECT
public:
    //! \brief Kind of message to output
    /*!
     */
    enum OutputFlag {
        //! Show the warning messages
        WARNINGS = 0x1,
        //! Show the errors
        ERRORS = 0x2,
        //! Show the debug messages and various debug informations
        DEBUGS = 0x4,
        //! Show the generic error message (translated)
        GENERIC = 0x8,
        //! Show the call stack
        TRACE = 0x10,
        //! Show the debug line number and file
        DEBUGSLINE = 0x20,
        //! Show a trace on every error
        ALWAYSTRACE = 0x40,
        //! Console output in color
        COLOR = 0x80,
        //! Messages, for the color
        MESSAGES = 0x100,
        //! Output in HTML colors
        HTMLCOLOR = 0x200,
        //! Print all message types
        ALL = ERRORS | WARNINGS | DEBUGS | GENERIC | TRACE | DEBUGSLINE | ALWAYSTRACE
    };
    Q_DECLARE_FLAGS(OutputFlags, OutputFlag)

    //! \brief Constructor
    /*!
     */
    CliMessageHandler();
    ~CliMessageHandler();

    //! \brief Print a Core error message
    /*! This function is for the Core error messages.
      The message will be printed on the console and on the log file, depending on the outputFlags set.
      After the message being printed, the application will quit with the given error code.
      @param e The error code
      @param line The line where the error was thrown (defined at compilation time)
      @param file The file where the error was thrown (defined at compilation time)
      @param object The name of the object throwning the error (defined dynamically)
      @param msg The message explaining the error
     */
    void error (CoreError e, int line, const char* file, const char* object, QString msg);

    //! \brief Print a CLI error message
    /*! Same as the precedent, but for the CLI errors.
      @param e The error code
      @param line The line where the error was thrown (defined at compilation time)
      @param file The file where the error was thrown (defined at compilation time)
      @param object The name of the object throwning the error (defined dynamically)
      @param msg The message explaining the error
     */
    void error (CliError e, int line, const char* file, const char* object, QString msg);

    //! \brief Print the call stack
    /*! The call stack is retrieved using the backtrace() functions in execinfo.h.
      After the call stack is printed, the error is showed ans the program is exited.
      @param e The error code
      @param line The line where the error was thrown (defined at compilation time)
      @param file The file where the error was thrown (defined at compilation time)
      @param object The name of the object throwning the error (defined dynamically)
      @param msg The message explaining the error
     */
    void trace (CliError e, int line, const char* file, const char* object, QString msg);
    void trace (CoreError e, int line, const char* file, const char* object, QString msg);

    //! \brief Print a warning message
    /*! The message is simply being printed, in color if available.
      @param line The line where the warning was sent (defined at compilation time)
      @param file The file where the warning was sent (defined at compilation time)
      @param object The name of the object sending the warning (defined dynamically)
      @param msg The message explaining the error
     */
    void warning (int line, const char* file, const char* object, QString msg);

    //! \brief Print a debug message
    /*! The message is simply being printed, in color if available.
      @param line The line where the debug message was sent (defined at compilation time)
      @param file The file where the debug message was sent (defined at compilation time)
      @param object The name of the object sending the debug message (defined dynamically)
      @param msg The message explaining the error
     */
    void debug (int line, const char* file, const char* object, QString msg);

    //! \brief Print a message in the console
    /*! Considered as the output of the program, the messages are always printed in the console and never printed in the log file, as they may include sensitive data.
      @param msg The message to print
     */
    void message (QString msg);

    //! \brief Specifies the log file
    /*! Until this function is called, nothing is printed on the log file.
      Before any message is printed, the Ezechiel build and build date along with the time when the writing began is written.
      @param logfile File where to write messages
     */
    void setLogFile(QString file);

    //! \brief Set the console output mode
    /*! Display these kind of informations in the console.
      @param flags Set of data to display
     */
    void setConsoleOutput(OutputFlags flags);

    //! \brief Set the log file output mode
    /*! Write these kind of informations in the log file.
      @param flags Set of data to write in the log file
     */
    void setLogOutput(OutputFlags flags);

    //! \brief Flush the output
    /*! Write all the messages stored in the stream, in the console output and in the log file.
     */
    void flush();

private:
    //Standard output stream
    QTextStream *std;
    //Error output stream
    QTextStream *err;
    //Log file
    QFile *logFile;
    //Stream for the log file
    QTextStream *log;
    //Console output set of flags
    OutputFlags outFlags;
    //Log file output set of flags
    OutputFlags logFlags;

    //Write this data to the log
    void writeLog (QString msg);
    //Returns a string containing the given object name, line and file but only if the given set of flags contains DEBUGS
    QString debugInfo (OutputFlags flags, int line, const char* file, const char* object);
    //Write the error output using this error string and description, then quit
    void error (QString e, QString desc, int num, int line, const char* file, const char* object, QString msg);
    //Produce a trace of the call stack
    QString trace ();
    //Return the desired color code or the reset color if nothing or COLOR is given
    QString color(OutputFlag msg_type = COLOR);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CliMessageHandler::OutputFlags)
} //namespace cliutils
} //namespace Tucuxi

#endif // CLIMESSAGEHANDLER_H
