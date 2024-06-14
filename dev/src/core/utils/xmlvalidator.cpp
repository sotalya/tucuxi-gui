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


#include "core/utils/xmlvalidator.h"
//#include "coremessagehandler.h"
//#include "core_errors.h"
#include "core/core.h"
#include "core/utils/logging.h"
#include "core/errors_core.h"

#include <QRegularExpression>
#include <QFile>
#include <QTemporaryDir>

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

XmlValidator::XmlValidator()
{
    try {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        LOG(QtCriticalMsg, OBJECTISINVALID, tr("Error during initialization of xerces-c '%1'").arg(message));
        XMLString::release(&message);
    }

    _msgHandler = new ValidatorMessageHandler;
}

XmlValidator::~XmlValidator()
{
    if (_msgHandler)
        delete _msgHandler;
}

bool XmlValidator::validate(const QString &xmlFilename, const QString &xsdFilename)
{
    QTemporaryDir tmpDir;
    initTempSchemaFolder(xsdFilename, tmpDir);
    QString xsdFilePath = tmpDir.path()+"/"+ QFileInfo(xsdFilename).fileName();

    QByteArray ba = QUrl::fromLocalFile(xsdFilePath).toString().toLocal8Bit();
    const char *xsdFilePathChar = ba.data();

    XercesDOMParser validator;
    validator.setValidationScheme(XercesDOMParser::Val_Always);
    validator.setDoNamespaces(true);
    validator.setDoSchema(true);
    validator.setLoadSchema(false);
    validator.setExternalNoNamespaceSchemaLocation(xsdFilePathChar);

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
}

bool XmlValidator::validate(const QByteArray &xmlData, const QString &xsdFilename)
{
    QTemporaryDir tmpDir;
    initTempSchemaFolder(xsdFilename, tmpDir);
    QString xsdFilePath = tmpDir.path()+"/"+ QFileInfo(xsdFilename).fileName();

    QByteArray ba = xsdFilePath.toLocal8Bit();
    const char *xsdFilePathChar = ba.data();

    XercesDOMParser validator;

    validator.setValidationScheme(XercesDOMParser::Val_Always);
    validator.setDoNamespaces(true);
    validator.setDoSchema(true);
    validator.setLoadSchema(false);
    validator.setExternalNoNamespaceSchemaLocation(xsdFilePathChar);

    validator.setErrorHandler(_msgHandler);

    std::string tmp(xmlData.constData(), xmlData.length());
    const char* inSrc = tmp.c_str();

    MemBufInputSource src((const XMLByte*)inSrc, strlen(inSrc), "dummy", false);

    try {
        validator.parse(src);
    }
    catch (const SAXException& toCatch) {
        return false;
    }

    return true;
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

void XmlValidator::initTempSchemaFolder(const QString &path, const QTemporaryDir &tmpDir) const
{
    // get current dir and copy all files into the temp folder
    QDir dir = QFileInfo(path).absoluteDir();
    //QString pathName = QFileInfo(path).fileName();

    for (const QString &filename : dir.entryList(QDir::Files)){
        QFile::copy(dir.path()+"/"+filename, tmpDir.path()+"/"+filename);
    }
}

XmlValidator::ValidatorMessageHandler::ValidatorMessageHandler():
    _exeption(XMLString::transcode(""), XMLString::transcode(""), XMLString::transcode(""),0,0)
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
    //throw SAXException(exc);
}

void XmlValidator::ValidatorMessageHandler::fatalError(const SAXParseException &exc)
{
    _type = QtFatalMsg;
    _description = QString(XMLString::transcode(exc.getMessage()));
    _exeption = exc;
    //throw SAXException(exc);
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
