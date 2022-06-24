#ifndef SECURITYMANAGER_H
#define SECURITYMANAGER_H

#include "apputils/src/user.h"
#include "datacrypter.h"

#include <QCoreApplication>

class QRegExpValidator;
namespace Tucuxi {
namespace Gui {
namespace Core {


/** \ingroup core
 *  \defgroup security Security
 *
 * Tucuxi provides a security manager that allows the encryption and the decryption
 * of sensitive data such as the patients information and medical data. It uses a master
 * key which is the same for all the users. In order to retrieve the master key, a user
 * must provide its username and password.
 *
 * The security manager also provides mechanisms to ensure that a drug XML file provided
 * by Tucuxi is valid. For this purpose, all drug XML files are signed before being
 * integrated into the software.
 *
 */

/** \ingroup security
 *  \brief Provides an interface to the security entity for the rest of the software.
 *
 * The security manager offers services such as authentication and users management.
 * It is possible to authenticate a user, check if a user exists, recover the users
 * list, and many more. Once authenticated, the security manager will store the master
 * key and can therefore be used to encrypt and decrypt any kind of data.
 *
 * For encapsulation purposes, the users' passwords and the master key cannot be retrieved
 * at all. Some of the functions listed below will require an authenticated valid user in
 * order to work: in this documentation, we will call such user the current user. Some
 * of those functions will also require the current user to be an administrator.
 *
 * The security manager implements the singleton pattern, or more precisely the phoenix
 * singleton of Andrei Alexandrescu, to ensure that there is only one user logged in at
 * any time, and to ensure the lifespan of the master key once logged in.
 *
 * \sa User, LoginDialog
 */
class SecurityManager
{
    Q_DECLARE_TR_FUNCTIONS(SecurityManager)

public:

    //! The information list.
    enum class Information {
        Encryption      //! An informative text about the encryption feature, its advantages and disadvantages.
    };

    /** \brief Returns the security manager's unique instance.
     * @return A reference to the unique instance.
     */
    static SecurityManager &instance();

    /** \brief Returns a translated information.
     * @param info The information to be returned.
     * @return The translated information.
     */
    static QString information(Information info);

    //! Validate a drug XML file.
    /** Validate a drug XML file given its signature.
     * @param key The key to validate.
     * @return True if the file is valid, otherwise false.
     */
    static bool validateDrug(const QString &key);

    //! Validate a username.
    /** Validate a username. The usernames must respect a certain number of criteria
     * such as a minimum lenght or being unique. This function will check if the given
     * username mets those criteria, and will fill in an error string if it doesn't.
     * @param username The username to validate.
     * @param error An error string.
     * @return True if the username is valid, otherwise false.
     */
    static bool validateUsername(const QString &username, QString &error);

    //! Validate a password.
    /** Validate a password. The passwords must respect a certain number of criteria
     * such as a minimum length or the use of special characters. This function will
     * check if the given password mets those criteria, and will fill in an error string
     * if it doesn't.
     * @param username The username associated to the password.
     * @param password The password to validate.
     * @param error An error string.
     * @return True if the password is valid, otherwise false.
     */
    static bool validatePassword(const QString &username, const QString &password, QString &error);

    /** \brief Check if a user's username is unique.
     * @param user The user to check.
     * @return True if unique amongst the existing usernames, otherwise false.
     */
    static bool isUnique(const User &user);

    /** \brief Check if a username is unique.
     * @param username The username to check.
     * @return True if unique amongst the existing usernames, otherwise false.
     */
    static bool isUnique(const QString &username);

    /** \brief Returns a regular expression validator for usernames.
     * @return The regular expression validator.
     */
    static QRegExpValidator *usernameValidator();

    /** \brief Authenticate a user.
     * @param username The user name.
     * @param password The user password.
     * @return True if authenticated, otherwise false.
     */
    bool authenticate(const QString &username, const QString &password);

    //! Try to authenticate a user.
    /** Try to authenticate a user, but without login in. The current user,
     * if any, will not change.
     * @param username The user name.
     * @param password The user password.
     * @return True if authenticated, otherwise false.
     */
    bool tryAuthenticate(const QString &username, const QString &password);

    /** \brief Check if the current user is valid and authenticated.
     * @return True if a user is currently authenticated, otherwise false.
     */
    bool isAuthenticated() const;

    /** \brief Check if the current user is an administrator.
     * @return True if the current user is an administrator, otherwise false.
     */
    bool isAdministrator() const;

    /** \brief Check if a user is an administrator.
     * @param user The user to check.
     * @return True if the user is an administrator, otherwise false.
     */
    bool isAdministrator(const User &user) const;

