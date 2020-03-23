#ifndef REPORT_H
#define REPORT_H

class ReportData;
class QWebPage;
class QString;

#include <QObject>
#include <QString>
#include <QImage>
#include <QDateTime>
#include <QMutex>

class Report : public QObject
{
    Q_OBJECT

public:
    Report(ReportData *data, QObject *parent = 0);
    virtual ~Report() = 0;

    //Interface exposed to Qt
    static QString toPlainText(const QString &html, bool singleLine = false);

    ReportData *data() const;
    QWebPage *page() const;

    QString templateName() const;
    QString language() const;

    QString title() const;
    QString reference() const;

    QDateTime creationDate() const;
    QDateTime publicationDate() const;

    int reportId() const;
    int patientId() const;
    int analystId() const;
    int recipientId() const;
    int instituteId() const;
    int treatmentId() const;

    bool save();
    bool destroy();
    bool publish(const QDateTime &date = QDateTime::currentDateTime());

    QMargins margins() const;
    QString error() const;

    //Interface exposed to JS
    Q_INVOKABLE virtual void debug(const QString &message);

signals:
    void dataChanged();

    void titleChanged(const QString &title);
    void referenceChanged(const QString &reference);
    void languageChanged(const QString &language);

    void creationDateChanged(const QDateTime &creationDate);
    void publicationDateChanged(const QDateTime &publicationDate);

protected:
    virtual void finishedLoading();
    virtual void finishedInitializing();

    void setError(const QString &error);

protected slots:
    virtual void valueChanged(const QString &id, const QString &value);

private:
    static QString jQuery();
    static QString moment();

    static QMutex _mutex;

    static const char *const _JS_REPORT_OBJECT_NAME;
    static const char *const _JS_QUERY_RESOURCE_FILE;
    static const char *const _JS_MOMENTS_WITH_LOCALES;

    QString getTempPath();
    void blockInputs();

    ReportData *_data;
    QWebPage *_page;

    QString _error;

private slots:
    void addToJavaScript();
    void finishedLoading(bool result);
};

Q_DECLARE_METATYPE(Report *)

#endif // REPORT_H
