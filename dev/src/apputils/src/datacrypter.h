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


#ifndef DATACRYPTER_H
#define DATACRYPTER_H

#include <QObject>

#ifdef __ANDROID__
//#include "botan_all.h"
#else
#include <botan/init.h>
#include <botan/botan.h>
#include <botan/bcrypt.h>
#include <botan/sha2_32.h>
#include <botan/aes.h>
#endif

using namespace Botan;

namespace Tucuxi {
namespace Gui {
namespace AppUtils {
class DataCrypter : public QObject
{
    Q_OBJECT
public:
    explicit DataCrypter(QObject *parent = nullptr);
    
    /**
     * @brief Creates a new user using the given credentials. This only works if a user is currently logged in.
     * @param login The new user login
     * @param password The new user password. Needs to be as complex as possible
     * @return The user encrypted master key or an NULL string if the procedure failed
     */
    QString createUser(const QString &login, const QString &password);

    /**
     * @brief Encrypt some data using the master key if a user is currently logged in
     * @param plain Plain text data in standard string format
     * @return The encrypted data. Returns an empty string if the operation failed
     */
    std::string encrypt(const std::string &plain);

    /**
     * @brief Encrypt some data using the master key if a user is currently logged in
     * @param plain Plain text data in QString format
     * @return The encrypted data. Returns an empty string if the operation failed
     */
    std::string encrypt(const QString &plain);

    /**
     * @brief Decrypt some encrypted data using the master key if a user is currently logged in
     * @param encrypted Encrypted text data in standard string format
     * @return Plain text data. Returns an empty string if the operation failed
     */
    std::string decrypt(const std::string &encrypted);

    /**
     * @brief Decrypt some encrypted data using the master key if a user is currently logged in
     * @param encrypted Encrypted text data in QString format
     * @return Plain text data. Returns an empty string if the operation failed
     */
    std::string decrypt(const QString &encrypted);

    /**
     * @brief Creates a random master key
     */
    void createMasterKey();

    /**
     * @brief Sets the master key. Only for debugging purpose !
     * @param key The master key
     */
    void setMasterKey(const QString &key);

    /**
     * @brief Gets the master key. Only for debugging purpose !
     * @return
     */
    QString getMasterKey();

signals:
    
public slots:

    /**
     * @brief Log a new user in
     * @param login The user login
     * @param password The user password
     * @param userKey The user key
     * @return True if login succeeded
     */
    bool login(const QString &login, const QString &password, const QString &userKey);

private:
    //Library initializer (must be created first)
//    Botan::LibraryInitializer init;

    //Symmetric master key
    Botan::SymmetricKey masterKey;
};

}//namespace AppUtils
}//namespace Gui
}//namespace Tucuxi
#endif // DATACRYPTER_H
