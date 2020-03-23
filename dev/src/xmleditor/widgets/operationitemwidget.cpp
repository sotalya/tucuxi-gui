#include "operationitemwidget.h"
#include "ui_operationitemwidget.h"

OperationItemWidget::OperationItemWidget(QWidget *parent) : DomItemWidget(parent), ui(new Ui::OperationItemWidget)
{
    ui->setupUi(this);
}

OperationItemWidget::~OperationItemWidget()
{
    delete ui;
}

bool OperationItemWidget::setNodeImpl(QDomElement node)
{
    _parser.setNode(node);
    return _parser.isValid();
}

void OperationItemWidget::setData()
{
    ui->parameterLine->setText(_parser.parameter());
    ui->parameterLine->setEnabled(true);

    ui->formulaText->setPlainText(_parser.formula().trimmed());
    ui->formulaText->setEnabled(true);
}

void OperationItemWidget::clearData()
{
    ui->parameterLine->clear();
    ui->parameterLine->setEnabled(false);

    ui->formulaText->clear();
    ui->formulaText->setEnabled(false);
}
