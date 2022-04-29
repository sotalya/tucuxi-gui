//@@license@@

#include "action.h"

namespace ezechiel {
namespace cliutils {
//The commands parameters
const char *const Action::_CMD_PARAM_USERNAME = "username=";
const char *const Action::_CMD_PARAM_PASSWORD = "password=";

//The dates and times formats
const char *const Action::_dateTimeFormat    = "dd/MM/yyyy-HH:mm";
const char *const Action::_xmlDateTimeFormat = "yyyy'-'MM'-'dd'T'HH:mm:ss";

QString Action::usernameParam()
{
    return _CMD_PARAM_USERNAME;
}

QString Action::passwordParam()
{
    return _CMD_PARAM_PASSWORD;
}

QString Action::dateTimeFormat()
{
    return QString(_dateTimeFormat);
}

QString Action::xmlDateTimeFormat()
{
    return QString(_xmlDateTimeFormat);
}

QString Action::name()
{
    return "";
}

QString Action::help()
{
    return "";
}

Action::~Action()
{

}

QString Action::error() const
{
    return _errors;
}

} //namespace cliutils
} //namespace ezechiel
