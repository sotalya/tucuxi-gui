#include "ackrequesttestwidget.h"

#include "rest/network/queryrequestrequest.h"
#include "rest/network/ackrequestrequest.h"
#include "rest/model/ackrequestxmlmessage.h"

#include <QPlainTextEdit>

AckRequestTestWidget::AckRequestTestWidget(QWidget *parent) :
    RequestTestWidget(new QueryRequestRequest, parent)
{
    setAcknowledgement(new AckRequestRequest, new AckRequestXmlMessage);
}

AckRequestTestWidget::~AckRequestTestWidget()
{

}

void AckRequestTestWidget::displaySuccess()
{
    QDomDocument doc;
    if (doc.setContent(_reply->data()))
        sendAck(doc, "ack_request", "AA");

    textArea()->setPlainText(_reply->isValid() ? tr("SUCCESS: The message is valid!") : tr("FAILURE: ") + _reply->errorString());
}
