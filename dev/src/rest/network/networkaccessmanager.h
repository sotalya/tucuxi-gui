//@@license@@

#ifndef NETWORKACCESSMANAGER_H
#define NETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>

class NetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT

public:

    enum class ManagerType {
        Default,
        Unique
    };

    static void setType(ManagerType _type);

    static NetworkAccessManager &getInstance();
//public slots:
//    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);

    // Sets a unique Ca certificate. No other one previously recorded would be used
    void setUniqueDefaultCaCertificate(const QString &fileName);

private:
    explicit NetworkAccessManager(QObject *parent = nullptr);
    virtual ~NetworkAccessManager();

    void addDefaultCaCertificate(const QString &fileName);

    static ManagerType sm_type;  // NOLINT(readability-identifier-naming)

};

#endif // NETWORKACCESSMANAGER_H
