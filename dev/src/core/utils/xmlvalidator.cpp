//@@license@@

#include "core/utils/xmlvalidator.h"
//#include "coremessagehandler.h"
//#include "core_errors.h"
#include "core/core.h"
#include "core/utils/logging.h"
#include "core/errors_core.h"

#include <QRegularExpression>
#include <QFile>

namespace Tucuxi {
namespace Gui {
namespace Core {


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
    XercesDOMParser validator;

    validator.setValidationScheme(XercesDOMParser::Val_Always);
    validator.setDoNamespaces(true);
    validator.setDoSchema(true);

    QByteArray ba = xsdFilename.toLocal8Bit();
    const char *xsdFilenameChar = ba.data();

    validator.setExternalNoNamespaceSchemaLocation(xsdFilenameChar);

    validator.setErrorHandler(_msgHandler);

    ba = xmlFilename.toLocal8Bit();
    const char *xmlFilenameChar = ba.data();

    try {
        validator.parse(xmlFilenameChar);
    }
    catch (const SAXException& toCatch) {
        return false;
    }

    return true;

//    QXmlSchemaValidator validator;
//    validator.setSchema(initSchema(xsdFilename));
//    validator.setMessageHandler(_msgHandler);

//    QFile file(xmlFilename);
//    if (!file.open(QIODevice::ReadOnly)) {
//        LOG(QtWarningMsg, INVALIDFILEFORMAT, tr("Cannot open the XML file '%1'").arg(file.fileName()));
//        return false;
//    }

//    return validator.validate(&file, QUrl::fromLocalFile(file.fileName()));
//    LOG(QtWarningMsg, NOPOTATO, tr("XML Validator called"));
//    return false;
}

bool XmlValidator::validate(QIODevice *xmlDevice, const QString &xsdFilename)
{
//    QXmlSchemaValidator validator;
//    validator.setSchema(initSchema(xsdFilename));
//    validator.setMessageHandler(_msgHandler);

//    return validator.validate(xmlDevice);
    LOG(QtWarningMsg, NOPOTATO, tr("XML QIODevice Validator called"));
    return false;
}

bool XmlValidator::validate(const QByteArray &xmlData, const QString &xsdFilename)
{
//    QXmlSchemaValidator validator;
//    validator.setSchema(initSchema(xsdFilename));
//    validator.setMessageHandler(_msgHandler);

//    return validator.validate(xmlData);
    LOG(QtWarningMsg, NOPOTATO, tr("XML Validator called"));
    return false;
}

QtMsgType XmlValidator::errorType() const
{
    return _msgHandler->type();
}

QString XmlValidator::errorMessage() const
{
    QString message = _msgHandler->description();
    return message.remove(QRegularExpression("<[^>]*>"));
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

//QXmlSchema XmlValidator::initSchema(const QString &path) const
//{
//    QXmlSchema schema;
//    schema.load(QUrl::fromLocalFile(path));

//    if (!schema.isValid())
//        LOG(QtCriticalMsg, XSDERROR, QString(tr("Invalid XML Schema file '%1'")).arg(path));

//    return schema;
//}

XmlValidator::ValidatorMessageHandler::ValidatorMessageHandler():
    _exeption((const XMLCh *const)"", (const XMLCh *const)"", (const XMLCh *const)"",0,0)
{

}

void XmlValidator::ValidatorMessageHandler::warning(const SAXParseException &exc)
{
    _type = QtWarningMsg;
    _description = QString(XMLString::transcode(exc.getMessage()));
    _exeption = exc;

}

void XmlValidator::ValidatorMessageHandler::error(const SAXParseException &exc)
{
    _type = QtCriticalMsg;
    _description = QString(XMLString::transcode(exc.getMessage()));
    _exeption = exc;
    throw SAXException(exc);
}

void XmlValidator::ValidatorMessageHandler::fatalError(const SAXParseException &exc)
{
    _type = QtFatalMsg;
    _description = QString(XMLString::transcode(exc.getMessage()));
    _exeption = exc;
    throw SAXException(exc);
}

void XmlValidator::ValidatorMessageHandler::resetErrors()
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
    return _exeption.getLineNumber();
}

int XmlValidator::ValidatorMessageHandler::column() const
{
    return _exeption.getColumnNumber();
}

void XmlValidator::ValidatorMessageHandler::handleMessage(QtMsgType type, const QString &description, const QUrl &identifier, const SAXParseException &sourceExeption)
{
    Q_UNUSED(identifier);

    _type = type;
    _description = description;
    _exeption = sourceExeption;
}

} // namespace Core
} // namespace Gui
} // namespace Tucuxi
