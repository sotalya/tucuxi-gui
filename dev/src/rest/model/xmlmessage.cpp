//@@license@@

#include "xmlmessage.h"
#include "core/utils/xmlvalidator.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {


XmlMessage::XmlMessage(QObject *parent) :
    Message(parent),
    _data()
{

}

void XmlMessage::setData(QByteArray data)
{
    _data = data;
}

QByteArray XmlMessage::data() const
{
    return _data;
}

XmlMessage::~XmlMessage()
{

}

bool XmlMessage::isValid()
{
    QByteArray d = data();
    if (d.isEmpty()) {
        setErrorString(tr("The message type '%1' appears to be empty").arg(messageType()));
        return false;
    }

    Tucuxi::Gui::Core::XmlValidator validator;
    if (!validator.validate(d, schemaPath())) {
        setErrorString(tr("The message type '%1' is invalid (%2)").arg(messageType(), validator.errorMessage()));
        return false;
    }

    return true;
}

}
}
}
