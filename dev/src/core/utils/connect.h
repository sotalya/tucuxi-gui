
#ifndef FILE_CONNECT_H
#define FILE_CONNECT_H

#include <QMessageBox>

#define COUCOU(c) c

#define CHECK_INVOKEMETHOD(invocation) \
    if (!(invocation)) \
    { \
        QString mess;\
        mess =QString("InvokeMethod error in file %1, line %2.").arg(__FILE__).arg(__LINE__);\
        QMessageBox::warning(nullptr, "Tucuxi",mess);\
    }

#define CONNECT(a,b,c,d) \
   if (!connect(a,b,c,d))\
           {\
               QString mess;\
               mess =QString("Signal connection error in file %1, line %2. %3: %4::%5").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__).arg(#a).arg(#b);\
               QMessageBox::warning(nullptr, "Tucuxi",\
                   mess);\
           }


#endif // FILE_CONNECT_H
