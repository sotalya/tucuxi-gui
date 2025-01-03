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


#ifndef ERROR_H
#define ERROR_H

#include <QString>
#include <QVariant>

namespace Tucuxi {
namespace Gui {
namespace Core {


enum ErrorCode {
    //! Everything went ok
    NoError = 0x0,
    //! No backend was able to handle this request
    NoBackend = 0x1,
    //! The wanted object wasn't found in the database
    NotFound = 0x2,
    //! You don't have the right to see this information
    NoAccess = 0x3,
    //! The query preparation or execution failed
    QueryError = 0x4
};

//! Structure returned by the requests
/** \ingroup database
   This object contains the error code (which can be NoError) and the database that responded.
  */
struct Response {
    Response() : error(NoError) { }
    Response(QString responder) : error(NoError), responder(responder) {}
    Response(QString responder, ErrorCode error) : error(error), responder(responder) {}
    //! Error code associated with the request
    ErrorCode error;
    //! Database that responded
    QString responder;
    //! Query error message
    QString message;
    QList<QObject*> results;

    template<typename T>
    QList<T> castresults() {
        QList<T> ret;
        foreach(QObject* qobjstar, results) {
            ret.append(qobject_cast<T>(qobjstar));
        }
        return ret;
    }
};

//class ScriptResponse : public QScriptClass {
//public:
//    Q_INVOKABLE ScriptResponse(QObject* parent) { this->setParent(parent); }
//    ScriptResponse(QList<QObject*> list) {
//        foreach (QObject* qobj, list) {
//            this->append(qobj);
//        }
//    }
//};

//Q_DECLARE_METATYPE(ScriptResponse*)
//Q_SCRIPT_DECLARE_QMETAOBJECT(ScriptResponse, QObject*)


} // namespace Core
} // namespace Gui
} // namespace Tucuxi

#endif // ERROR_H
