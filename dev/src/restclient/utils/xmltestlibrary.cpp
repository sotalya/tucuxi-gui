//@@license@@

#include "restclient/utils/xmltestlibrary.h"

#include "core/utils/xmlvalidator.h"

#include <QFile>

XmlTestLibrary &XmlTestLibrary::getInstance()
{
    static XmlTestLibrary xtl;
    return xtl;
}

QDomDocument XmlTestLibrary::queryList() const
{
    return _queryList;
}

QDomDocument XmlTestLibrary::replyList() const
{
    return _replyList;
}

QDomDocument XmlTestLibrary::queryRequest(const QString &requestId) const
{
    Q_ASSERT(_queryRequests.contains(requestId));
    return _queryRequests.value(requestId);
}

QDomDocument XmlTestLibrary::replyRequest(const QString &requestId) const
{
    Q_ASSERT(_replyRequests.contains(requestId));
    return _replyRequests.value(requestId);
}

QDomDocument XmlTestLibrary::notification(const QString &requestId) const
{
    Q_ASSERT(_notifications.contains(requestId));
    return _notifications.value(requestId);
}

bool XmlTestLibrary::isValid(const QString &requestId) const
{
    return _requestIds.contains(requestId);
}

XmlTestLibrary::XmlTestLibrary(QObject *parent) :
    QObject(parent),
    _queryListTestFile(),
    _replyListTestFile(),
    _queryRequestsTestFiles(),
    _replyRequestsTestFiles(),
    _notificationsTestFiles(),
    _queryList(),
    _replyList(),
    _queryRequests(),
    _replyRequests(),
    _notifications(),
    _requestIds()
{
    //The list files
    _queryListTestFile = ":/test/xml/query_list.xml";
    _replyListTestFile = ":/test/xml/reply_list.xml";

    //The requests files
    _queryRequestsTestFiles << ":/test/xml/000000001/query_request.xml";
    _replyRequestsTestFiles << ":/test/xml/000000001/reply_request.xml";
    _notificationsTestFiles << ":/test/xml/000000001/notification.xml";
    _queryRequestsTestFiles << ":/test/xml/123456789/query_request.xml";
    _replyRequestsTestFiles << ":/test/xml/123456789/reply_request.xml";
    _notificationsTestFiles << ":/test/xml/123456789/notification.xml";
    _queryRequestsTestFiles << ":/test/xml/1509113188/query_request.xml";
    _replyRequestsTestFiles << ":/test/xml/1509113188/reply_request.xml";
    _notificationsTestFiles << ":/test/xml/1509113188/notification.xml";
    _queryRequestsTestFiles << ":/test/xml/1509113194/query_request.xml";
    _replyRequestsTestFiles << ":/test/xml/1509113194/reply_request.xml";
    _notificationsTestFiles << ":/test/xml/1509113194/notification.xml";
    _queryRequestsTestFiles << ":/test/xml/1509113611/query_request.xml";
    _replyRequestsTestFiles << ":/test/xml/1509113611/reply_request.xml";
    _notificationsTestFiles << ":/test/xml/1509113611/notification.xml";
    _queryRequestsTestFiles << ":/test/xml/169999134/query_request.xml";
    _replyRequestsTestFiles << ":/test/xml/169999134/reply_request.xml";
    _notificationsTestFiles << ":/test/xml/169999134/notification.xml";
    _queryRequestsTestFiles << ":/test/xml/169999135/query_request.xml";
    _replyRequestsTestFiles << ":/test/xml/169999135/reply_request.xml";
    _notificationsTestFiles << ":/test/xml/169999135/notification.xml";
    _queryRequestsTestFiles << ":/test/xml/169999136/query_request.xml";
    _replyRequestsTestFiles << ":/test/xml/169999136/reply_request.xml";
    _notificationsTestFiles << ":/test/xml/169999136/notification.xml";
    _queryRequestsTestFiles << ":/test/xml/169999137/query_request.xml";
    _replyRequestsTestFiles << ":/test/xml/169999137/reply_request.xml";
    _notificationsTestFiles << ":/test/xml/169999137/notification.xml";
    _queryRequestsTestFiles << ":/test/xml/169999138/query_request.xml";
    _replyRequestsTestFiles << ":/test/xml/169999138/reply_request.xml";
    _notificationsTestFiles << ":/test/xml/169999138/notification.xml";
    _queryRequestsTestFiles << ":/test/xml/169999139/query_request.xml";
    _replyRequestsTestFiles << ":/test/xml/169999139/reply_request.xml";
    _notificationsTestFiles << ":/test/xml/169999139/notification.xml";
    _queryRequestsTestFiles << ":/test/xml/169999140/query_request.xml";
    _replyRequestsTestFiles << ":/test/xml/169999140/reply_request.xml";
    _notificationsTestFiles << ":/test/xml/169999140/notification.xml";
    _queryRequestsTestFiles << ":/test/xml/169999141/query_request.xml";
    _replyRequestsTestFiles << ":/test/xml/169999141/reply_request.xml";
    _notificationsTestFiles << ":/test/xml/169999141/notification.xml";
    _queryRequestsTestFiles << ":/test/xml/169999142/query_request.xml";
    _replyRequestsTestFiles << ":/test/xml/169999142/reply_request.xml";
    _notificationsTestFiles << ":/test/xml/169999142/notification.xml";
    _queryRequestsTestFiles << ":/test/xml/169999143/query_request.xml";
    _replyRequestsTestFiles << ":/test/xml/169999143/reply_request.xml";
    _notificationsTestFiles << ":/test/xml/169999143/notification.xml";

    //Load the files
    loadTestFile(_queryListTestFile, Tucuxi::GuiCore::XmlValidator::Queries, _queryList);
    loadTestFile(_replyListTestFile, Tucuxi::GuiCore::XmlValidator::Reply_List, _replyList);
    for (int i = 0; i < _queryRequestsTestFiles.size(); ++i)
        loadTestFile(_queryRequestsTestFiles.at(i), Tucuxi::GuiCore::XmlValidator::Queries, _queryRequests);
    for (int i = 0; i < _replyRequestsTestFiles.size(); ++i)
        loadTestFile(_replyRequestsTestFiles.at(i), Tucuxi::GuiCore::XmlValidator::Reply_Request, _replyRequests);
    for (int i = 0; i < _notificationsTestFiles.size(); ++i)
        loadTestFile(_notificationsTestFiles.at(i), Tucuxi::GuiCore::XmlValidator::Notification, _notifications);
}

