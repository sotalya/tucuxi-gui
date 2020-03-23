#ifndef MIRTHREQUEST_H
#define MIRTHREQUEST_H

class Message;

#include <QNetworkRequest>

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

#endif // MIRTHREQUEST_H
