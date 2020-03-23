#ifndef UTILS_H
#define UTILS_H

#include <QTextStream>

inline QTextStream& qMessage()
{
    static QTextStream ts(stdout);
    return ts;
}

#endif // UTILS_H
