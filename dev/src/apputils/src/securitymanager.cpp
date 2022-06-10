#include "apputils/src/securitymanager.h"
#include "core/core.h"
//#include "dbinterface.h"
//#include "core_errors.h"
//#include "coremessagehandler.h"
#include "core/interfaces/abstractrepository.h"
#include "apputils/src/apputilsrepository.h"
#include "core/utils/logging.h"
#include "core/errors_core.h"

#include <QVariant>
#include <QRegularExpression>
#include <QRegExpValidator>

namespace Tucuxi {
namespace Gui {
namespace Core {


//The unique instance
SecurityManager *SecurityManager::_instance = 0;

//The information texts
const char *const SecurityManager::_INFORMATION_ENCRYPTION = QT_TR_NOOP_UTF8(
            "<html><head/><body><p align=\"justify\">In EzeCHieL, you have the possibility to "
            "encrypt sensitive data such as personal information and medical records. Encryting "
            "sensitive data may be useful as it restricts access to users with valid usernames and "
            "passwords. If a malicious or unauthorized user were to obtain an encrypted database, "
            "the data would still be well protected, and the user would be unable to retreive the "
            "data without a valid username and password.</p><p align=\"justify\">If you decide to "
            "encrypt your data, <span style=\" font-weight:600; color:#ff0000;\">be aware that if "
            "you lose your username and/or password, the data in the database will be permanently "
            "lost</span>. If you decide not to encrypt your data, you would be able to recover it, "
            "but anyone else with access to the database would also. In either case, you still need "
            "to provide a valid username and password to use EzeCHieL.</p></body></html>");

//The username error strings
const char *const SecurityManager::_USERNAME_TOO_SHORT  = QT_TR_NOOP_UTF8("The username must contain at least %1 characters");
const char *const SecurityManager::_USERNAME_DUPLICATED = QT_TR_NOOP_UTF8("The username already exists");

//The password error strings
const char *const SecurityManager::_PASSWORD_TOO_SHORT  = QT_TR_NOOP_UTF8("The password must contain at least %1 characters");
const char *const SecurityManager::_PASSWORD_UPPERCASE  = QT_TR_NOOP_UTF8("The password must contain at least %1 uppercase character");
const char *const SecurityManager::_PASSWORD_LOWERCASE  = QT_TR_NOOP_UTF8("The password must contain at least %1 lowercase character");
const char *const SecurityManager::_PASSWORD_NUMERIC    = QT_TR_NOOP_UTF8("The password must contain at least %1 numeric character");
const char *const SecurityManager::_PASSWORD_USERNAME   = QT_TR_NOOP_UTF8("The password must be different than the username");

//The username minimums
const int SecurityManager::_USERNAME_MIN_SIZE  = 3;

//The password minimums
const int SecurityManager::_PASSWORD_MIN_SIZE  = 8;
const int SecurityManager::_PASSWORD_MIN_UPPER = 1;
const int SecurityManager::_PASSWORD_MIN_LOWER = 1;
const int SecurityManager::_PASSWORD_MIN_NUM   = 1;

//The username regular expression
const char *const SecurityManager::_USERNAME_REG_EXP = "[a-zA-Z][a-zA-Z0-9_]*";

//Returns the unique instance
SecurityManager &SecurityManager::instance()
{
    //Create the instance if necessary
    if (_instance == 0)
        _instance = new SecurityManager();

    return *_instance;
}

//Returns a translated information
QString SecurityManager::information(SecurityManager::Information info)
{
    if (info == Information::Encryption)
        return tr(_INFORMATION_ENCRYPTION);

    return QString();
}

//Validate a dug XML file
bool SecurityManager::validateDrug(const QString &key)
{
    Q_UNUSED(key);

    return false; //ToDo
}

//Validates a username
bool SecurityManager::validateUsername(const QString &username, QString &error)
{
    //Check the username lenght
    if (username.size() < _USERNAME_MIN_SIZE) {
        error = tr(_USERNAME_TOO_SHORT).arg(_USERNAME_MIN_SIZE);
        return false;
    }

    //Check the username uniqueness
    if (!isUnique(username)) {
        error = tr(_USERNAME_DUPLICATED);
        return false;
    }

    return true;
}

//Validates a password
bool SecurityManager::validatePassword(const QString &username, const QString &password, QString &error)
{
    //Check the username lenght
    if (password.size() < _PASSWORD_MIN_SIZE) {
        error = tr(_PASSWORD_TOO_SHORT).arg(_PASSWORD_MIN_SIZE);
        return false;
    }

    //Check if contains uppercase
    if (password.count(QRegularExpression("[A-Z]")) < _PASSWORD_MIN_UPPER) {
        error = tr(_PASSWORD_UPPERCASE).arg(_PASSWORD_MIN_UPPER);
        return false;
    }

    //Check if contains lowercase
    if (password.count(QRegularExpression("[a-z]")) < _PASSWORD_MIN_LOWER) {
        error = tr(_PASSWORD_LOWERCASE).arg(_PASSWORD_MIN_LOWER);
        return false;
    }

    //Check if contains numbers
    if (password.count(QRegularExpression("[0-9]")) < _PASSWORD_MIN_NUM) {
        error = tr(_PASSWORD_NUMERIC).arg(_PASSWORD_MIN_NUM);
        return false;
    }

    //Check if different than username
    if (password == username) {
        error = tr(_PASSWORD_USERNAME);
        return false;
    }

    return true;
}

//Check if a user's username is valid
bool SecurityManager::isUnique(const User &user)
{
    //Check if the user is valid
    if (!user.isValid())
        return false;

    return isUnique(user.username());
}

//Check if a username is unique
bool SecurityManager::isUnique(const QString &username)
{
    return !userExists(username);
}

//Returns a username reg exp validator
QRegExpValidator *SecurityManager::usernameValidator()
{
    return new QRegExpValidator(QRegExp(_USERNAME_REG_EXP));
}

//Authenticates a user
bool SecurityManager::authenticate(const QString &username, const QString &password)
{
    return true;
    //FIX!!!
//    QString userKey;

//    //Get the userKey from database
//    if (dynamic_cast<Tucuxi::EphemeralDB*>(ABSTRACTREPO)->getUserKey(username, userKey).error != NoError)
//        return false;

//    //Try to authenticate the user
//    if (_crypter.login(username, password, userKey))
//        if (APPUTILSREPO->getUser(username, _user).error == NoError)
//            return true;

//    //Set an invalid user otherwise
//    _user = User();

//    return false;
}

//Try to authenticate a user
bool SecurityManager::tryAuthenticate(const QString &username, const QString &password)
{
    QString userKey;

    //Get the userKey from database
    if (APPUTILSREPO->getUserKey(username, userKey).error != NoError)
        return false;

    //Try to authenticate the user
    return _crypter.login(username, password, userKey);
}

//Check if the current user is authenticated
bool SecurityManager::isAuthenticated() const
{
    return _user.isValid();
}

//Check if the current user is an administrator
bool SecurityManager::isAdministrator() const
{
    //Check if current is authenticated
    if (!isAuthenticated())
        return false;

    return isAdministrator(_user);
}

//Check if a user is an administrator
bool SecurityManager::isAdministrator(const User &user) const
{
    //Check if the user is valid
    if (!user.isValid())
        return false;

    return user.authorization() == User::AdminLevel;
}

//Changes the current user password
bool SecurityManager::changePassword(const QString &newPassword, const QString &oldPassword)
{
    //Check if current is authenticated
    if (!isAuthenticated())
        return false;

    //Get the userKey from database
    QString userKey = _user.privateKey();

    //Try to authenticate the user
    if (!_crypter.login(_user.username(), oldPassword, userKey))
        return false;

    return createUserKey(_user, newPassword);
}

//Change an existing user password.
bool SecurityManager::changePassword(const QString &newPassword, const User &user)
{
    //Check if the user is valid
    if (!user.isValid())
        return false;

    //Check if current is admin
    if (!isAdministrator())
        return false;

    //Check if the user exists
    if (!userExists(user.username()))
        return false;

    return createUserKey(user, newPassword);
}

//Returns the current user
User SecurityManager::user() const
{
    return _user;
}

//Returns the users list
QList<User> SecurityManager::users() const
{
    QList<User> users;

    //Get the users list
    //FIX!!!
//    APPUTILSREPO->getUsersList(users);

    return users;
}

//Checks if there is one or more users
bool SecurityManager::hasUsers() const
{
    return !users().isEmpty();
}

//Adds a new user
bool SecurityManager::addUser(const User &user, const QString &password)
{
    //Add a new user if admin
    if (isAdministrator())
        return createUser(user, password);

    //Check if there is no user
    if (!users().isEmpty())
        return false;

    //Add the first user
    User root(user);
    root.setAuthorization(User::AdminLevel);

    //Create the master key
    _crypter.createMasterKey();

    return createUser(root, password);
}

//Edits an existing user
bool SecurityManager::editUser(const User &user, const User &modified)
{
    //Check if the users are valid
    if (!user.isValid() || !modified.isValid())
        return false;

    //Check if current is admin
    if (!isAdministrator())
        return false;

    //Check username is unchanged
    if (user.username() != modified.username())
        return false;

    //Check if user is different
    if (_user.username() == user.username())
        return false;

    //Check if the user exists
    if (!userExists(user.username()))
        return false;

    return APPUTILSREPO->setUser(modified).error == NoError;
}

//Removes an existing user
bool SecurityManager::removeUser(const User &user)
{
    //Check if the user is valid
    if (!user.isValid())
        return false;

    //Check if current is admin
    if (!isAdministrator())
        return false;

    //Check if current is different
    if (_user.username() == user.username())
        return false;

    return APPUTILSREPO->deleteUser(user.username()).error == NoError;
}

//Encrypts a piece of information
QVariant SecurityManager::encrypt(const QVariant &data)
{
    //Check if the data is valid
    if (!data.isValid())
        return QVariant();

    //Check if current is authenticated
    if (!isAuthenticated())
        return QVariant();

    //Encrypt using std::string format
    if (data.canConvert<std::string>())
        return QVariant::fromValue<std::string>(_crypter.encrypt(data.value<std::string>()));

    //Encrypt using QString format
    if (data.canConvert<QString>())
        return QVariant(QString::fromStdString(_crypter.encrypt(data.toString())));

    return QVariant();
}

//Decrypts a piece of information
QVariant SecurityManager::decrypt(const QVariant &data)
{
    //Check if the data is valid
    if (!data.isValid())
        return QVariant();

    //Check if current is authenticated
    if (!isAuthenticated())
        return QVariant();

    //Decrypt using std::string format
    if (data.canConvert<std::string>())
        return QVariant::fromValue<std::string>(_crypter.decrypt(data.value<std::string>()));

    //Decrypt using QString format
    if (data.canConvert<QString>())
        return QVariant(QString::fromStdString(_crypter.decrypt(data.toString())));

    return QVariant();
}

//Checks if the database is encrypted
bool SecurityManager::isEncrypted() const
{
    bool status;

    //Get the encryption status
    Response response = APPUTILSREPO->isEncrypted(status);

    if (response.error != NoError)
        LOG(QtCriticalMsg, Tucuxi::Gui::Core::DATAERROR, tr("Could not retrieve the database encryption status. The database returned the following error:\n%1").arg(response.message));

    //Return the encryption status
    return status;
}

//Sets the database encryption status
bool SecurityManager::setEncrypted(bool status)
{
    return APPUTILSREPO->setEncrypted(status).error == NoError;
}

//Clean up at shutdown
void SecurityManager::cleanUp()
{
    delete _instance;
    _instance = 0;
}

//Check a user existence
bool SecurityManager::userExists(const QString &username)
{
    //Unused user provided to the database
    User unused;

    //FIX!!!
    return false;
//    return dynamic_cast<Tucuxi::EphemeralDB*>(ABSTRACTREPO)->getUser(username, unused).error == NoError;
}

//Protected constructor
SecurityManager::SecurityManager()
{
    //Function to call at shutdown
    atexit(&cleanUp);
}

//Private copy constructor
SecurityManager::SecurityManager(const SecurityManager &other)
{
    Q_UNUSED(other);
}

//Private destructor
SecurityManager::~SecurityManager()
{

}

//Private assignment operator
SecurityManager &SecurityManager::operator =(const SecurityManager &other)
{
    Q_UNUSED(other);

    return *this;
}

//Create a new user
bool SecurityManager::createUser(const User &user, const QString &password)
{
    QString unused;

    //Check if the username is valid
    if (!validateUsername(user.username(), unused))
        return false;

    //Add the user to the database
//    if (dynamic_cast<Tucuxi::EphemeralDB*>(ABSTRACTREPO)->setUser(user).error != NoError)
//        return false;

    //Create the user private key
    if (createUserKey(user, password))
        return true;

    //Delete the user in case of error
//    dynamic_cast<Tucuxi::EphemeralDB*>(ABSTRACTREPO)->deleteUser(user.username());

    return false;
}

//Create a new private key
bool SecurityManager::createUserKey(const User &user, const QString &password)
{
    //Get the user new private key
    QString newKey = _crypter.createUser(user.username(), password);

    if (newKey.isEmpty())
        return false;

    //Set the user new private key
    //FIX!!!
    return false;
//    return dynamic_cast<Tucuxi::EphemeralDB*>(ABSTRACTREPO)->setUserKey(user.username(), newKey).error == NoError;
}

}//namespace core
}//namespace Gui
}//namespace Tucuxi
