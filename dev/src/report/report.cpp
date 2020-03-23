#include "report.h"
#include "reportdata.h"
#include "identifiers.h"
#include "genericdao.h"
#include "reportsqlitedao.h"
#include "core.h"

#include <QWebPage>
#include <QWebFrame>
#include <QFile>
#include <QTextDocument>
#include <QDebug>
#include <QWebElement>
#include <QTemporaryDir>

QMutex Report::_mutex;

const char *const Report::_JS_REPORT_OBJECT_NAME   = "Report";
const char *const Report::_JS_QUERY_RESOURCE_FILE  = ":/javascript/jquery.js";
const char *const Report::_JS_MOMENTS_WITH_LOCALES = ":/javascript/moment-with-locales.js";

Report::Report(ReportData *data, QObject *parent) : QObject(parent),
    _data(data),
    _page(new QWebPage(this)),
    _error()
{
    //Configure the data
    _data->setParent(this);
    _data->setObjectName("Data");

    //Configure the page
    _page->settings()->setMaximumPagesInCache(0);
    _page->settings()->setObjectCacheCapacities(0, 0, 0);

    //Connect the page's signals
    connect(_page->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(addToJavaScript()));
    connect(_page,              SIGNAL(loadFinished(bool)),              this, SLOT(finishedLoading(bool)));

    //Connect the data's signals
    connect(_data, SIGNAL(valueChanged(QString,QString)), this, SIGNAL(dataChanged()));
    connect(_data, SIGNAL(valuesReset()),                 this, SIGNAL(dataChanged()));
    connect(_data, SIGNAL(imageChanged(QString,QImage)),  this, SIGNAL(dataChanged()));
    connect(_data, SIGNAL(imagesReset()),                 this, SIGNAL(dataChanged()));
    connect(_data, SIGNAL(sectionHidden(QString)),        this, SIGNAL(dataChanged()));
    connect(_data, SIGNAL(sectionShown(QString)),         this, SIGNAL(dataChanged()));
    connect(_data, SIGNAL(sectionsReset()),               this, SIGNAL(dataChanged()));

    connect(_data, SIGNAL(valueChanged(QString,QString)), this, SLOT(valueChanged(QString,QString)));

    //Load the report HTML page
    _page->mainFrame()->load(QUrl::fromLocalFile(getTempPath() + "/" + _data->templateName()));
}

Report::~Report()
{

}

QString Report::toPlainText(const QString &html, bool singleLine)
{
    QTextDocument doc;
    doc.setHtml(html);

    return singleLine ? doc.toPlainText().trimmed().replace(QRegExp("\n+"), " ") : doc.toPlainText().trimmed();
}

ReportData *Report::data() const
{
    return _data;
}

QWebPage *Report::page() const
{
    return _page;
}

QString Report::templateName() const
{
    return data()->templateName();
}

QString Report::language() const
{
    return data()->language();
}

QString Report::title() const
{
    return data()->title();
}

QString Report::reference() const
{
    return data()->reference();
}

QDateTime Report::creationDate() const
{
    return data()->creationDate();
}

QDateTime Report::publicationDate() const
{
    return data()->publicationDate();
}

int Report::reportId() const
{
    return data()->reportId();
}

int Report::patientId() const
{
    return data()->patientId();
}

int Report::analystId() const
{
    return data()->analystId();
}

int Report::recipientId() const
{
    return data()->recipientId();
}

int Report::instituteId() const
{
    return data()->instituteId();
}

int Report::treatmentId() const
{
    return data()->treatmentId();
}

bool Report::save()
{
    QScopedPointer<GenericDAO<Report>> reportDAO(new ReportSQLiteDAO());

    if (!reportDAO->save(this)) {
        setError(reportDAO->error());
        return false;
    }

    return true;
}

bool Report::destroy()
{
    QScopedPointer<GenericDAO<Report>> reportDAO(new ReportSQLiteDAO());

    if (!reportDAO->remove(this)) {
        setError(reportDAO->error());
        return false;
    }

    return true;
}

