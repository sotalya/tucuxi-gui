#include "dosageitemwidget.h"
#include "ui_dosageitemwidget.h"
#include "core.h"
#include "definitionmanager.h"

#include <limits>

DosageItemWidget::DosageItemWidget(QWidget *parent) : DomItemWidget(parent), ui(new Ui::DosageItemWidget)
{
    ui->setupUi(this);

    foreach (Descriptor d, CORE->definitionManager()->intakes())
        ui->intakeCombo->addItem(d.name, d.id);

    connect(ui->intakeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(intakeChanged(int)));
}

DosageItemWidget::~DosageItemWidget()
{
    delete ui;
}

bool DosageItemWidget::setNodeImpl(QDomElement node)
{
    ui->intakeCombo->setCurrentIndex(-1);

    _parser.setNode(node);
    return _parser.isValid();
}

void DosageItemWidget::setData()
{
    ui->intakeCombo->setCurrentIndex(ui->intakeCombo->findData(_parser.intakeId()));
    ui->intakeCombo->setEnabled(true);

    ui->unitCombo->setCurrentUnit(_parser.unit());
    ui->unitCombo->setEnabled(true);

    ui->anyBox->setChecked(_parser.anyDose());
    ui->anyBox->setEnabled(true);

    ui->dosesList->clear();
    ui->dosesList->setEnabled(true);
    foreach (double a, _parser.values())
        ui->dosesList->addItem(new QListWidgetItem(QString::number(a) + " [" + _parser.unit() + "]"));
}

void DosageItemWidget::clearData()
{
    ui->intakeCombo->setCurrentIndex(-1);
    ui->intakeCombo->setEnabled(false);

    ui->unitCombo->setCurrentIndex(-1);
    ui->unitCombo->setEnabled(false);

    ui->anyBox->setChecked(false);
    ui->anyBox->setEnabled(false);

    ui->dosesList->clear();
    ui->dosesList->setEnabled(false);
}

void DosageItemWidget::intakeChanged(int index)
{
    if (CORE->definitionManager()->isInfusion(ui->intakeCombo->itemData(index).toString())) {
        ui->durationLabel->show();
        ui->durationBox->show();
        ui->durationBox->setValue(_parser.duration());
    } else {
        ui->durationLabel->hide();
        ui->durationBox->hide();
        ui->durationBox->clear();
    }
}
