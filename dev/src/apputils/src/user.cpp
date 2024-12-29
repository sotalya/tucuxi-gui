/*
 * Tucuxi - Tucuxi-gui software.
 * This software is able to perform prediction of drug concentration in blood
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


#include "apputils/src/user.h"
#include "core/core.h"
//#include "dbinterface.h"

//The authorizathion names
const char *const User::_LEVEL_ADMIN_NAME  = QT_TR_NOOP_UTF8("Administrator");
const char *const User::_LEVEL_USER_NAME   = QT_TR_NOOP_UTF8("Tucuxi User");
const char *const User::_LEVEL_UNKOWN_NAME = QT_TR_NOOP_UTF8("Unknown Level");

//The authorizathion descriptions
const char *const User::_LEVEL_ADMIN_DESC  = QT_TR_NOOP_UTF8("An administrator can create new user as well as edit the existing users. He does have full access to the database and software functionalities.");
const char *const User::_LEVEL_USER_DESC   = QT_TR_NOOP_UTF8("An Tucuxi user can modify its own information and have full access to the database and software functionalities.");
const char *const User::_LEVEL_UNKOWN_DESC = QT_TR_NOOP_UTF8("Unknown or invalid authorization level.");

//Returns a level name
QString User::levelName(User::Level level)
{
    switch (level) {
    case UserLevel:  return tr(_LEVEL_USER_NAME);
    case AdminLevel: return tr(_LEVEL_ADMIN_NAME);
    default:         return tr(_LEVEL_UNKOWN_NAME);
    }
}

//Returns a level description
QString User::levelDesc(User::Level level)
{
    switch (level) {
    case UserLevel:  return tr(_LEVEL_USER_DESC);
    case AdminLevel: return tr(_LEVEL_ADMIN_DESC);
    default:         return tr(_LEVEL_UNKOWN_DESC);
    }
}

//Constructor
User::User() : _authorization(UserLevel)
{

}

//Constructor
User::User(const QString &username, Level level) : _username(username), _authorization(level)
{

}

//Checks if the user is valid
bool User::isValid() const
{
    return !_username.isEmpty() && !privateKey().isEmpty();
}

//Returns the username
QString User::username() const
{
    return _username;
}

//Sets the username
void User::setUsername(const QString &username)
{
    _username = username;
}

//Returns the private key
QString User::privateKey() const
{
    QString key;

    //Check if the username is valid
    if (_username.isEmpty())
        return QString();

    //Retrieve the key from the database
    if (APPUTILSREPO->getUserKey(_username, key).error != Tucuxi::Gui::Core::NoError)
        return QString();

    return key;
}

//Sets the private key
bool User::setPrivateKey(const QString &key)
{
    //Check if the username is valid
    if (_username.isEmpty())
        return false;

    //Sets the user private key
    if (APPUTILSREPO->setUserKey(_username, key).error != Tucuxi::Gui::Core::NoError)
        return false;

    return true;
}

//Returns the authorization level
User::Level User::authorization() const
{
    return _authorization;
}

//Sets the authorization level
void User::setAuthorization(Level level)
{
    _authorization = level;
}
