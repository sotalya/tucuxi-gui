//@@license@@

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
