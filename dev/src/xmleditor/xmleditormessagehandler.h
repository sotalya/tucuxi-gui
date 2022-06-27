#ifndef XMLEDITORMESSAGEHANDLER_H
#define XMLEDITORMESSAGEHANDLER_H

#include "coremessagehandler.h"
#include "core.h"
#include "xmleditorerrors.h"
#include "core_errors.h"

#include <typeinfo>

#include <QString>
#include <QTextStream>
#include <QFlags>
#include <QFile>

//! Use this macro to send an error and quit (both Editor and Core errors)
#undef ERROR
#define ERROR(e, m) static_cast<XmlEditorMessageHandler *>(CORE->messageHandler())->error(e, __LINE__, __FILE__, typeid(this).name(), QString(m))

//! Use this macro to send a warning message
#undef WARNING
#define WARNING(m) static_cast<XmlEditorMessageHandler *>(CORE->messageHandler())->warning(__LINE__, __FILE__, typeid(this).name(), QString(m))

//! Use this macro to send a debug message
#undef DEBUG
#define DEBUG(m) static_cast<XmlEditorMessageHandler *>(CORE->messageHandler())->debug(__LINE__, __FILE__, typeid(this).name(), QString(m))

//! Use this macro to print a message followed by a stack trace, and quit
#undef TRACE
#define TRACE(m) static_cast<XmlEditorMessageHandler *>(CORE->messageHandler())->trace(__LINE__, __FILE__, typeid(this).name(), QString(m))

//! Use this macro to send a result message
#undef MESSAGE
#define MESSAGE(m) static_cast<XmlEditorMessageHandler *>(CORE->messageHandler())->message(QString(m))

//Re-definition of the macros for non-object
#undef SERROR
#define SERROR(n, e, m) static_cast<XmlEditorMessageHandler *>(CORE->messageHandler())->error(e,__LINE__, __FILE__, n, QString(m))

#undef SWARNING
#define SWARNING(n, m) static_cast<XmlEditorMessageHandler *>(CORE->messageHandler())->warning(__LINE__, __FILE__, n, QString(m))

#undef SDEBUG
#define SDEBUG(n, m) static_cast<XmlEditorMessageHandler *>(CORE->messageHandler())->debug(__LINE__, __FILE__, n, QString(m))

#undef STRACE
#define STRACE(n, m) static_cast<XmlEditorMessageHandler *>(CORE->messageHandler())->trace(__LINE__, __FILE__, n, QString(m))

//! Use this macro to get an instance of this message handler
#define MESSAGE_HANDLER static_cast<XmlEditorMessageHandler *>(CORE->messageHandler())

//Maximum trace size (number of callings)
#define MAX_TRACE_SIZE 100

//Number of milliseconds to show the warning in the status bar
#define STATUS_TIMEOUT 10000

/** \ingroup xmleditor message
 *  \brief The XML Editor message handler
 *
 * This message handler is able to print either on the console or on a file, or both.
 * In addition to the debug, warning and error message needed to be handled, a call backtrace and simple messages are handled.
 * The trace() function will print the call stack, while the message() function will simple print the given message in the console.
 * Each kind of message can be enabled or disabled independantly in the console or in the log file.
 * To enable the log file output, simply set the log file output messages to show (setLogOutput()) and define the log file (setLogFile()).
 * In the console and in a Linux environment, the message are printed in color.
 */
class XmlEditorMessageHandler : public CoreMessageHandler
{
    Q_OBJECT

public:
    //! \brief Messages types
    enum OutputFlag {
        //! Warning messages
        WARNINGS = 0x1,
        //! Errors messages
        ERRORS = 0x2,
        //! Debug messages and various debug informations
        DEBUGS = 0x4,
        //! Generic error message (translated)
        GENERIC = 0x8,
        //! Call stack content
        TRACE = 0x10,
        //! Debug line number and file
        DEBUGSLINE = 0x20,
        //! Trace on all errors
        ALWAYSTRACE = 0x40,
        //! Console color
        COLOR = 0x80,
        //! Message color
        MESSAGES = 0x100,
        //! HTML color
        HTMLCOLOR = 0x200,
        //! All messages types
        ALL = ERRORS | WARNINGS | DEBUGS | GENERIC | TRACE | DEBUGSLINE | ALWAYSTRACE
    };
    Q_DECLARE_FLAGS(OutputFlags, OutputFlag)    // == typedef QFlags<OutputFlag> OutputFlags

