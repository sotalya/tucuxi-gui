#include "intakeheaditemwidget.h"
#include "ui_intakeheaditemwidget.h"
#include "core.h"
#include "definitionmanager.h"

IntakeHeadItemWidget::IntakeHeadItemWidget(QWidget *parent) : DomItemWidget(parent), ui(new Ui::IntakeHeadItemWidget)
{
    ui->setupUi(this);

    foreach (Descriptor d, CORE->definitionManager()->intakes())
        ui->intakeCombo->addItem(d.name, d.id);
}

IntakeHeadItemWidget::~IntakeHeadItemWidget()
{
    delete ui;
}

bool IntakeHeadItemWidget::setNodeImpl(QDomElement node)
{
    _parser.setNode(node);
    return _parser.isValid();
}

void IntakeHeadItemWidget::setData()
{
    ui->idLine->setText(_parser.id());
    ui->idLine->setEnabled(true);

    ui->intakeCombo->setCurrentIndex(ui->intakeCombo->findData(_parser.intakeId()));
    ui->intakeCombo->setEnabled(true);
}

void IntakeHeadItemWidget::clearData()
{
    ui->idLine->clear();
    ui->idLine->setEnabled(false);

    ui->intakeCombo->setCurrentIndex(-1);
    ui->intakeCombo->setEnabled(false);
}

