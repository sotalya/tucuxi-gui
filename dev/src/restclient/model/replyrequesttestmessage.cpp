#include "replyrequesttestmessage.h"
#include "restclient/utils/xmltestlibrary.h"
#include "restclient/utils/utils.h"

#include <QDomDocument>

ReplyRequestTestMessage::ReplyRequestTestMessage(const QString &forRequestId, QObject *parent) :
    ReplyRequestXmlMessage(parent),
    TestMessageValidator(),
    _requestId(forRequestId)
{

}

ReplyRequestTestMessage::~ReplyRequestTestMessage()
{

}

void ReplyRequestTestMessage::setData(QByteArray data)
{
    ReplyRequestXmlMessage::setData(data);

    QDomDocument doc;
    if (!doc.setContent(data)) {
        qMessage() << tr("Could not generate DOM for the message") << " '" << messageType() << "'" << endl;
        return;
    }

    validate(doc.toString(2), XmlTestLibrary::getInstance().replyRequest(_requestId).toString(2), messageType());
}