    //! Default constructor
    XmlEditorMessageHandler();

    //! Default destructor
    ~XmlEditorMessageHandler();

    //! Prints a Core error message.
    /** Prints an error message in the console and in the log file, depending on the outputFlags set.
     * After the message is printed, the application will quit returning the given error code.
     * @param e The error code
     * @param line The line where the error was thrown (defined at compilation time)
     * @param file The file where the error was thrown (defined at compilation time)
     * @param object The name of the object throwning the error (defined dynamically)
     * @param msg The message explaining the error
     */
    void error (CoreError e, int line, const char* file, const char* object, QString msg);

    //! Prints an XML editor's error message.
    /** Prints a warning message in the console and in the log file, depending on the outputFlags set.
     * @param e The error code
     * @param line The line where the error was thrown (defined at compilation time)
     * @param file The file where the error was thrown (defined at compilation time)
     * @param object The name of the object throwning the error (defined dynamically)
     * @param msg The message explaining the error
     */
    void error (XmlEditorError e, int line, const char* file, const char* object, QString msg);

    //! Prints the call stack.
    /** Prints the call stack retrieved using the backtrace() functions in execinfo.h.
     * After the call stack is printed, the error is showed and the program stops.
     * @param e The error code
     * @param line The line where the error was thrown (defined at compilation time)
     * @param file The file where the error was thrown (defined at compilation time)
     * @param object The name of the object throwning the error (defined dynamically)
     * @param msg The message explaining the error
     */
    void trace (int line, const char* file, const char* object, QString msg);

    //! Prints a warning message.
    /** Prints a warning message, in color if available.
     * @param line The line where the warning was sent (defined at compilation time)
     * @param file The file where the warning was sent (defined at compilation time)
     * @param object The name of the object sending the warning (defined dynamically)
     * @param msg The message explaining the error
     */
    void warning (int line, const char* file, const char* object, QString msg);

    //! Prints a debug message.
    /** Prints a debug message, in color if available.
     * @param line The line where the debug message was sent (defined at compilation time)
     * @param file The file where the debug message was sent (defined at compilation time)
     * @param object The name of the object sending the debug message (defined dynamically)
     * @param msg The message explaining the error
     */
    void debug (int line, const char* file, const char* object, QString msg);

    //! Prints a simple message in the console.
    /** Considered as the output of the program, the messages are always printed in the console and never printed in the log file, as they may include sensitive data.
     * @param msg The message to print
     */
    void message (QString msg);

    //! Sets the log file.
    /** Unless this function is called, nothing is printed in the log file.
     * Before any message is printed, the Tucuxi build and build date along with the time when the writing began is written.
     * @param logfile File where to write messages
     */
    void setLogFile(QString file);

    //! Retrieves the log file name.
    QString getLogFile ();

    /** \brief Sets the console output mode.
     * @param flags The output flag(s)
     */
    void setConsoleOutput(OutputFlags flags);

    /** \brief Sets the log file output mode.
     * @param flags The output flag(s)
     */
    void setLogOutput(OutputFlags flags);

    /** \brief Flushes the output.
     * Writes all the messages stored in the stream, in the console output and in the log file.
     */
    void flush();

private:
    //Writes data to the log file
    void writeLog (QString msg);

    //Returns a string containing the given object name, line and file but only if the given set of flags contains DEBUGS
    QString debugInfo (OutputFlags flags, int line, const char* file, const char* object);

    //Write the error output using this error string and description, then quit
    void error (QString e, QString desc, int num, int line, const char* file, const char* object, QString msg);

    //Produce a trace of the call stack (Linux only)
    QString trace ();

    /* Return the desired color code or the reset color if nothing or COLOR is given. The
       html parameter can be used to retrieve the html color, even if not set in the flags */
    QString color(OutputFlag msg_type = COLOR, bool html = false);

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
};

//Declares global operator|() function for XmlEditorMessageHandler::OutputFlags, which is of type QFlags<OutputFlag>
Q_DECLARE_OPERATORS_FOR_FLAGS(XmlEditorMessageHandler::OutputFlags)

#endif // XMLEDITORMESSAGEHANDLER_H
