#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dbhandler.h"
#include <QFileDialog>

#define HANDLER DBHandler::instance()

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->browse, SIGNAL(clicked()),this,SLOT(browseClicked()));

    evaluateButton = new QPushButton("Evaluate Database");
    connect(evaluateButton, SIGNAL(clicked()), HANDLER, SLOT(evaluate()));
    connect(evaluateButton, SIGNAL(clicked()), this, SLOT(evaluateClicked()));
    evaluateButton->setEnabled(false);

    connect(HANDLER, SIGNAL(publishResult(bool)), this, SLOT(evaluationFinished(bool)));

    anonymizeButton = new QPushButton("Anonymize Database");
    connect(anonymizeButton, SIGNAL(clicked()), HANDLER, SLOT(anonymizeDB()));
    connect(anonymizeButton, SIGNAL(clicked()), this, SLOT(anonymizeClicked()));
    connect(HANDLER, SIGNAL(publishAnonymizeStatus(bool)), this, SLOT(anonymisationStatus(bool)));

    ui->vLayout->addStretch();
    ui->vLayout->addWidget(evaluateButton);
    ui->vLayout->addStretch();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::browseClicked()
{
    QString referenceDb = QFileDialog::getOpenFileName(this, tr("Open Database"), "/home", tr("SQL database (*.db)"));

    if(referenceDb != "") {
        ui->path->setText(referenceDb);
        HANDLER->setDBPath(referenceDb);

        QString anonymizedDb(QDir(QCoreApplication::applicationDirPath()).filePath("anonymizeddb.db"));
        ui->anonymizedName->setText(anonymizedDb);
        HANDLER->setAnonDBName(anonymizedDb);

        evaluateButton->setEnabled(true);
        ui->browse->setEnabled(false);
    }
}

void MainWindow::evaluateClicked()
{
    ui->anonymizedName->setEnabled(false);
}

void MainWindow::anonymizeClicked()
{

}

void MainWindow::evaluationFinished(bool result)
{
    QLayoutItem* item;
    while((item = ui->vLayout->takeAt(0)) != 0)
    {
        delete item->widget();
        delete item;
    }
    if(result)
    {
        ui->vLayout->addWidget(new QLabel("The Database was cleared to be anonymized. "));
        ui->vLayout->addWidget(anonymizeButton);
    }
    else
    {
        ui->vLayout->addWidget(new QLabel("A problem occured. Either the database was unreachable, or not EzeCHieL compatible. Please try again, and make sure you select the correct database. "));
    }
}

void MainWindow::anonymisationStatus(bool result)
{
    QLayoutItem* item;
    while((item = ui->vLayout->takeAt(0)) != 0)
    {
        delete item->widget();
        delete item;
    }
    if(result)
    {
        ui->vLayout->addWidget(new QLabel("The Database was anonymized successfully. "));
    }
    else
    {
        ui->vLayout->addWidget(new QLabel("The Database couldn't be anonymized. The following problem occured: "));
        ui->vLayout->addWidget(new QLabel(HANDLER->getErrMsg()));
    }
}
