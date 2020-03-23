#include "parameteritemwidget.h"
#include "ui_parameteritemwidget.h"

#include <limits>

ParameterItemWidget::ParameterItemWidget(QWidget *parent) : DomItemWidget(parent), ui(new Ui::ParameterItemWidget)
{
    ui->setupUi(this);

    ui->valueBox->setMaximum((double)std::numeric_limits<int>::max());
    ui->additiveBox->setMaximum((double)std::numeric_limits<int>::max());
    ui->proportionalBox->setMaximum((double)std::numeric_limits<int>::max());
    ui->minimumBox->setMaximum((double)std::numeric_limits<int>::max());
    ui->maximumBox->setMaximum((double)std::numeric_limits<int>::max());
    ui->stepBox->setMaximum((double)std::numeric_limits<int>::max());
}

ParameterItemWidget::~ParameterItemWidget()
{
    delete ui;
}

bool ParameterItemWidget::setNodeImpl(QDomElement node)
{
    _parser.setNode(node);
    return _parser.isValid();
}

void ParameterItemWidget::setData()
{
    ui->idLine->setText(_parser.parameterId());
    ui->idLine->setEnabled(true);

    ui->unitBox->setCurrentUnit(_parser.unit());
    ui->unitBox->setEnabled(true);

    ui->valueBox->setValue(_parser.value());
    ui->valueBox->setEnabled(true);

    ui->additiveBox->setValue(_parser.bsvAdditive());
    ui->additiveBox->setEnabled(true);

    ui->proportionalBox->setValue(_parser.bsvProportional());
    ui->proportionalBox->setEnabled(true);

    ui->minimumBox->setValue(_parser.minimum());
    ui->minimumBox->setEnabled(true);

    ui->maximumBox->setValue(_parser.maximum());
    ui->maximumBox->setEnabled(true);

    ui->stepBox->setValue(_parser.step());
    ui->stepBox->setEnabled(true);
}

void ParameterItemWidget::clearData()
{
    ui->idLine->clear();
    ui->idLine->setEnabled(false);

    ui->unitBox->setCurrentIndex(-1);
    ui->unitBox->setEnabled(false);

    ui->valueBox->clear();
    ui->valueBox->setEnabled(false);

    ui->additiveBox->clear();
    ui->additiveBox->setEnabled(false);

    ui->proportionalBox->clear();
    ui->proportionalBox->setEnabled(false);

    ui->minimumBox->clear();
    ui->minimumBox->setEnabled(false);

    ui->maximumBox->clear();
    ui->maximumBox->setEnabled(false);

    ui->stepBox->clear();
    ui->stepBox->setEnabled(false);
}
