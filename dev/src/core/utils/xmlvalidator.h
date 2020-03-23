#ifndef XMLVALIDATOR_H
#define XMLVALIDATOR_H

#include <QCoreApplication>
#include <QXmlSchemaValidator>
#include <QAbstractMessageHandler>
#include <QSourceLocation>

namespace ezechiel {
namespace core {


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
    class ValidatorMessageHandler : public QAbstractMessageHandler
    {

    public:
        ValidatorMessageHandler();

        QString description() const;
        QtMsgType type() const;

        int line() const;
        int column() const;

    protected:
        virtual void handleMessage(QtMsgType type, const QString &description, const QUrl &identifier, const QSourceLocation &sourceLocation);

    private:
        QtMsgType _type;
        QString _description;
        QSourceLocation _location;
    };

    QXmlSchema initSchema(const QString &path) const;

    ValidatorMessageHandler *_msgHandler;
};

} // namespace core
} // namespace ezechiel


#endif // XMLVALIDATOR_H
