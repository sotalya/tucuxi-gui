//@@license@@

#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QDate>

#include "listtestwidget.h"
#include "ui_listtestwidget.h"
#include "restclient/model/replylisttestmessage.h"
#include "restclient/utils/xmltestlibrary.h"
#include "core/utils/logging.h"
#include "errors_restclient.h"

#include "rest/network/querylistrequest.h"
#include "rest/network/acklistrequest.h"
#include "rest/model/acklistxmlmessage.h"


ListTestWidget::ListTestWidget(QWidget *parent) :
    AbstractTestWidget(new QueryListRequest, parent),
    _ui(new Ui::ListTestWidget),
    _model(new QStandardItemModel(this)),
    _selector(new QItemSelectionModel(_model, this))
{
    _ui->setupUi(this);

    _ui->textArea->hide();
    _ui->listWidget->setModel(_model);
    _ui->listWidget->setSelectionModel(_selector);

    _model->setHorizontalHeaderLabels(QStringList()
                                      << "ID"
                                      << "State"
                                      << "Firstname"
                                      << "Lastname"
                                      << "Birth Date"
                                      << "Gender"
                                      << "Institute");

    setAcknowledgement(new AckListRequest, new AckListXmlMessage);

    connect(_ui->pushButton, SIGNAL(clicked()), this, SLOT(start()));
    connect(_selector, SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(selectionChanged(QModelIndex,QModelIndex)));
}

ListTestWidget::~ListTestWidget()
{
    delete _ui;
}

void ListTestWidget::enable()
{
    _ui->listWidget->setEnabled(true);
    AbstractTestWidget::enable();
}

void ListTestWidget::disable()
{
    _ui->listWidget->setEnabled(false);
    AbstractTestWidget::disable();
}

void ListTestWidget::reset()
{
    _model->removeRows(0, _model->rowCount());
    AbstractTestWidget::reset();
}

QPushButton *ListTestWidget::pushButton()
{
    return _ui->pushButton;
}

QProgressBar *ListTestWidget::progressBar()
{
    return _ui->progressBar;
}

QPlainTextEdit *ListTestWidget::textArea()
{
    return _ui->textArea;
}

void ListTestWidget::displayError(const QString &errorMsg)
{
    _ui->listWidget->setVisible(false);
    _ui->textArea->setVisible(true);
    _ui->textArea->setPlainText(tr("FAILURE: ") + errorMsg);
}

void ListTestWidget::displaySuccess()
{
    Q_ASSERT(_reply);

    QString replyError;

    _ui->listWidget->setVisible(true);
    _ui->textArea->setVisible(false);

    _model->removeRows(0, _model->rowCount());

    QString response = QString(_reply.data()->data());
    displayError(response);
    if (!_reply->isValid())
        replyError = _reply->errorString();

    QDomDocument doc;
    QString qxmlError;
    int errline = -1;
    int columnline = -1;
    if (!doc.setContent(response,false, &qxmlError, &errline, &columnline)) {
        displayError(tr("QXmlError parsing DOM with error: %1, at line: %2, at column: %3, data: %4").arg(qxmlError).arg(errline).arg(columnline).arg(response));
        return;
    }

    parseRequests(doc);
    sendAck(doc, "ack_list", "AA");

    if (_model->rowCount() == 0)
        displayError(replyError.isEmpty() ? tr("Could not find any request in the 'REPLY_LIST' message") : replyError);
}

void ListTestWidget::parseRequests(const QDomDocument &doc)
{
    EXLOG(QtDebugMsg, ezechiel::restclient::NOEZERROR, tr("Parsing requests from doc: %1").arg(doc.toString()));
    QDomElement requestNode = doc.documentElement().firstChildElement("request");
    while (!requestNode.isNull()) {
        QString id        = requestNode.firstChildElement("requestId").firstChild().toText().data();
        QString state     = requestNode.firstChildElement("requestState").firstChild().toText().data();
        QString firstName = requestNode.firstChildElement("patient").firstChildElement("name").firstChildElement("firstName").firstChild().toText().data();
        QString lastName  = requestNode.firstChildElement("patient").firstChildElement("name").firstChildElement("lastName").firstChild().toText().data();
        QString birthDate = requestNode.firstChildElement("patient").firstChildElement("birthdate").firstChild().toText().data();
        QString gender    = requestNode.firstChildElement("patient").firstChildElement("gender").firstChild().toText().data();
        QString service   = requestNode.firstChildElement("patient").firstChildElement("institute").firstChildElement("name").firstChild().toText().data();

        QList<QStandardItem *> row;
        QBrush brush(id.isEmpty() || !XmlTestLibrary::getInstance().isValid(id) ? Qt::darkRed : Qt::black);

        QStandardItem *idItem = new QStandardItem(id);
        idItem->setEditable(false);
        idItem->setForeground(brush);
        QStandardItem *stateItem = new QStandardItem(state);
        stateItem->setEditable(false);
        stateItem->setForeground(brush);
        QStandardItem *firstNameItem = new QStandardItem(firstName);
        firstNameItem->setEditable(false);
        firstNameItem->setForeground(brush);
        QStandardItem *lastNameItem = new QStandardItem(lastName);
        lastNameItem->setEditable(false);
        lastNameItem->setForeground(brush);
        QStandardItem *birtDateItem = new QStandardItem(QDate::fromString(birthDate, Qt::ISODate).toString(Qt::SystemLocaleShortDate));
        birtDateItem->setEditable(false);
        birtDateItem->setForeground(brush);
        QStandardItem *genderItem = new QStandardItem(gender);
        genderItem->setEditable(false);
        genderItem->setForeground(brush);
        QStandardItem *serviceItem = new QStandardItem(service);
        serviceItem->setEditable(false);
        serviceItem->setForeground(brush);

        row << idItem << stateItem << firstNameItem << lastNameItem << birtDateItem << genderItem << serviceItem;
        _model->appendRow(row);

        _ui->listWidget->resizeColumnsToContents();

        EXLOG(QtDebugMsg, ezechiel::restclient::NOEZERROR, tr("...request parsed..."));
        requestNode = requestNode.nextSiblingElement("request");
    }
}

void ListTestWidget::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);

    if (!current.isValid())
        return;

    QString requestId = _model->item(current.row(), 0)->data(Qt::DisplayRole).toString();
    emit requestSelected(requestId);
}