bool Report::publish(const QDateTime &date)
{
    if (publicationDate().isValid()) {
        setError(tr("The report was already published on the %1.").arg(publicationDate().toString(Qt::SystemLocaleLongDate)));
        return false;
    }
    if (!date.isValid()) {
        setError(tr("The publication date is invalid."));
        return false;
    }

    _data->setPublicationDate(date);

    if (!save()) {
        _data->unsetValue(ID::Publication);
        return false;
    }

    blockInputs();
    return true;
}

QMargins Report::margins() const
{
    // ToDo ///////////////////////////////////////////////////////////////////////////////////////
    // Get the margin from javascript, and add setter.
    // ////////////////////////////////////////////////////////////////////////////////////////////

    return QMargins(10, 20, 10, 20);
}

QString Report::error() const
{
    return _error;
}

void Report::debug(const QString &message)
{
    qDebug() << "Javascrip: " << message;
}

void Report::finishedLoading()
{

}

void Report::finishedInitializing()
{

}

void Report::setError(const QString &error)
{
    _error = error;
}

void Report::valueChanged(const QString &id, const QString &value)
{
    if (id == ID::Title)
        emit titleChanged(value);
    if (id == ID::Reference)
        emit referenceChanged(value);
    if (id == ID::Language)
        emit languageChanged(value);
    if (id == ID::Creation)
        emit creationDateChanged(QDateTime::fromString(value, Qt::ISODate));
    if (id == ID::Publication)
        emit publicationDateChanged(QDateTime::fromString(value, Qt::ISODate));
}

QString Report::jQuery()
{
    static QString jQuery;

    if (jQuery.isEmpty()) {
        QFile jQueryFile(_JS_QUERY_RESOURCE_FILE);
        if (jQueryFile.open(QIODevice::ReadOnly))
            jQuery = jQueryFile.readAll();
        jQueryFile.close();
    }

    return jQuery;
}

QString Report::moment()
{
    static QString moment;

    if (moment.isEmpty()) {
        QFile momentFile(_JS_MOMENTS_WITH_LOCALES);
        if (momentFile.open(QIODevice::ReadOnly))
            moment = momentFile.readAll();
        momentFile.close();
    }

    return moment;
}

QString Report::getTempPath()
{
    static QString tempPath;

    if (tempPath.isEmpty()) {
        QMutexLocker locker(&_mutex);

        if (tempPath.isEmpty()) {
            static QTemporaryDir tempDir;
            tempPath = tempDir.path();

            QDir dir(":/templates");
            QStringList templates = dir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

            for (int i = 0; i < templates.count(); ++i)
                QFile::copy(dir.path() + "/" + templates.at(i), tempPath + "/" + templates.at(i));
        }
    }

    return tempPath;
}

void Report::blockInputs()
{
    //Prevent edition
    QWebElementCollection editable = _page->mainFrame()->findAllElements("[contenteditable=\"true\"]");
    for (int i = 0; i < editable.count(); ++i)
        editable[i].setAttribute("contenteditable", "false");

    //Prevent clicks
    QWebElementCollection clickable = _page->mainFrame()->findAllElements("*");
    for (int i = 0; i < clickable.count(); ++i)
        clickable[i].setStyleProperty("pointer-events", "none");
}

void Report::addToJavaScript()
{
    _page->mainFrame()->addToJavaScriptWindowObject(_JS_REPORT_OBJECT_NAME, this);
}

void Report::finishedLoading(bool result)
{
    if (!result)
        return;

    finishedLoading();

    _page->mainFrame()->evaluateJavaScript(jQuery());
    _page->mainFrame()->evaluateJavaScript(moment());
    _page->mainFrame()->evaluateJavaScript("init()");

    if (publicationDate().isValid())
        blockInputs();

    finishedInitializing();
}
