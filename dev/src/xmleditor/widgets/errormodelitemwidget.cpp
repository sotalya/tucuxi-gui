#include "errormodelitemwidget.h"
#include "ui_errormodelitemwidget.h"

#include <limits>

ErrorModelItemWidget::ErrorModelItemWidget(QWidget *parent) : DomItemWidget(parent), ui(new Ui::ErrorModelItemWidget)
{
    ui->setupUi(this);

    ui->additiveBox->setMaximum((double)std::numeric_limits<int>::max());
    ui->proportionalBox->setMaximum((double)std::numeric_limits<int>::max());
}

ErrorModelItemWidget::~ErrorModelItemWidget()
{
    delete ui;
}

bool ErrorModelItemWidget::setNodeImpl(QDomElement node)
{
    _parser.setNode(node);
    return _parser.isValid();
}

void ErrorModelItemWidget::setData()
{
    ui->unitBox->setCurrentUnit(_parser.additiveUnit());
    ui->unitBox->setEnabled(true);

    ui->additiveBox->setValue(_parser.additive());
    ui->additiveBox->setEnabled(true);

    ui->proportionalBox->setValue(_parser.proportional());
    ui->proportionalBox->setEnabled(true);
}

void ErrorModelItemWidget::clearData()
{
    ui->unitBox->setCurrentIndex(-1);
    ui->unitBox->setEnabled(false);

    ui->additiveBox->clear();
    ui->additiveBox->setEnabled(false);

    ui->proportionalBox->clear();
    ui->proportionalBox->setEnabled(false);
}
