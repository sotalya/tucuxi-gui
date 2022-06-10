//@@license@@

#include "replylisttestmessage.h"
#include "restclient/utils/xmltestlibrary.h"
#include "restclient/utils/utils.h"
#include "core/utils/logging.h"
#include "restclient/errors_restclient.h"

#include <QDomDocument>

ReplyListTestMessage::ReplyListTestMessage(QObject *parent) :
    ReplyListXmlMessage(parent),
    TestMessageValidator()
{

}

ReplyListTestMessage::~ReplyListTestMessage()
{

}

void ReplyListTestMessage::setData(QByteArray data)
{
    ReplyListXmlMessage::setData(data);

    QDomDocument doc;
    QString qxmlError;
    int errline = -1;
    int columnline = -1;
    if (!doc.setContent(data,false, &qxmlError, &errline, &columnline)) {
        QString raw = QString::fromLocal8Bit(data);
//        displayError(replyError.isEmpty() ? tr("Could not generate DOM for the 'REPLY_LIST' message") : replyError);
        EXLOG(QtFatalMsg, Tucuxi::restclient::NOEZERROR, tr("QXmlError parsing DOM with error: %1, at line: %2, at column: %3, raw data: %4").arg(qxmlError).arg(errline).arg(columnline).arg(raw));
        return;
    }

    validate(doc.toString(2), XmlTestLibrary::getInstance().replyList().toString(2), messageType());
}
