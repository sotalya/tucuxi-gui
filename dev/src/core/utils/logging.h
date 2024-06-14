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


#ifndef LOGGING
#define LOGGING

//#include <QApplication>
#include <QDebug>
#include <QFile>

namespace Tucuxi {

#define EZERROR(MODULE, NAME, DESC) \
    struct NAME { \
        static QString report() { \
            return QString("Module: %1, ErrorName: %2, Description: %3 ").arg(QT_TR_NOOP(#MODULE), QT_TR_NOOP(#NAME), QT_TR_NOOP(DESC)); \
        } \
    };

enum class ProcessingResult
{
    Success,
    Failure,
    Aborted
};

namespace logging
{

class Logger
{
public:
    static void initLogFile(QString appDirectory);

    template < typename error >
    static inline void log(QtMsgType type, QString msg, const char * file, int line, const char * function)
    {
        QString strType;
        switch(type)
        {
            case(QtDebugMsg):    strType="Debug"; break;
            case(QtInfoMsg):     strType="Info"; break;
            case(QtWarningMsg):  strType="Warning"; break;
            case(QtCriticalMsg): strType="Critical"; break;
            case(QtFatalMsg):    strType="Fatal"; break;
            default:             strType=""; break;
        }

        QString formattedMsg = QString("%1: %2: %3 (%4:%5, %6)\n").arg(strType, error::report(), msg, file, QString::number(line), function);

        if (logFile.isOpen())
        {
            logFile.write(formattedMsg.toLocal8Bit());
        }

        #ifdef QT_DEBUG
        switch(type)
        {
            case(QtDebugMsg):    qDebug() << formattedMsg; break;
            case(QtInfoMsg):     qInfo() << formattedMsg; break;
            case(QtWarningMsg):  qWarning() << formattedMsg; break;
            case(QtCriticalMsg): qCritical() << formattedMsg; break;
            case(QtFatalMsg):    qCritical() << formattedMsg; abort(); break;
            default:             qInfo() << formattedMsg; break;
        }
        #endif
    }
private:
    static QFile logFile;
};

} //namespace logging

#define OWNLOG(ERRTYPE, ERROR, MSG) { LOG(ERRTYPE, ERROR, MSG); this->log<ERROR>(ERRTYPE, MSG, __FILE__, __LINE__, __FUNCTION__); }
#define LOG(ERRTYPE, ERROR, MSG) { logging::Logger::log<ERROR>(ERRTYPE, MSG, __FILE__, __LINE__, __FUNCTION__); }
#define EXLOG(ERRTYPE, ERROR, MSG) { Tucuxi::logging::Logger::log<ERROR>(ERRTYPE, MSG, __FILE__, __LINE__, __FUNCTION__); }
#define PRECOND(CONDITION, ERROR, MSG) \
    if (!(CONDITION)) LOG(QtFatalMsg, ERROR, MSG);
#define POSTCOND(CONDITION, ERROR, MSG) \
    if (!(CONDITION)) LOG(QtFatalMsg, ERROR, MSG);
#define PRECONDCONT(CONDITION, ERROR, MSG) \
    if (!(CONDITION)) { \
        LOG(QtWarningMsg, ERROR, MSG); \
        return ProcessingResult::Failure;\
    }
#define POSTCONDCONT(CONDITION, ERROR, MSG) \
    if (!(CONDITION)) { \
        LOG(QtWarningMsg, ERROR, MSG); \
        return ProcessingResult::Failure;\
    }
#define POSTCONDCONT2(CONDITION, ERROR, MSG) \
    if (!(CONDITION)) { \
        if (logIfFailedCondition) \
            LOG(QtWarningMsg, ERROR, MSG); \
        return ProcessingResult::Failure;\
    }
} //namespace Tucuxi
#endif // LOGGING

