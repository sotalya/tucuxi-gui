//@@license@@

#ifndef XMLTESTLIBRARY_H
#define XMLTESTLIBRARY_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QDomDocument>

class XmlTestLibrary : public QObject
{
    Q_OBJECT

public:
    static XmlTestLibrary &getInstance();

    QDomDocument queryList() const;
    QDomDocument replyList() const;
    QDomDocument queryRequest(const QString &requestId) const;
    QDomDocument replyRequest(const QString &requestId) const;
    QDomDocument notification(const QString &requestId) const;

    bool isValid(const QString &requestId) const;

private:
    explicit XmlTestLibrary(QObject *parent = nullptr);
    virtual ~XmlTestLibrary() Q_DECL_OVERRIDE;

    void loadTestFile(const QString &filename, const QString &schema, QDomDocument &doc);
    void loadTestFile(const QString &filename, const QString &schema, QMap<QString, QDomDocument> &map);

    QString _queryListTestFile;
    QString _replyListTestFile;
    QStringList _queryRequestsTestFiles;
    QStringList _replyRequestsTestFiles;
    QStringList _notificationsTestFiles;

    QDomDocument _queryList;
    QDomDocument _replyList;
    QMap<QString, QDomDocument> _queryRequests;
    QMap<QString, QDomDocument> _replyRequests;
    QMap<QString, QDomDocument> _notifications;

    QStringList _requestIds;
};

#endif // XMLTESTLIBRARY_H
