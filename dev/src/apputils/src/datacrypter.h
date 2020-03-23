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

namespace ezechiel {
namespace apputils {
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

}//namespace apputils
}//namespace ezechiel
#endif // DATACRYPTER_H
