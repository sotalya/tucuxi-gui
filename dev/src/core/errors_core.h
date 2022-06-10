//@@license@@

#ifndef COREERRORS
#define COREERRORS

#include <QDebug>

#include "core/utils/logging.h"

namespace Tucuxi {
namespace GuiCore {

    EZERROR(CORE, NOEZERROR, "There is no error.")
    EZERROR(CORE, ITWOULDBENICE,"")
    EZERROR(CORE, FILENOTFOUND,"A file has not been found.")
    EZERROR(CORE, ACCESSDENIED,"The access to a ressource has been denied, check the rights on the host system.")
    EZERROR(CORE, INVALIDPLUGIN,"The plugin is invalid, either a bad version, for the wrong system or compiled wrong.")
    EZERROR(CORE, INVALIDEXTENSION,"The file extension used is unknown or invalid.")
    EZERROR(CORE, INVALIDCHARACTER,"An invalid character has been found in the entered data.")
    EZERROR(CORE, XSDERROR,"A fatal error has occurred when processing an XML Schema file.")
    EZERROR(CORE, XMLERROR,"A fatal error has occurred when parsing the XML file.")
    EZERROR(CORE, INVALIDFILEFORMAT,"The given file format is invalid")
    EZERROR(CORE, DATAERROR,"The data furnished is invalid or do not follows the specification.")
    EZERROR(CORE, DATAMISSING,"Data is missing.")
    EZERROR(CORE, XMLFORMATERROR,"The XML file is incorrectly formatted.")
    EZERROR(CORE, INTERRUPTED,"Tucuxi was stopped by the OS.")
    EZERROR(CORE, UNINITIALIZED,"An non-initialised data has been accessed")
    EZERROR(CORE, INVALIDARG,"An invalid argument has been given")
    EZERROR(CORE, VERSIONERROR,"A required file has the wrong version")
    EZERROR(CORE, CASTERROR,"An unhandled cast error")
    EZERROR(CORE, INVALIDCYCLE,"CycleData object invalid")
    EZERROR(CORE, INVALIDPROPINDEX,"Invalid Property Index")
    EZERROR(CORE, OBJECTISINVALID,"Invalid Object")
        //Security
    EZERROR(CORE, NORIGHTS,"The security policy in use doesn't allow the wanted action. Be careful, lawyers have really little sens of humor.")
    EZERROR(CORE, INCORRECTPASS ,"The given password is incorrect.")
    EZERROR(CORE, INCORRECTUSER ,"The given user is incorrect.")
    EZERROR(CORE, ENCRYPTIONFAILED,"The encryption failed")
    EZERROR(CORE, AUTHENTICATIONFAILED,"The authentication failed")
        //MM
    EZERROR(CORE, NOREVERSE,"This model doesn't handle the reverse search of posology from final concentration.")
    EZERROR(CORE, NOMODEL,"The requested model cannot be found.")
    EZERROR(CORE, NODRUG,"The requested drug cannot be found.")
    EZERROR(CORE, NOOPTION,"The requested option cannot be found.")
    EZERROR(CORE, NOENGINE,"The requested prediction engine cannot be found.")
    EZERROR(CORE, INVALIDOPTION,"An invalid option as been given")
    EZERROR(CORE, BADPLUGIN,"The plugin to load is invalid")
    EZERROR(CORE, NOPERCENTILE,"The requested percentile engine cannot be found.")
    EZERROR(CORE, NOPARAMETER,"The requested parameter is not supplied by the drug XML")
        //DB
    EZERROR(CORE, NOITEM,"The requested item wasn't found, check the ID or name given.")
    EZERROR(CORE, LINKEDITEM,"The item to remove is used by other items, they must be deleted or modified before this item can be removed. \"We make no orphan, kill all the related.")
    EZERROR(CORE, NODATABASE,"No database plugin are present but a call has been made")
    EZERROR(CORE, INVALIDDATABASE,"The database has not been found or is invalid")

    EZERROR(CORE, MISSINGID,"Could not retrieve a newly inserted item's ID")
    EZERROR(CORE, OUTDATED,"The database is outdated")
    //Config
    EZERROR(CORE, UNKNOWNSETTING,"The requested configuration setting is unknown")
    // Help
    EZERROR(CORE, NOHELPTOPIC,"A specific topic is not described correctly")
    EZERROR(CORE, HELPTOPICFILE, "help topics file contains uncorrect data")
    //JOKE Joke stuff
    EZERROR(CORE, NOPOTATO,"A potato is missing. Look if its not in the face of someone.")
    EZERROR(CORE, OLDCHEESE,"Something smells bad. Really bad.")
    EZERROR(CORE, CHINESEDOTHEWORK,"The program worked, but I don't know how.")
    EZERROR(CORE, ICANHAZCHEESEBURGER,"Your spelling is really bad, please open a dictionary.")
    EZERROR(CORE, XENATTAC,"Really, only the Dr. Freeman can resolve this")
} // namespace core
} // namespace Tucuxi
#endif // ERRORS

