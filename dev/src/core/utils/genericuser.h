/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef GENERICUSER_H
#define GENERICUSER_H

#include <QCoreApplication>
//#include "dal/genericdbobjects.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


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
class GenericUser : public QObject
{
//    Q_DECLARE_TR_FUNCTIONS(User)
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY userPKNotify)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString key READ key WRITE setKey NOTIFY keyChanged)
    Q_PROPERTY(int authorization READ authorization WRITE setAuthorization NOTIFY authorizationChanged)    

public:

    //! The authorization levels
    enum Level {
        UserLevel,
        AdminLevel,
        LEVEL_COUNT
    };

    Q_INVOKABLE GenericUser() {
        setId(0);
    }

    void setId(int id){ m_id = id; emit userPKNotify(id, this);}
    int id() const{ return m_id; }

    void setUsername(QString name){m_name = name; emit usernameChanged(name);}
    QString username() const{ return m_name; }

    void setKey(const QString& key) {
        m_key = key; }
    QString key() const{ return m_key; }

    void setAuthorization(int auth){ m_auth = auth; emit authorizationChanged(auth);}
    int authorization() const{ return m_auth; }

    static QString levelName(Level level);

    static QString levelDesc(Level level);

//    User(const QString &username, Level level = UserLevel);

    void setIsValid(bool valid) { m_valid = valid; }
    bool isValid() const{ return m_valid; }

//    QString username() const;

//    void setUsername(const QString &username);

//    QString privateKey() const;

//    bool setPrivateKey(const QString &key);

//    Level authorization() const;

//    void setAuthorization(Level level);

signals:
    void userPKNotify(int, GenericUser*);
    void usernameChanged(QString);
    void authorizationChanged(int);
    void keyChanged(QString);
    void isValidChanged(bool);

private:

    int m_id;
    QString m_name;
    int m_auth;
    QString m_key;
    bool m_valid;
    //The authorizathion names
    static const char *const _LEVEL_ADMIN_NAME;
    static const char *const _LEVEL_USER_NAME;
    static const char *const _LEVEL_UNKOWN_NAME;

    //The authorizathion descriptions
    static const char *const _LEVEL_ADMIN_DESC;
    static const char *const _LEVEL_USER_DESC;
    static const char *const _LEVEL_UNKOWN_DESC;

    //The username
//    QString _username;

    //The authorization level
//    Level _authorization;
};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::GenericUser*)

#endif // GENERICUSER_H
