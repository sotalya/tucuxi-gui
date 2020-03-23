//Defines the Core's errors

//! This code is returned when the program executed without error.
DECLARE_CORE_ERROR( NOERRORS ,0)
//! The requested functionality is not yet implemented.
DECLARE_CORE_ERROR( ITWOULDBENICE ,1)
//! A file has not been found.
DECLARE_CORE_ERROR( FILENOTFOUND ,2)
//! The access to a ressource has been denied, check the rights on the host system.
DECLARE_CORE_ERROR( ACCESSDENIED ,3)
//! The plugin is invalid, either a bad version, for the wrong system or compiled wrong.
DECLARE_CORE_ERROR( INVALIDPLUGIN ,4)
//! The file extension used is unknown or invalid.
DECLARE_CORE_ERROR( INVALIDEXTENSION, 5)
//! An invalid character has been found in the entered data.
DECLARE_CORE_ERROR( INVALIDCHARACTER ,6)
//! A fatal error has occurred when processing an XML Schema file.
DECLARE_CORE_ERROR(XSDERROR, 7)
//! A fatal error has occurred when parsing the XML file.
DECLARE_CORE_ERROR( XMLERROR, 8)
//! The given file format is invalid
DECLARE_CORE_ERROR( INVALIDFILEFORMAT, 9)
//! The data furnished is invalid or do not follows the specification.
DECLARE_CORE_ERROR( DATAERROR, 10)
//! A data is missing.
DECLARE_CORE_ERROR( DATAMISSING, 11)
//! The XML file is incorrectly formatted.
DECLARE_CORE_ERROR( XMLFORMATERROR, 12)
//! Ezechiel was stopped by the OS.
DECLARE_CORE_ERROR( INTERRUPTED, 13)
//! An non-initialised data has been accessed
DECLARE_CORE_ERROR( UNINITIALISED, 14)
//! An invalid argument has been given
DECLARE_CORE_ERROR( INVALIDARG, 15)
//! A required file has the wrong version
DECLARE_CORE_ERROR( VERSIONERROR, 16)
//! An unhandled cast error
DECLARE_CORE_ERROR( CASTERROR, 17)

//Security
//! The security policy in use doesn't allow the wanted action. Be careful, lawyers have really little sens of humor.
DECLARE_CORE_ERROR( NORIGHTS ,20)
//! The given password is incorrect.
DECLARE_CORE_ERROR( INCORRECTPASS ,21)
//! The given user is incorrect.
DECLARE_CORE_ERROR( INCORRECTUSER ,22)
//! The encryption failed
DECLARE_CORE_ERROR( ENCRYPTIONFAILED ,23)
//! The authentication failed
DECLARE_CORE_ERROR( AUTHENTICATIONFAILED, 24)

//MM
//! This model doesn't handle the reverse search of posology from final concentration.
DECLARE_CORE_ERROR( NOREVERSE ,30)
//! The requested model cannot be found.
DECLARE_CORE_ERROR( NOMODEL ,31)
//! The requested drug cannot be found.
DECLARE_CORE_ERROR( NODRUG ,32)
//! The requested option cannot be found.
DECLARE_CORE_ERROR( NOOPTION ,33)
//! The requested prediction engine cannot be found.
DECLARE_CORE_ERROR( NOENGINE ,34)
//! An invalid option as been given
DECLARE_CORE_ERROR( INVALIDOPTION ,35)
//! The plugin to load is invalid
DECLARE_CORE_ERROR( BADPLUGIN ,36)
//! The requested percentile engine cannot be found.
DECLARE_CORE_ERROR( NOPERCENTILE ,37)
//! The requested parameter is not supplied by the drug XML
DECLARE_CORE_ERROR( NOPARAMETER ,38)

//DB
//! The requested item wasn't found, check the ID or name given.
DECLARE_CORE_ERROR( NOITEM ,40)
//! The item to remove is used by other items, they must be deleted or modified before this item can be removed. "We make no orphan, kill all the related."
DECLARE_CORE_ERROR( LINKEDITEM ,41)
//! No database plugin are present but a call has been made
DECLARE_CORE_ERROR( NODATABASE ,42)
//! The database has not been found or is invalid
DECLARE_CORE_ERROR( INVALIDDATABASE ,43)
//! Could not retrieve a newly inserted item's ID
DECLARE_CORE_ERROR( MISSINGID, 44)
//! The database is outdated
DECLARE_CORE_ERROR( OUTDATED, 45)

//Config
//! The requested configuration setting is unknown
DECLARE_CORE_ERROR( UNKNOWNSETTING ,50)

// Help
// A specific topic is not described correctly
DECLARE_CORE_ERROR( NOHELPTOPIC,60)
// The help topics file contains uncorrect data
DECLARE_CORE_ERROR( HELPTOPICFILE, 61)


//JOKE Joke stuff
//! A potato is missing. Look if its not in the face of someone.
DECLARE_CORE_ERROR( NOPOTATO ,300)
//! Something smells bad. Really bad.
DECLARE_CORE_ERROR( OLDCHEESE ,301)
//! The program worked, but I don't know how...
DECLARE_CORE_ERROR( CHINESEDOTHEWORK ,302)
//! Your spelling is really bad, please open a dictionary.
DECLARE_CORE_ERROR( ICANHAZCHEESEBURGER ,303)
//! Really, only the Dr. Freeman can resolve this
DECLARE_CORE_ERROR( XENATTACK, 304)

