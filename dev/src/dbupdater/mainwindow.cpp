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
    evaluateButton->setEnabled(false);

    connect(HANDLER, SIGNAL(publishResult(bool, QStringList)), this, SLOT(evaluationFinished(bool, QStringList)));

    updateButton = new QPushButton("Update Database");
    connect(updateButton, SIGNAL(clicked()), HANDLER, SLOT(updateDB()));
    connect(HANDLER, SIGNAL(publishUpdateStatus(bool)), this, SLOT(updateStatus(bool)));

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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Database"),
                                                    QCoreApplication::applicationDirPath(),
                                                     tr("SQL database (*.db)"));
    if(fileName != "")
    {
        ui->path->setText(fileName);
        HANDLER->setDBPath(fileName);
        evaluateButton->setEnabled(true);
        ui->browse->setEnabled(false);
    }
}

void MainWindow::evaluationFinished(bool result, QStringList patches)
{
    QLayoutItem* item;
    while((item = ui->vLayout->takeAt(0)) != 0)
    {
        delete item->widget();
        delete item;
    }
    if(result)
    {
        if(patches.isEmpty())
        {
            ui->vLayout->addWidget(new QLabel("The Database is up-to-date. "));
        }
        else
        {
            ui->vLayout->addWidget(new QLabel("The Database needs to be updated. "));
            ui->vLayout->addWidget(updateButton);
        }
    }
    else
    {
        ui->vLayout->addWidget(new QLabel("A problem occured. Either the database was unreachable, or not EzeCHieL compatible. Please try again, and make sure you select the correct database. "));
    }
}

void MainWindow::updateStatus(bool result)
{
    QLayoutItem* item;
    while((item = ui->vLayout->takeAt(0)) != 0)
    {
        delete item->widget();
        delete item;
    }
    if(result)
    {
        ui->vLayout->addWidget(new QLabel("The Database was updated successfully. "));
    }
    else
    {
        ui->vLayout->addWidget(new QLabel("The Database couldn't be updated. The following problem occured: "));
        ui->vLayout->addWidget(new QLabel(HANDLER->getErrMsg()));
    }
}
