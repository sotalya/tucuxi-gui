//Defines the CLI errors

//! The user's password is needed but not given, see the -P global option.
DECLARE_CLI_ERROR( MISSINGPASS ,100)
//! The user is needed but not given, see the -U global option. If it's a child, look around the house.
DECLARE_CLI_ERROR( MISSINGUSER ,101)
//! The syntax of the command line is incorrect.
DECLARE_CLI_ERROR( SYNTAXERROR ,102)
//! A value for a parameter or option requiring one has not been given.
DECLARE_CLI_ERROR( MISSINGVALUE ,103)
//! A needed parameter for this action has not been given.
DECLARE_CLI_ERROR( MISSINGPARAMETER ,104)
//! The requested action is unknown, check the documentation for a list of available one. (Also check if the keyboard is not in DVORAK, some hairy system admin might have passed here.)
DECLARE_CLI_ERROR( UNKNOWNACTION ,105)
//! The master password is needed but not given, see the -M global option.
DECLARE_CLI_ERROR( MISSINGMASTER ,106)
//! The given option is unknown.
DECLARE_CLI_ERROR( UNKNOWNOPTION ,109)
//! An error appened in the action
DECLARE_CLI_ERROR( ACTIONERROR ,110)