    //! Change the current user password.
    /** Change the current user password given its current password. The current
     * user must be valid and authenticated. The new password must contain at least
     * six characters.
     * @param newPassword The new password.
     * @param oldPassword The current password.
     * @return True if successfully changed, otherwise false.
     */
    bool changePassword(const QString &newPassword, const QString &oldPassword);

    //! Change an existing user's password.
    /** Change an existing user's password. The current user must be valid, authenticated
     * and must possess the administrator rights. The new password must contain at least
     * six characters.
     * @param newPassword The new password.
     * @param user The user.
     * @return True if successfully changed, otherwise false.
     */
    bool changePassword(const QString &newPassword, const User &user);

    /** \brief Return the current user, if any.
     * @return The current user if authenticated, or an invalid user otherwise.
     */
    User user() const;

    /** \brief Returns the list of all the users.
     * @return The list of all the users.
     */
    QList<User> users() const;

    /** \brief Returns true if the database has one or more users registered.
     * @return True if the database has one or more users.
     */
    bool hasUsers() const;

    //! Add a new user.
    /** Add a new user with the given password. The username must be unique, and the
     * password must contain at least six characters. The current user must be valid,
     * authenticated and must possess the administrator rights.
     *
     * If there user list is empty, meaning there is no existing user, the user will
     * be created without further checks. It will be given the administrator rights
     * regardless of what is indicated in the given user object.
     *
     * @param user The user to create.
     * @param password The user password.
     * @return True if successfully added, otherwise false.
     */
    bool addUser(const User &user, const QString &password);

    //! Edit an existing user.
    /** Edit an existing user. The username cannot be changed. The current user must
     * be valid, authenticated and must possess the administrator rights.
     *
     * @param user The user to edit.
     * @param modified The edited user.
     * @return True if successfully added, otherwise false.
     */
    bool editUser(const User &user, const User &modified);

    //! Remove an existing user.
    /** Remove an existing user from the users list. The current user must be valid,
     * authenticated and must possess the administrator rights.
     * @param user The user to remove.
     * @return True if successfully removed, otherwise false.
     */
    bool removeUser(const User &user);

    //! Encrypt the given piece of information.
    /** Encrypt the given piece of information. The current user must be valid and
     * authenticated.
     * @param data The data to encrypt.
     * @return The encrypted data, or an invalid QVariant if it failed.
     */
    QVariant encrypt(const QVariant &data);

    //! Decrypt the given piece of information.
    /** Decrypt the given piece of information. The current user must be valid and
     * authenticated.
     * @param data The data to decrypt.
     * @return The decrypted data, or an invalid QVariant if it failed.
     */
    QVariant decrypt(const QVariant &data);

    /** \brief Returns true if the database is encrypted.
     * @return True if the database is encrypted.
     */
    bool isEncrypted() const;

    //! Set the encryption status of the database.
    /** Set the encryption status of the database. This function will NOT encrypt or decrypt the
     * current database. It will just set the database internal encryption flag to the requested
     * value.
     * @param status The database encryption status.
     * @return True if successfully set, otherwise false.
     */
    bool setEncrypted(bool status);

private:
    //Clean up at shutdown
    static void cleanUp();

    //Check a user existence
    static bool userExists(const QString &username);

    //The unique instance
    static SecurityManager *_instance;

    //The information texts
    static const char *const _INFORMATION_ENCRYPTION;

    //The username error strings
    static const char *const _USERNAME_TOO_SHORT;
    static const char *const _USERNAME_DUPLICATED;

    //The password error strings
    static const char *const _PASSWORD_TOO_SHORT;
    static const char *const _PASSWORD_UPPERCASE;
    static const char *const _PASSWORD_LOWERCASE;
    static const char *const _PASSWORD_NUMERIC;
    static const char *const _PASSWORD_USERNAME;

    //The username constraints
    static const int _USERNAME_MIN_SIZE;

    //The password constraints
    static const int _PASSWORD_MIN_SIZE;
    static const int _PASSWORD_MIN_UPPER;
    static const int _PASSWORD_MIN_LOWER;
    static const int _PASSWORD_MIN_NUM;

    //The username regular expression
    static const char *const _USERNAME_REG_EXP;

    //Private constructor
    SecurityManager();

    //Private copy constructor
    SecurityManager(const SecurityManager &other);

    //Private destructor
    ~SecurityManager();

    //Private assignment operator
    SecurityManager &operator =(const SecurityManager &other);

    //Create a new user
    bool createUser(const User &user, const QString &password);

    //Create a new private key
    bool createUserKey(const User &user, const QString &password);

    //The master key
    QString _masterKey;

    //The current user
    User _user;

    //The en/decryption utility class
    Tucuxi::Gui::AppUtils::DataCrypter _crypter;
};

}//namespace Core
}//namespace Gui
}//namespace Tucuxi
Q_DECLARE_METATYPE(std::string)

#endif // SECURITYMANAGER_H
