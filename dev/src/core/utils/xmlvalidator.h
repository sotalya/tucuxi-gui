//@@license@@

#ifndef XMLVALIDATOR_H
#define XMLVALIDATOR_H

#include <QCoreApplication>
//#include <QXmlSchemaValidator>
//#include <QAbstractMessageHandler>
//#include <QSourceLocation>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/util/PlatformUtils.hpp>

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
    bool validate(QIODevice *xmlDevice, const QString &xsdFilename);
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

    //QXmlSchema initSchema(const QString &path) const;

    ValidatorMessageHandler *_msgHandler;
};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi


#endif // XMLVALIDATOR_H
