//@@license@@

#include "useraction.h"
#include "core/core.h"
#include "apputils/src/appcore.h"
#include "apputils/src/securitymanager.h"
#include "apputils/src/apputilsrepository.h"
#include "core/utils/logging.h"
#include "errors_cliutils.h"
//#include "dbinterface.h"

namespace ezechiel {
namespace cliutils {
//The error messages
const char *const UserAction::_INVALID_SUBCOMMAND   = QT_TR_NOOP_UTF8("please run the help for more information");
const char *const UserAction::_INVALID_ARGUMENT     = QT_TR_NOOP_UTF8("The argument '%1' is unknown.");
const char *const UserAction::_INVALID_ARGUMENTS    = QT_TR_NOOP_UTF8("Invalid number of arguments given: the '%1' subcommand requires exactly %2 arguments.");
const char *const UserAction::_TOO_FEW_ARGUMENTS    = QT_TR_NOOP_UTF8("Invalid number of arguments given: the '%1' subcommand requires at least %2 arguments.");
const char *const UserAction::_TOO_MANY_ARGUMENTS   = QT_TR_NOOP_UTF8("Invalid number of arguments given: the '%1' subcommand requires at most %2 arguments.");
const char *const UserAction::_USER_CREATION_FAILED = QT_TR_NOOP_UTF8("The creation of the user failed due to an unknown error. Please try again.");
const char *const UserAction::_ADMINISTRATOR_RIGHTS = QT_TR_NOOP_UTF8("You need administrator rights to %1. Please login with an appropriate user account and try again.");

//The shared messages
const char *const UserAction::_DATABASE_STATUS = QT_TR_NOOP_UTF8("The database is currently %1.");

//The action commands
const char *const UserAction::_ACTION_COMMAND   = "user";
const char *const UserAction::_CMD_CREATE_FIRST = "first";
const char *const UserAction::_CMD_CREATE       = "create";
const char *const UserAction::_CMD_PASSWORD     = "password";
const char *const UserAction::_CMD_ENCRYPTED    = "encryption";
const char *const UserAction::_CMD_ENCRYPT      = "encrypt";
const char *const UserAction::_CMD_DECRYPT      = "decrypt";

//The commands parameters
const char *const UserAction::_CMD_PARAM_USER  = "user";
const char *const UserAction::_CMD_PARAM_ADMIN = "admin";

QString UserAction::name()
{
    return _ACTION_COMMAND;
}

QString UserAction::help()
{
    QString str;
    str = tr("   NAME") + "\n" +
          tr("       EzeCHieL User Command") + "\n" + "\n" +
          tr("   SYNOPSIS") + "\n" +
             "       " + QString("ezechiel-cli [OPTIONS] %1 SUBCOMMAND").arg(_ACTION_COMMAND) + "\n" + "\n" +
          tr("   DESCRIPTION") + "\n" +
          tr("       The EzeCHieL User Command can be used to manage the EzeCHieL users as well as the encryption status of the database. A valid username and password must be passed for every subcommand with the notable exception of the creation of the database's first user.") + "\n" + "\n" +
          tr("   SUBCOMMANDS") + "\n" +
             "       " + QString("%1 USERNAME PASSWORD").arg(_CMD_CREATE_FIRST) + "\n" +
          tr("              Create a first user. For the operation to succeed, the database must be empty and it must not contain any user. By default, the initial user will be an administrator.")  + "\n" + "\n" +
             "       " + QString("%1USERNAME %2PASSWORD %3 USERNAME PASSWORD [%4|%5]").arg(_CMD_PARAM_USERNAME, _CMD_PARAM_PASSWORD, _CMD_CREATE, _CMD_PARAM_USER, _CMD_PARAM_ADMIN) + "\n" +
          tr("              Create a new user. The first username and password are the login information of an administrator. The second username and password are the login information of the new user. If the last argument is omitted, the newly created user will be a simple user.")  + "\n" + "\n" +
             "       " + QString("%1USERNAME %2PASSWORD %3 NEWPASSWORD").arg(_CMD_PARAM_USERNAME, _CMD_PARAM_PASSWORD, _CMD_PASSWORD) + "\n" +
          tr("              Change the password of the user USERNAME.")  + "\n" + "\n" +
             "       " + QString("%1USERNAME %2PASSWORD %3").arg(_CMD_PARAM_USERNAME, _CMD_PARAM_PASSWORD, _CMD_ENCRYPTED) + "\n" +
          tr("              Check if the database is currently encrypted.")  + "\n" + "\n" +
             "       " + QString("%1USERNAME %2PASSWORD %3").arg(_CMD_PARAM_USERNAME, _CMD_PARAM_PASSWORD, _CMD_ENCRYPT) + "\n" +
          tr("              Encrypt the whole database. For the operation to succeed, the database must not be already encrypted.")  + "\n" + "\n" +
             "       " + QString("%1USERNAME %2PASSWORD %3").arg(_CMD_PARAM_USERNAME, _CMD_PARAM_PASSWORD, _CMD_DECRYPT) + "\n" +
          tr("              Decrypt the whole database. For the operation to succeed, the database must be encrypted.")  + "\n" + "\n" +
          "\n";

    return str;
}

UserAction::UserAction()
{

}

bool UserAction::run(const QStringList &args)
{
    QString subcommand;

    //Copy the arguments
    QStringList wargs = args;

    //Clear the errors
    _errors.clear();

    //Check the arguments
    if (!wargs.size()) {
        _errors = tr("Missing subcommand, %1.").arg(tr(_INVALID_SUBCOMMAND));
        return false;
    }

    //Run login-free subcommands
    if (wargs.first() == _CMD_CREATE_FIRST) {
        wargs.removeFirst();
        return createFirst(wargs);
    }

    //Authenticate for the others
    if (wargs.size() < 2 || !wargs.at(0).startsWith(_CMD_PARAM_USERNAME) || !wargs.at(1).startsWith(_CMD_PARAM_PASSWORD)) {
        _errors = tr("Missing username and/or password, %1.").arg(tr(_INVALID_SUBCOMMAND));
        return false;
    }

    wargs[0].remove(_CMD_PARAM_USERNAME);
    QString username = wargs.takeFirst();
    wargs[0].remove(_CMD_PARAM_PASSWORD);
    QString password = wargs.takeFirst();

    if (!APPCORE->securityManager()->authenticate(username, password)) {
        _errors = tr("The authentication failed, please check your username and password and try again");
        return false;
    }

    //Get the subcommand
    subcommand = wargs.takeFirst();

    //Run the subcommand
    if (subcommand == _CMD_CREATE)
        return createUser(wargs);
    if (subcommand == _CMD_PASSWORD)
        return changePassword(wargs, password);
    if (subcommand == _CMD_ENCRYPTED)
        return checkEncryption(wargs);
    if (subcommand == _CMD_ENCRYPT)
        return encryptDatabase(wargs, true);
    if (subcommand == _CMD_DECRYPT)
        return encryptDatabase(wargs, false);

    //Display an error
    _errors = tr("Invalid subcommand '%1' given, %2.").arg(subcommand, tr(_INVALID_SUBCOMMAND));

    return false;
}

bool UserAction::createFirst(QStringList &args)
{
    //Check if enough arguments
    if (args.size() != 2) {
        _errors = tr(_INVALID_ARGUMENTS).arg(_CMD_CREATE_FIRST, QString::number(2));
        return false;
    }

    //Check if there is users
//    if (APPCORE->securityManager()->hasUsers()) {
//        _errors = tr("One or more users already exists for this database. Use the '%1' subcommand to add a new user.").arg(_CMD_CREATE);
//        return false;
//    }

    //Get the username and password
    QString username = args.takeFirst();
    QString password = args.takeFirst();

    //Check the username
    if (!APPCORE->securityManager()->validateUsername(username, _errors))
        return false;

    //Check the password
    if (!APPCORE->securityManager()->validatePassword(username, password, _errors))
        return false;

    //Create the user
    User user(username);

    if (!APPCORE->securityManager()->addUser(user, password)) {
        _errors = tr(_USER_CREATION_FAILED);
        return false;
    }

    return true;
}

bool UserAction::createUser(QStringList &args)
{
    //Check if too few arguments
    if (args.size() < 2) {
        _errors = tr(_TOO_FEW_ARGUMENTS).arg(_CMD_CREATE, QString::number(2));
        return false;
    }

    //Check if too many arguments
    if (args.size() > 3) {
        _errors = tr(_TOO_MANY_ARGUMENTS).arg(_CMD_CREATE, QString::number(3));
        return false;
    }

    //Check if administrator
    if (!APPCORE->securityManager()->isAdministrator()) {
        _errors = tr(_ADMINISTRATOR_RIGHTS).arg(tr("create a new user"));
        return false;
    }

    //Get the username and password
    QString username = args.takeFirst();
    QString password = args.takeFirst();

    //Check the username
    if (!APPCORE->securityManager()->validateUsername(username, _errors))
        return false;

    //Check the password
    if (!APPCORE->securityManager()->validatePassword(username, password, _errors))
        return false;

    //Get the user level
    User::Level level = User::UserLevel;

    if (!args.isEmpty()) {
        QString levelStr = args.takeFirst();

        //Check if valid
        if (levelStr != _CMD_PARAM_USER && levelStr != _CMD_PARAM_ADMIN) {
            _errors = tr(_INVALID_ARGUMENT).arg(levelStr);
            return false;
        }

        //Set the level
        if (levelStr == _CMD_PARAM_ADMIN)
            level = User::AdminLevel;
    }

    //Create the user
    User user(username, level);

    if (!APPCORE->securityManager()->addUser(user, password)) {
        _errors = tr(_USER_CREATION_FAILED);
        return false;
    }

    return true;
}

bool UserAction::changePassword(QStringList &args, const QString &password)
{
    //Check if enough arguments
    if (args.size() != 1) {
        _errors = tr(_INVALID_ARGUMENTS).arg(_CMD_PASSWORD, QString::number(1));
        return false;
    }

    //Get the new password
    QString newPassword = args.takeFirst();

    //Check the new password
    if (!APPCORE->securityManager()->validatePassword(APPCORE->securityManager()->user().username(), newPassword, _errors))
        return false;

    //Set the new password
    if (!APPCORE->securityManager()->changePassword(newPassword, password)) {
        _errors = tr("Failed to change the password. Please check your login information and try again.");
        return false;
    }

    return true;
}

bool UserAction::checkEncryption(QStringList &args)
{
    //Check if too many arguments
    if (!args.isEmpty()) {
        _errors = tr(_INVALID_ARGUMENTS).arg(_CMD_ENCRYPTED, QString::number(0));
        return false;
    }

    //Check if administrator
    if (!APPCORE->securityManager()->isAdministrator()) {
        _errors = tr(_ADMINISTRATOR_RIGHTS).arg(tr("check the encryption status"));
        return false;
    }

    //Print the encryption status
    if (APPCORE->securityManager()->isEncrypted())
    {
        LOG(QtDebugMsg, NOEZERROR, tr(_DATABASE_STATUS).arg(tr("encrypted")));
    } else
    {
        LOG(QtDebugMsg, NOEZERROR, tr(_DATABASE_STATUS).arg(tr("decrypted")));
    }

    return true;
}

bool UserAction::encryptDatabase(QStringList &args, bool encrypt)
{
    //Check if too many arguments
    if (!args.isEmpty()) {
        _errors = tr(_INVALID_ARGUMENTS).arg(_CMD_ENCRYPTED, QString::number(0));
        return false;
    }

    //Check if administrator
    if (!APPCORE->securityManager()->isAdministrator()) {
        _errors = tr(_ADMINISTRATOR_RIGHTS).arg(tr("change the database's encryption status"));
        return false;
    }

    //Check if already encrypted
    if (APPCORE->securityManager()->isEncrypted() == encrypt) {
        _errors = tr("The database is already %1.").arg(encrypt ? tr("encrypted") : tr("decrypted"));
        return false;
    }

    //Encrypt the whole database
    ezechiel::GuiCore::Response response;

    if ((response = APPUTILSREPO->encryptDatabase(encrypt)).error != ezechiel::GuiCore::NoError) {
        _errors = tr("An error occured while %1 the database.\n\n%2").arg(encrypt ? tr("encrypting") : tr("decrypting"), response.message);
        return false;
    }

    return true;
}

} //namespace cliutils
} //namespace ezechiel
