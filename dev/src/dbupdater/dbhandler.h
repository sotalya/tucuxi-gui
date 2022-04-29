//@@license@@

#ifndef DBHANDLER_H
#define DBHANDLER_H


#include <QSqlDatabase>

#include <QObject>
#include <QStringList>

class DBHandler : public QObject
{
    Q_OBJECT
public:
    static DBHandler* instance();
    void setDBPath(QString path);

    bool connect();

    bool disconnect();

    bool hasBasicFingerPrint();

    QString getErrMsg();
signals:
    void publishResult(bool result, QStringList patches);
    void publishUpdateStatus(bool result);
public slots:
    void evaluate();
    void updateDB();
private:
    DBHandler();
    bool enableFkSupport();
    bool update_0_2_1(const QString &version);
    bool update_0_2_2(const QString &version);
    bool update_0_2_3(const QString &version);
    bool update_0_2_4(const QString &version);
    bool update_0_2_5(const QString &version);
    bool update_0_2_6(const QString &version);
    bool update_0_2_7(const QString &version);
    bool updateVersion(const QString &version);

    static DBHandler* handler;
    QString path;
    QSqlDatabase db;
    QStringList versions;
    QString errMsg;
};

#endif // DBHANDLER_H
