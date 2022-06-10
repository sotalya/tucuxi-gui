#include "datacrypter.h"
#include "core/core.h"
//#include "coremessagehandler.h"
////#include "core_errors.h"
#include <botan/pipe.h>
#ifdef Q_WS_WIN
#include <botan/cipher_mode.h>
#endif
#include <botan/b64_filt.h>
#include <botan/key_filt.h>
#include <botan/lookup.h>
#include "core/utils/logging.h"
#include "errors_apputils.h"

#include <QDebug>

using namespace Tucuxi::GuiCore;
namespace Tucuxi {
namespace GuiAppUtils {

DataCrypter::DataCrypter(QObject *parent) :
    QObject(parent)
{
}

QString DataCrypter::createUser(const QString &login, const QString &password)
{
    // Simply concatenate login and password before to feed the hashing function
    QString credential = login + password;

    Botan::HashFunction* hash(Botan::get_hash("SHA-256"));

    // Generate the key to decrypt the user encrypted master key. The hash is used as the symmetric key
    // REMARK : We could also use a KDF (key derivation function) to proceed. This would make the brute force attack weaker
    Botan::SymmetricKey key = hash->process(credential.toStdString());

    // Create the encryption pipe
    Botan::AutoSeeded_RNG rng;
    Botan::InitializationVector iv(rng, 16);
    Botan::Pipe pipe(get_cipher("AES-256/CBC", key, iv, Botan::ENCRYPTION), new Botan::Base64_Encoder());

    // Encrypt some data
    pipe.process_msg(masterKey.as_string());
    QString cipher = QString::fromStdString(pipe.read_all_as_string(0));

    // Return the IV and the ciphertext
    return QString::fromStdString(iv.as_string()) + cipher;
}

std::string DataCrypter::encrypt(const std::string &plain)
{
    if (masterKey.length() <= 0)
    {
        LOG(QtCriticalMsg, NORIGHTS, tr("No masterkey available. A valid user needs to be logged in."));
    }

    try {
        // Create the encryption pipe
        Botan::AutoSeeded_RNG rng;
        Botan::InitializationVector iv(rng, 16);
        Botan::Pipe pipe(get_cipher("AES-256/CBC", masterKey, iv, Botan::ENCRYPTION), new Botan::Base64_Encoder());

        // Encrypt some data
        pipe.process_msg(plain);
        std::string cipher = pipe.read_all_as_string(0);

        // Return the IV and the ciphertext
        return iv.as_string() + cipher;
    }
    catch (...)
    {
        LOG(QtCriticalMsg, ENCRYPTIONFAILED, tr("Unexpected encryption exception"));
    }

    return std::string();
}

std::string DataCrypter::encrypt(const QString &plain)
{
    return encrypt(plain.toStdString());
}

std::string DataCrypter::decrypt(const std::string &encrypted)
{
    if (masterKey.length() <= 0)
    {
        LOG(QtWarningMsg, Tucuxi::GuiAppUtils::NOEZERROR, "No masterkey available. A valid user needs to be logged in.");
        return std::string();
    }

    // The encrypted data contains 2 parts : [IV][Cipher]
    Botan::InitializationVector iv;
    try
    {
        iv = Botan::InitializationVector(encrypted.substr(0,32));
//        iv.change(encrypted.substr(0, 32));
    }
    catch (std::invalid_argument &e)
    {
        LOG(QtWarningMsg, Tucuxi::GuiAppUtils::NOEZERROR, tr("Unexpected data format (%1)").arg(e.what()));
        return std::string();
    }

    std::string cipher = encrypted.size() < 32 ? std::string() : encrypted.substr(32);

    try
    {
        // Create the decryption pipe
        Botan::Pipe pipe(new Botan::Base64_Decoder(), get_cipher("AES-256/CBC", masterKey, iv, Botan::DECRYPTION));

        // Decrypt some data
        pipe.process_msg(cipher);
        return pipe.read_all_as_string(0);
    }
    catch (Botan::Algorithm_Not_Found &e)
    {
        LOG(QtWarningMsg, Tucuxi::GuiAppUtils::NOEZERROR, tr("Algorithm not found (%1)").arg(e.what()));
        return std::string();
    }
    catch (Botan::Invalid_IV_Length &e)
    {
        LOG(QtWarningMsg, Tucuxi::GuiAppUtils::NOEZERROR, tr("Invalid lenght (%1)").arg(e.what()));
        return std::string();
    }
    catch (Botan::Decoding_Error &e)
    {
        LOG(QtWarningMsg, Tucuxi::GuiAppUtils::NOEZERROR, tr("Decoding error (%1)").arg(e.what()));
        return std::string();
    }
    catch (...)
    {
        LOG(QtWarningMsg, Tucuxi::GuiAppUtils::NOEZERROR, tr("Unexpected decryption exception"));
        return std::string();
    }
}

std::string DataCrypter::decrypt(const QString &encrypted)
{
    return decrypt(encrypted.toStdString());
}

void DataCrypter::createMasterKey()
{
    // Create a new random key
    Botan::AutoSeeded_RNG rng;
    masterKey = rng.random_vec(32);
}

void DataCrypter::setMasterKey(const QString &key)
{
    masterKey = Botan::SymmetricKey(key.toStdString());
}

QString DataCrypter::getMasterKey()
{
    return QString::fromStdString(masterKey.as_string());
}

bool DataCrypter::login(const QString &login, const QString &password, const QString &userKey)
{
    // Simply concatenate login and password before to feed the hashing function
    QString credential = login + password;

    Botan::HashFunction* hash(Botan::get_hash("SHA-256"));

    // Generate the key to decrypt the user encrypted master key. The hash is used as the symmetric key
    // REMARK : We could also use a KDF (key derivation function) to proceed. This would make the brute force attack weaker
    Botan::SymmetricKey key = hash->process(credential.toStdString());

    // Now the user key is made in 2 parts : [IV][Cipher]
    Botan::InitializationVector iv(userKey.left(32).toStdString());
    QString cipher = userKey.mid(32);

    // Create the pipe for decrypting the user key
    Botan::Pipe pipe(new Botan::Base64_Decoder(), get_cipher("AES-256/CBC", key, iv, Botan::DECRYPTION));

    // Decrypt the user crypted master key
    try
    {
        pipe.process_msg(cipher.toStdString());
        masterKey = Botan::SymmetricKey(pipe.read_all_as_string(0));
    }

    // Corrupted data or invalid passphrase
    catch (Botan::Decoding_Error)
    {
        return false;
    }

    return true;
}

}//namespace GuiAppUtils
}//namespace Tucuxi
