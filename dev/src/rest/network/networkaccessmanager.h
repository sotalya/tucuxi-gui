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


#ifndef NETWORKACCESSMANAGER_H
#define NETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>

namespace Tucuxi {
namespace Gui {
namespace Rest {

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

}
}
}


#endif // NETWORKACCESSMANAGER_H
