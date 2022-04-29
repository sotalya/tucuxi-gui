//@@license@@

#ifndef DBHANDLER_H
#define DBHANDLER_H


#include <QSqlDatabase>

#include <QObject>
#include <QStringList>
#include <QFile>

class DBHandler : public QObject
{
    Q_OBJECT
public:
    static DBHandler* instance();
    void setDBPath(QString path);
    void setAnonDBName(QString name);

    bool connect(QString = "");

    bool disconnect();

    bool hasBasicFingerPrint();

    QString getErrMsg();
signals:
    void publishResult(bool result);
    void publishAnonymizeStatus(bool result);
public slots:
    void evaluate();
    void anonymizeDB();
private:
    DBHandler();

    bool enableFkSupport();

    static DBHandler* handler;
    QString path;
    QString name;
    QSqlDatabase db;
    QString errMsg;
};

#endif // DBHANDLER_H
