//@@license@@

#ifndef RESTLOGGER_H
#define RESTLOGGER_H

#include <QString>

class RestLogger
{

public:

    static RestLogger *getInstance();

    void logList(const QString& text);
    void logRequest(const QString& text);

    void setPath(const QString &path);

private:
    RestLogger();

    QString path;
    QString currentPath;

};

#endif // RESTLOGGER_H
