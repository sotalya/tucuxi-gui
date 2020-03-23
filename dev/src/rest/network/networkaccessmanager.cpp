#include "networkaccessmanager.h"

#include <QFile>
#include <QNetworkReply>

NetworkAccessManager::ManagerType NetworkAccessManager::sm_type = NetworkAccessManager::ManagerType::Default;

void NetworkAccessManager::setType(ManagerType _type)
{
    sm_type = _type;
}

NetworkAccessManager &NetworkAccessManager::getInstance()
{
    static NetworkAccessManager nm;
    return nm;
}

NetworkAccessManager::NetworkAccessManager(QObject *parent) :
    QNetworkAccessManager(parent)
{
    if (sm_type == ManagerType::Default) {
    QStringList certificates;
    certificates << ":/certificates/mirth.hevs.ch.crt"
                 << ":/certificates/mirth.chuv.ch.pem";

    while (!certificates.isEmpty())
        addDefaultCaCertificate(certificates.takeFirst());
    }
//    connect(&NetworkAccessManager::getInstance(), SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
}

NetworkAccessManager::~NetworkAccessManager()
{

}

//void NetworkAccessManager::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors) {
//   reply->ignoreSslErrors();
//}

void NetworkAccessManager::addDefaultCaCertificate(const QString &fileName)
{
    QFile certificationFile(fileName);
    if (!certificationFile.open(QIODevice::ReadOnly))
        qFatal(qPrintable(tr("Could not open the certificate '%1'").arg(fileName)));

    QSslCertificate certificate(certificationFile.readAll());
    QSslSocket::addDefaultCaCertificate(certificate);
}

void NetworkAccessManager::setUniqueDefaultCaCertificate(const QString &fileName)
{
    addDefaultCaCertificate(fileName);
}
