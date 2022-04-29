//@@license@@

#include "acknotificationtestmessage.h"
#include "restclient/utils/xmltestlibrary.h"
#include "restclient/utils/utils.h"

#include <QDomDocument>

AckNotificationTestMessage::AckNotificationTestMessage(const QDomDocument &expected, QObject *parent) :
    AckNotificationXmlMessage(parent),
    TestMessageValidator(),
    _expected(expected)
{

}

AckNotificationTestMessage::~AckNotificationTestMessage()
{

}

void AckNotificationTestMessage::setData(QByteArray data)
{
    AckNotificationXmlMessage::setData(data);

    QDomDocument doc;
    if (!doc.setContent(data)) {
        qMessage() << tr("Could not generate DOM for the message") << " '" << messageType() << "'" << endl;
        return;
    }

    validate(doc.toString(2), _expected.toString(2), messageType());
}
