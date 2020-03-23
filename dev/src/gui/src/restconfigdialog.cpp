#include "restconfigdialog.h"
#include "ui_restconfigdialog.h"

RestConfigDialog::RestConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RestConfigDialog)
{
    ui->setupUi(this);
}

RestConfigDialog::~RestConfigDialog()
{
    delete ui;
}

QString RestConfigDialog::getIp() const
{
    return ui->ip->text();
}

QString RestConfigDialog::getPort() const
{
    return ui->port->text();
}

QString RestConfigDialog::getScheme() const
{
    return ui->scheme->text();
}
