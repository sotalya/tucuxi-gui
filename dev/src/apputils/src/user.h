#ifndef USER_H
#define USER_H

#include <QCoreApplication>

#include "apputils/src/apputilsrepository.h"

/** \ingroup security
 *  \brief An Tucuxi user.
 *
 * This simple class describes an Tucuxi user. It does contain the username and other
 * general information about the user such as its authorization level. It does not store
 * the user's private key, used for authentication, but provides both functions to retrieve
 * and set the private key.
 *
 * A user can be in a valid or invalid state. The user is considered invalid if its username
 * is emtpy or if it's private key is emtpy (or cannot be retrieved for whatever reason).
 *
 * \sa SecurityManager, LoginDialog
 */
class User
{
    Q_DECLARE_TR_FUNCTIONS(User)

public:

    //! The authorization levels
    enum Level {
        UserLevel,
        AdminLevel,
        LEVEL_COUNT
    };

    /** \brief Returns the name of an authorization level.
     * @param level The level.
     * @return The level's name.
     */
    static QString levelName(Level level);

    /** \brief Returns the description of an authorization level.
     * @param level The level.
     * @return The level's description.
     */
    static QString levelDesc(Level level);

    //! Constructor.
    /** Construct an invalid user.
     */
    User();

    //! Constructor.
    /** Constructs a user.
     * @param username The username.
     * @param level The authorization level.
     */
    User(const QString &username, Level level = UserLevel);

    /** \brief Check if the user is valid.
     * @return True if the user is valid, otherwise false.
     */
    bool isValid() const;

    /** \brief Returns the username.
     * @return The username.
     */
    QString username() const;

    /** \brief Sets the username.
     * @param username The username.
     */
    void setUsername(const QString &username);

    //! Returns the private key.
    /** Returns the private key stored in the database. This function will make a call to the
     * database in order to retrieve the key.
     * @return The private key, or an empty string if an error occures.
     */
    QString privateKey() const;

    //! Sets the private key.
    /** Sets the private key in the database. This function will make a call to the database in
     * order to save the private key of the user.
     *
     * Warning: the user must be previously set in the databasefor the operation to be successful.
     *
     * @param key The private key.
     * @return True if successfully set, otherwise false.
     */
    bool setPrivateKey(const QString &key);

    /** \brief Returns the authorization level.
     * @return The authorization level.
     */
    Level authorization() const;

    /** \brief Sets the authorization level.
     * @param state The authorization level.
     */
    void setAuthorization(Level level);

private:
    //The authorizathion names
    static const char *const _LEVEL_ADMIN_NAME;
    static const char *const _LEVEL_USER_NAME;
    static const char *const _LEVEL_UNKOWN_NAME;

    //The authorizathion descriptions
    static const char *const _LEVEL_ADMIN_DESC;
    static const char *const _LEVEL_USER_DESC;
    static const char *const _LEVEL_UNKOWN_DESC;

    //The username
    QString _username;

    //The authorization level
    Level _authorization;
};

#endif // USER_H
