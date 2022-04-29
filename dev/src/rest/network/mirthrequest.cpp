//@@license@@

#include "rest/network/mirthrequest.h"
#include "rest/model/message.h"

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
