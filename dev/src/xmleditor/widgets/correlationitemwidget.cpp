#include "correlationitemwidget.h"
#include "ui_correlationitemwidget.h"

#include <limits>

CorrelationItemWidget::CorrelationItemWidget(QWidget *parent) : DomItemWidget(parent), ui(new Ui::CorrelationItemWidget)
{
    ui->setupUi(this);

    ui->valueBox->setMaximum((double)std::numeric_limits<int>::max());
}

CorrelationItemWidget::~CorrelationItemWidget()
{
    delete ui;
}

bool CorrelationItemWidget::setNodeImpl(QDomElement node)
{
    _parser.setNode(node);
    return _parser.isValid();
}

void CorrelationItemWidget::setData()
{
    ui->param1Line->setText(_parser.param1Id());
    ui->param1Line->setEnabled(true);

    ui->param2Line->setText(_parser.param2Id());
    ui->param2Line->setEnabled(true);

    ui->valueBox->setValue(_parser.value());
    ui->valueBox->setEnabled(true);
}

void CorrelationItemWidget::clearData()
{
    ui->param1Line->clear();
    ui->param1Line->setEnabled(false);

    ui->param2Line->clear();
    ui->param2Line->setEnabled(false);

    ui->valueBox->clear();
    ui->valueBox->setEnabled(false);
}
