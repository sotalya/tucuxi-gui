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


#ifndef MIRTHREQUEST_H
#define MIRTHREQUEST_H


#include <QNetworkRequest>


namespace Tucuxi {
namespace Gui {
namespace Rest {

class Message;

class MirthRequest : public QNetworkRequest
{

public:
    static void updateDefaultScheme(const QString &scheme);
    static void updateDefaultUsername(const QString &username);
    static void updateDefaultPassword(const QString &password);
    static void updateDefaultHost(const QString &host);
    static void updateDefaultPort(int port);
    static void updateDefaultBasePath(const QString &basePath);
    static void updateDefaultPath(const QString &path);

    MirthRequest();
    virtual ~MirthRequest() = 0;

    virtual QByteArray postData() const;

    virtual void setMessage(Message *message);
    virtual void clearMessage();

protected:
    void updateScheme(const QString &scheme);
    void updateUsername(const QString &username);
    void updatePassword(const QString &password);
    void updateHost(const QString &host);
    void updatePort(int port);
    void updatePath(const QString &path);

private:
    static QString _DEFAULT_SCHEME;
    static QString _DEFAULT_USERNAME;
    static QString _DEFAULT_PASSWORD;
    static QString _DEFAULT_HOST;
    static QString _DEFAULT_PATH_BASE;
    static QString _DEFAULT_PATH;

    static int _DEFAULT_PORT;

    void updateUrl();

    QScopedPointer<Message> _message;

    QString _scheme;
    QString _username;
    QString _password;
    QString _host;
    int _port;
    QString _path;
};

}
}
}

#endif // MIRTHREQUEST_H