XmlTestLibrary::~XmlTestLibrary()
{

}

void XmlTestLibrary::loadTestFile(const QString &filename, const QString &schema, QDomDocument &doc)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qFatal(qPrintable(QString("Could not open the XML test file '%1', aborting...").arg(filename)));

    Tucuxi::GuiCore::XmlValidator validator;
    if (!validator.validate(filename, schema))
        qFatal(qPrintable(QString("Could not validate the XML test file '%1' (%2), aborting...").arg(filename, validator.errorMessage())));

    if (!doc.setContent(&file))
        qFatal(qPrintable(QString("Could set the document for the XML test file '%1', aborting...").arg(filename)));
}

void XmlTestLibrary::loadTestFile(const QString &filename, const QString &schema, QMap<QString, QDomDocument> &map)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qFatal(qPrintable(QString("Could not open the XML test file '%1', aborting...").arg(filename)));

    Tucuxi::GuiCore::XmlValidator validator;
    if (!validator.validate(filename, schema))
        qFatal(qPrintable(QString("Could not validate the XML test file '%1' (%2), aborting...").arg(filename, validator.errorMessage())));

    QDomDocument doc;
    if (!doc.setContent(&file))
        qFatal(qPrintable(QString("Could set the document for the XML test file '%1', aborting...").arg(filename)));

    QString requestId, msgType  = doc.documentElement().attributeNode("type").value();
    if (msgType == "query_request")
        requestId = doc.documentElement().firstChildElement("requestId").firstChild().toText().data();
    else
        requestId = doc.documentElement().firstChildElement().firstChildElement("requestId").firstChild().toText().data();

    if (requestId.isEmpty())
        qFatal(qPrintable(QString("Could find the request ID of the XML test file '%1', aborting...").arg(filename)));

    if (!_requestIds.contains(requestId))
        _requestIds.append(requestId);

    map.insert(requestId, doc);
}
