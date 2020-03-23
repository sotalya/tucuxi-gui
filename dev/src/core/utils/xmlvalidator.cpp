#include "core/utils/xmlvalidator.h"
//#include "coremessagehandler.h"
//#include "core_errors.h"
#include "core/core.h"
#include "core/utils/logging.h"
#include "core/errors_core.h"

#include <QXmlSchema>
#include <QRegExp>
#include <QFile>

namespace ezechiel {
namespace core {

//Drug XML schemas
const char *const XmlValidator::Drug = ":/schemas/drug.xsd";

//Exchange XML schemas
const char *const XmlValidator::Import = ":/schemas/import.xsd";
const char *const XmlValidator::Export = ":/schemas/export.xsd";

//Messages XML schemas
const char *const XmlValidator::Queries       = ":/schemas/queries.xsd";
const char *const XmlValidator::Reply_List    = ":/schemas/reply_list.xsd";
const char *const XmlValidator::Reply_Request = ":/schemas/reply_request.xsd";
const char *const XmlValidator::Notification  = ":/schemas/notification.xsd";
const char *const XmlValidator::Acks          = ":/schemas/acks.xsd";

XmlValidator::XmlValidator() :
    _msgHandler(new ValidatorMessageHandler)
{

}

XmlValidator::~XmlValidator()
{
    if (_msgHandler)
        delete _msgHandler;
}

bool XmlValidator::validate(const QString &xmlFilename, const QString &xsdFilename)
{
    QXmlSchemaValidator validator;
    validator.setSchema(initSchema(xsdFilename));
    validator.setMessageHandler(_msgHandler);

    QFile file(xmlFilename);
    if (!file.open(QIODevice::ReadOnly)) {
        LOG(QtWarningMsg, INVALIDFILEFORMAT, tr("Cannot open the XML file '%1'").arg(file.fileName()));
        return false;
    }

    return validator.validate(&file, QUrl::fromLocalFile(file.fileName()));
}

bool XmlValidator::validate(QIODevice *xmlDevice, const QString &xsdFilename)
{
    QXmlSchemaValidator validator;
    validator.setSchema(initSchema(xsdFilename));
    validator.setMessageHandler(_msgHandler);

    return validator.validate(xmlDevice);
}

bool XmlValidator::validate(const QByteArray &xmlData, const QString &xsdFilename)
{
    QXmlSchemaValidator validator;
    validator.setSchema(initSchema(xsdFilename));
    validator.setMessageHandler(_msgHandler);

    return validator.validate(xmlData);
}

QtMsgType XmlValidator::errorType() const
{
    return _msgHandler->type();
}

QString XmlValidator::errorMessage() const
{
    QString message = _msgHandler->description();
    return message.remove(QRegExp("<[^>]*>"));
}

QString XmlValidator::errorHtmlMessage() const
{
    return _msgHandler->description();
}

int XmlValidator::errorLine() const
{
    return _msgHandler->line();
}

int XmlValidator::errorColumn() const
{
    return _msgHandler->column();
}

QXmlSchema XmlValidator::initSchema(const QString &path) const
{
    QXmlSchema schema;
    schema.load(QUrl::fromLocalFile(path));

    if (!schema.isValid())
        LOG(QtCriticalMsg, XSDERROR, QString(tr("Invalid XML Schema file '%1'")).arg(path));

    return schema;
}

XmlValidator::ValidatorMessageHandler::ValidatorMessageHandler() : QAbstractMessageHandler(0)
{

}

QString XmlValidator::ValidatorMessageHandler::description() const
{
    return _description;
}

QtMsgType XmlValidator::ValidatorMessageHandler::type() const
{
    return _type;
}

int XmlValidator::ValidatorMessageHandler::line() const
{
    return _location.line();
}

int XmlValidator::ValidatorMessageHandler::column() const
{
    return _location.column();
}

void XmlValidator::ValidatorMessageHandler::handleMessage(QtMsgType type, const QString &description, const QUrl &identifier, const QSourceLocation &sourceLocation)
{
    Q_UNUSED(identifier);

    _type = type;
    _description = description;
    _location = sourceLocation;
}

} // namespace core
} // namespace ezechiel
