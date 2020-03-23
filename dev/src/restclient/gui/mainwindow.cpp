#include <QDebug>
#include <QMessageBox>
#include <QNetworkReply>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "listtestwidget.h"
#include "ackrequesttestwidget.h"
#include "notificationtestwidget.h"
#include "restclient/model/querylisttestmessage.h"
#include "restclient/model/replylisttestmessage.h"
#include "restclient/model/queryrequesttestmessage.h"
#include "restclient/model/replyrequesttestmessage.h"
#include "restclient/model/notificationtestmessage.h"
#include "restclient/model/acknotificationtestmessage.h"
#include "restclient/utils/xmltestlibrary.h"

#include "rest/network/networkaccessmanager.h"
#include "rest/network/queryrequestrequest.h"
#include "rest/network/notificationrequest.h"


const char *const MainWindow::_REQUEST_WIDGET_BUTTON_TEXT = QT_TR_NOOP_UTF8("Query Request");
const char *const MainWindow::_NOTIFY_WIDGET_BUTTON_TEXT  = QT_TR_NOOP_UTF8("Notify");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow),
    _listWidget(new ListTestWidget(this)),
    _requestWidget(new AckRequestTestWidget(this)),
    _notifyWidget(new NotificationTestWidget(this))
{
    _ui->setupUi(this);

    _ui->sslErrorWidget->setVisible(false);
    _ui->sslErrorSeparator->setVisible(false);

    initTestWidgets();

    connect(_listWidget,    SIGNAL(started(AbstractTestWidget*)), this, SLOT(testStarted(AbstractTestWidget*)));
    connect(_listWidget,    SIGNAL(stopped(AbstractTestWidget*)), this, SLOT(testStopped(AbstractTestWidget*)));
    connect(_requestWidget, SIGNAL(started(AbstractTestWidget*)), this, SLOT(testStarted(AbstractTestWidget*)));
    connect(_requestWidget, SIGNAL(stopped(AbstractTestWidget*)), this, SLOT(testStopped(AbstractTestWidget*)));
    connect(_notifyWidget,  SIGNAL(started(AbstractTestWidget*)), this, SLOT(testStarted(AbstractTestWidget*)));
    connect(_notifyWidget,  SIGNAL(stopped(AbstractTestWidget*)), this, SLOT(testStopped(AbstractTestWidget*)));

    connect(_listWidget,    SIGNAL(errors(const QString &)), this, SLOT(errors(const QString &)));
    connect(_requestWidget, SIGNAL(errors(const QString &)), this, SLOT(errors(const QString &)));
    connect(_notifyWidget,  SIGNAL(errors(const QString &)), this, SLOT(errors(const QString &)));

    connect(&NetworkAccessManager::getInstance(), SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(sslErros(QNetworkReply*,QList<QSslError>)));

    connect(_ui->resetButton, SIGNAL(clicked()), this, SLOT(reset()));

    connect(dynamic_cast<ListTestWidget *>(_listWidget), SIGNAL(requestSelected(QString)), this, SLOT(requestSelected(QString)));
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::initTestWidgets()
{
    _listWidget->setQueryMessage(new QueryListTestMessage);
    _listWidget->setReplyMessage(new ReplyListTestMessage);

    _requestWidget->setButtonText(tr(_REQUEST_WIDGET_BUTTON_TEXT));
    _notifyWidget->setButtonText(tr(_NOTIFY_WIDGET_BUTTON_TEXT));

    _requestWidget->disable();
    _notifyWidget->disable();

    _ui->listTestWidgetLayout->addWidget(_listWidget);
    _ui->requestTestWidgetsLayout->addWidget(_requestWidget);
    _ui->requestTestWidgetsLayout->addWidget(_notifyWidget);
}

void MainWindow::testStarted(AbstractTestWidget *testWidget)
{
    Q_UNUSED(testWidget);
    _ui->resetButton->setEnabled(false);
}

void MainWindow::testStopped(AbstractTestWidget *testWidget)
{
    if (testWidget != _listWidget)
        testWidget->disable();
    if (testWidget == _requestWidget)
        _notifyWidget->enable();

    _ui->resetButton->setEnabled(true);
}

void MainWindow::errors(const QString &errorString)
{
    Q_UNUSED(errorString);
    _ui->resetButton->setEnabled(true);
}

void MainWindow::sslErros(QNetworkReply *reply, QList<QSslError> errors)
{
    if (errors.count() == 1 && errors.first().error() == QSslError::SelfSignedCertificate) {
        if (QMessageBox::warning(this,
                                 tr("Certificate error"),
                                 errors.first().errorString() + ". " + tr("Would you like to proceed anyway ?"),
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::No) == QMessageBox::Yes) {
            reply->ignoreSslErrors();
            return;
        }
    }

    _ui->sslErrorWidget->setVisible(true);
    _ui->sslErrorSeparator->setVisible(true);

    for (int i = 0; i < errors.count(); ++i)
        _ui->sslErrorWidget->appendPlainText(errors.at(i).errorString() + ".");

    _ui->resetButton->setEnabled(true);
}

void MainWindow::reset()
{
    _requestWidget->disable();
    _notifyWidget->disable();

    _requestWidget->setButtonText(tr(_REQUEST_WIDGET_BUTTON_TEXT));
    _notifyWidget->setButtonText(tr(_NOTIFY_WIDGET_BUTTON_TEXT));

    _listWidget->reset();
    _requestWidget->reset();
    _notifyWidget->reset();

    _ui->sslErrorWidget->clear();
    _ui->sslErrorWidget->setVisible(false);
    _ui->sslErrorSeparator->setVisible(false);
}

void MainWindow::requestSelected(const QString &requestId)
{
    if (requestId.isEmpty() || !XmlTestLibrary::getInstance().isValid(requestId)) {
        _requestWidget->disable();
        _requestWidget->reset();

        _notifyWidget->disable();
        _notifyWidget->reset();

        return;
    }

    _requestWidget->setQueryMessage(new QueryRequestTestMessage(requestId));
    _requestWidget->setReplyMessage(new ReplyRequestTestMessage(requestId));
    _notifyWidget->setQueryMessage(new NotificationTestMessage(requestId));

    _requestWidget->setButtonText(tr(_REQUEST_WIDGET_BUTTON_TEXT) + QString(" (%1)").arg(requestId));
    _notifyWidget->setButtonText(tr(_NOTIFY_WIDGET_BUTTON_TEXT) + QString(" (%1)").arg(requestId));

    _requestWidget->enable();
    _requestWidget->reset();

    _notifyWidget->disable();
    _notifyWidget->reset();
}
