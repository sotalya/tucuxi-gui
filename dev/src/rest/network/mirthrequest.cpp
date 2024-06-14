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


#include "rest/network/mirthrequest.h"
#include "rest/model/message.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

QString MirthRequest::_DEFAULT_SCHEME    = "https";
QString MirthRequest::_DEFAULT_USERNAME  = "";
QString MirthRequest::_DEFAULT_PASSWORD  = "";
QString MirthRequest::_DEFAULT_HOST      = "ezecs1";
QString MirthRequest::_DEFAULT_PATH_BASE = "/services";
QString MirthRequest::_DEFAULT_PATH      = "";

int MirthRequest::_DEFAULT_PORT = 8889;

void MirthRequest::updateDefaultScheme(const QString &scheme)
{
    _DEFAULT_SCHEME = scheme;
}

void MirthRequest::updateDefaultUsername(const QString &username)
{
    _DEFAULT_USERNAME = username;
}

void MirthRequest::updateDefaultPassword(const QString &password)
{
    _DEFAULT_PASSWORD = password;
}

void MirthRequest::updateDefaultHost(const QString &host)
{
    _DEFAULT_HOST = host;
}

void MirthRequest::updateDefaultPort(int port)
{
    _DEFAULT_PORT = port;
}

void MirthRequest::updateDefaultBasePath(const QString &basePath)
{
    _DEFAULT_PATH_BASE = basePath;
}

void MirthRequest::updateDefaultPath(const QString &path)
{
    _DEFAULT_PATH = path;
}

MirthRequest::MirthRequest() :
    _message(),
    _scheme(_DEFAULT_SCHEME),
    _username(_DEFAULT_USERNAME),
    _password(_DEFAULT_PASSWORD),
    _host(_DEFAULT_HOST),
    _port(_DEFAULT_PORT),
    _path(_DEFAULT_PATH)
{
    setHeader(QNetworkRequest::ContentTypeHeader, "text/xml");

    updateUrl();
}

MirthRequest::~MirthRequest()
{

}

QByteArray MirthRequest::postData() const
{
    if (_message.isNull())
        return QByteArray();

    return _message->data();
}

void MirthRequest::setMessage(Message *message)
{
    _message.reset(message);
}

void MirthRequest::clearMessage()
{
    _message.reset();
}

void MirthRequest::updateScheme(const QString &scheme)
{
    _scheme = scheme;
    updateUrl();
}

void MirthRequest::updateUsername(const QString &username)
{
    _username = username;
    updateUrl();
}

void MirthRequest::updatePassword(const QString &password)
{
    _password = password;
    updateUrl();
}

void MirthRequest::updateHost(const QString &host)
{
    _host = host;
    updateUrl();
}

void MirthRequest::updatePort(int port)
{
    _port = port;
    updateUrl();
}

void MirthRequest::updatePath(const QString &path)
{
    _path = QString(_DEFAULT_PATH_BASE) + "/" + path;
    updateUrl();
}

void MirthRequest::updateUrl()
{
    QUrl url;
    url.setScheme(_scheme);
    url.setUserName(_username);
    url.setPassword(_password);
    url.setHost(_host);
    url.setPort(_port);
    url.setPath(_path);

    Q_ASSERT(url.isValid());
    setUrl(url);
}

}
}
}
