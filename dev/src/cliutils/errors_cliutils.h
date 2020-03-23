#ifndef CLIUTILSERRORS
#define CLIUTILSERRORS

#include <QDebug>
#include "core/utils/logging.h"

namespace ezechiel {
namespace cliutils {

EZERROR(CLIUTILS, NOEZERROR,"There is no error.")
EZERROR(CLIUTILS, MISSINGPASS ,"The user's password is needed but not given, see the -P global option.")
EZERROR(CLIUTILS, MISSINGUSER ,"The user is needed but not given, see the -U global option. If it's a child, look around the house.")
EZERROR(CLIUTILS, SYNTAXERROR ,"The syntax of the command line is incorrect.")
EZERROR(CLIUTILS, MISSINGVALUE ,"A value for a parameter or option requiring one has not been given.")
EZERROR(CLIUTILS, MISSINGPARAMETER ,"A needed parameter for this action has not been given.")
EZERROR(CLIUTILS, UNKNOWNACTION ,"The requested action is unknown, check the documentation for a list of available one. (Also check if the keyboard is not in DVORAK, some hairy system admin might have passed here.")
EZERROR(CLIUTILS, MISSINGMASTER ,"The master password is needed but not given, see the -M global option.")
EZERROR(CLIUTILS, UNKNOWNOPTION ,"The given option is unknown.")
EZERROR(CLIUTILS, ACTIONERROR,"An error appened in the action")
EZERROR(CLIUTILS, FILENOTFOUND,"Couldnt find the file specified.")
EZERROR(CLIUTILS, DATAERROR,"Couldnt find the file specified.")
EZERROR(CLIUTILS, OBJECTNOTFOUND,"Couldnt find the object assumed to exist.")
EZERROR(CLIUTILS, CANTSAVE,"Couldnt save an object to DB.")

} //namespace ezechiel
} //namespace cliutils
#endif // CLIUTILSERRORS

