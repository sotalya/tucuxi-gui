#include "drugheaditemwidget.h"
#include "ui_drugheaditemwidget.h"

DrugHeadItemWidget::DrugHeadItemWidget(QWidget *parent) : DomItemWidget(parent), ui(new Ui::DrugHeadItemWidget)
{
    ui->setupUi(this);
}

DrugHeadItemWidget::~DrugHeadItemWidget()
{
    delete ui;
}

bool DrugHeadItemWidget::setNodeImpl(QDomElement node)
{
    _parser.setNode(node);
    return _parser.isValid();
}

void DrugHeadItemWidget::setData()
{
    ui->idLine->setText(_parser.id());
    ui->idLine->setEnabled(true);

    ui->atcLine->setText(_parser.atc());
    ui->atcLine->setEnabled(true);

    ui->nameLine->setText(_parser.translatedName());
    ui->nameLine->setEnabled(true);

    ui->shortDescText->setPlainText(_parser.translatedShortDesc());
    ui->shortDescText->setEnabled(true);

    ui->longDescText->setPlainText(_parser.translatedLongDesc());
    ui->longDescText->setEnabled(true);

    ui->brandList->clear();
    ui->brandList->setEnabled(true);

    foreach(QString brand, _parser.brands())
        ui->brandList->addItem(new QListWidgetItem(brand));
}

void DrugHeadItemWidget::clearData()
{
    ui->idLine->clear();
    ui->idLine->setEnabled(false);

    ui->atcLine->clear();
    ui->atcLine->setEnabled(false);

    ui->nameLine->clear();
    ui->nameLine->setEnabled(false);

    ui->shortDescText->clear();
    ui->shortDescText->setEnabled(false);

    ui->longDescText->clear();
    ui->longDescText->setEnabled(false);

    ui->brandList->clear();
    ui->brandList->setEnabled(false);
}
