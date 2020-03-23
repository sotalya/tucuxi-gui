#include "covariateitemwidget.h"
#include "ui_covariateitemwidget.h"

#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <limits>

CovariateItemWidget::CovariateItemWidget(QWidget *parent) : DomItemWidget(parent), ui(new Ui::CovariateItemWidget)
{
    ui->setupUi(this);

    ui->definitionLayout->getWidgetPosition(ui->defaultWidget, &_defaultWidgetPosition, &_defaultWidgetRole);
    _defaultWidget.reset(ui->defaultWidget);

    ui->typeBox->addItem(tr("Decimal"), QVariant::Double);
    ui->typeBox->addItem(tr("Integer"), QVariant::Int);
    ui->typeBox->addItem(tr("True/False"), QVariant::Bool);

    connect(ui->typeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(typeChanged(int)));
}

CovariateItemWidget::~CovariateItemWidget()
{
    delete ui;
}

bool CovariateItemWidget::setNodeImpl(QDomElement node)
{
    ui->typeBox->setCurrentIndex(-1);

    _parser.setNode(node);
    return _parser.isValid();
}

void CovariateItemWidget::setData()
{
    ui->idLine->setText(_parser.id());
    ui->idLine->setEnabled(true);

    ui->nameLine->setText(_parser.name());
    ui->nameLine->setEnabled(true);

    ui->descriptionText->setPlainText(_parser.description());
    ui->descriptionText->setEnabled(true);

    ui->unitBox->setCurrentUnit(_parser.unit());
    ui->unitBox->setEnabled(true);

    ui->typeBox->setCurrentIndex(ui->typeBox->findData(QMetaType::type(_parser.type().toLocal8Bit())));
    ui->typeBox->setEnabled(true);
}

void CovariateItemWidget::clearData()
{
    ui->idLine->clear();
    ui->idLine->setEnabled(false);

    ui->nameLine->clear();
    ui->nameLine->setEnabled(false);

    ui->descriptionText->clear();
    ui->descriptionText->setEnabled(false);

    ui->unitBox->setCurrentIndex(-1);
    ui->unitBox->setEnabled(false);

    ui->typeBox->setCurrentIndex(-1);
    ui->typeBox->setEnabled(false);
}

void CovariateItemWidget::typeChanged(int index)
{
    ui->definitionLayout->removeWidget(_defaultWidget.data());

    if (index == -1)
        return;

    switch (ui->typeBox->itemData(index).toInt()) {
        case QMetaType::Bool:
        {
            QCheckBox *checkBox = new QCheckBox();
            checkBox->setChecked(_parser.defaultValue() != 0.0);

            _defaultWidget.reset(checkBox);
            break;
        }
        case QMetaType::Int:
        {
            QSpinBox *spinBox = new QSpinBox();
            spinBox->setMaximum(std::numeric_limits<int>::max());
            spinBox->setMinimum(std::numeric_limits<int>::min());
            spinBox->setValue((int)_parser.defaultValue());

            _defaultWidget.reset(spinBox);
            break;
        }
        default:
        {
            QDoubleSpinBox *doubleSpinBox = new QDoubleSpinBox();
            doubleSpinBox->setMaximum((double)std::numeric_limits<int>::max());
            doubleSpinBox->setMinimum((double)std::numeric_limits<int>::min());
            doubleSpinBox->setValue(_parser.defaultValue());

            _defaultWidget.reset(doubleSpinBox);
            break;
        }
    }

    ui->definitionLayout->setWidget(_defaultWidgetPosition, _defaultWidgetRole, _defaultWidget.data());
}
