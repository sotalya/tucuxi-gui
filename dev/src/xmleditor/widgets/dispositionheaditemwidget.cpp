#include "dispositionheaditemwidget.h"
#include "ui_dispositionheaditemwidget.h"

DispositionHeadItemWidget::DispositionHeadItemWidget(QWidget *parent) : DomItemWidget(parent), ui(new Ui::DispositionHeadItemWidget)
{
    ui->setupUi(this);
}

DispositionHeadItemWidget::~DispositionHeadItemWidget()
{
    delete ui;
}

bool DispositionHeadItemWidget::setNodeImpl(QDomElement node)
{
    _parser.setNode(node);
    return _parser.isValid();
}

void DispositionHeadItemWidget::setData()
{
    ui->idLine->setText(_parser.id());
    ui->idLine->setEnabled(true);

    ui->nameLine->setText(_parser.translatedName());
    ui->nameLine->setEnabled(true);
}

void DispositionHeadItemWidget::clearData()
{
    ui->idLine->clear();
    ui->idLine->setEnabled(false);

    ui->nameLine->clear();
    ui->nameLine->setEnabled(false);
}
