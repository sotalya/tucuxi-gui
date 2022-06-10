//@@license@@

#ifndef COREMESSAGEHANDLER_H
#define COREMESSAGEHANDLER_H

//#include "core_errors.h"

#include <QString>

namespace Tucuxi {
namespace Gui {
namespace Core {


/** \defgroup message Messaging system
  * The \ref core is independant of the interfaces (GUI, CLI) and so do not know how the message will be handled.
  * For example, the CLI might want to crash on error because no interaction is done with the user, while the GUI might want to display the error but continue to work anyway.
  * A flexible way to handle the message is so necessary.
  * The message system is made of an interface, describing which kind of messages the \ref core need, and as many implementing classes as needed for each interface.
  *
  * Moreover, defines are used to provide more data about the current line, file and object used in a way that does no require anything from the user.
  * The user gives only the message (or a message and an error code for the errors), and the compiler create the produce all the other data.
  * Each interface should re-define these defines to adapt them to its own interface.
  *
  * The Core object handles the message singleton. To be used, an interface must register itself as soon as possible in the Core object.
  */

//! \brief Handles the message for the Core part
/*! \ingroup core
   This handler is nothing but a minimal interface insuring that the Core message type (debug, error, warning) are handled by any implementing handler.
   All fonctions are virtual, by itself Core does nothing of its errors.
   An executable using the Core library MUST implement a message handler and install it BEFORE any of the Core library component is loaded, or a segmentation error will occur.
   It is asked that no valuable administrative information is given in these messages.
  */
class CoreMessageHandler : public QObject
{
   //Q_OBJECT

public:
   //! \brief Handle an error message
   /*! An error message is thrown when an irrecoverable event occur.
      In this case, the function throwning the error will most likely stop.
      Thus, it is recommended to exit or properly stop the involved action.
      @param e The corresponding error and error code
      @param line The line where the error was thrown (defined at compilation time)
      @param file The file where the error was thrown (defined at compilation time)
      @param object The name of the object throwning the error (defined dynamically)
      @param msg The message explaining the error
     */
   virtual void error (CoreError e, int line, const char* file, const char* object, QString msg) = 0;

   //! \brief Handle a warning message
   /*! Warning messages are sent when something strange append, but the execution can continue anyway.
      @param line The line where the warning was sent (defined at compilation time)
      @param file The file where the warning was sent (defined at compilation time)
      @param object The name of the object sending the warning (defined dynamically)
      @param msg The message explaining the error
     */
   virtual void warning (int line, const char* file, const char* object, QString msg) = 0;

   //! \brief Handle the debug message
   /*! Deubuging data, can be ignored if no developper is around.
      @param line The line where the message was sent (defined at compilation time)
      @param file The file where the message was sent (defined at compilation time)
      @param object The name of the object sending the message (defined dynamically)
      @param msg The message explaining the error
     */
   virtual void debug (int line, const char* file, const char* object, QString msg) = 0;
};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

#endif // COREMESSAGEHANDLER_H
