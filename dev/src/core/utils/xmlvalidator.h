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


#ifndef XMLVALIDATOR_H
#define XMLVALIDATOR_H

#include <QCoreApplication>
#include <QtXml>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>

namespace Tucuxi {
namespace Gui {
namespace Core {

using namespace xercesc;


class XmlValidator
{
    Q_DECLARE_TR_FUNCTIONS(XmlValidator)

public:

    //Drug XML schemas
    static const char *const Drug;

    //Exchange XML schemas
    static const char *const Import;
    static const char *const Export;

    //Messages XML schemas
    static const char *const Queries;
    static const char *const Reply_List;
    static const char *const Reply_Request;
    static const char *const Notification;
    static const char *const Acks;

    XmlValidator();
    ~XmlValidator();

    bool validate(const QString &xmlFilename, const QString &xsdFilename);
    bool validate(const QByteArray &xmlData, const QString &xsdFilename);

    QtMsgType errorType() const;
    QString errorMessage() const;
    QString errorHtmlMessage() const;

    int errorLine() const;
    int errorColumn() const;

private:
    class ValidatorMessageHandler : public ErrorHandler
    {

    public:
        ValidatorMessageHandler();

        virtual void warning(const SAXParseException& exc) override;
        virtual void error(const SAXParseException& exc) override;
        virtual void fatalError(const SAXParseException& exc) override;
        virtual void resetErrors() override;

        QString description() const;
        QtMsgType type() const;

        int line() const;
        int column() const;

    protected:
        virtual void handleMessage(QtMsgType type, const QString &description, const QUrl &identifier, const SAXParseException &sourceExeption);

    private:
        QtMsgType _type;
        QString _description;
        SAXParseException _exeption;
    };

    void initTempSchemaFolder(const QString &path, const QTemporaryDir &tmpDir) const;

    ValidatorMessageHandler *_msgHandler;
};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi


#endif // XMLVALIDATOR_H
