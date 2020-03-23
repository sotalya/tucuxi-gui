#include "targetitemwidget.h"
#include "ui_targetitemwidget.h"
#include "core.h"
#include "definitionmanager.h"

#include <limits>

TargetItemWidget::TargetItemWidget(QWidget *parent) : DomItemWidget(parent), ui(new Ui::TargetItemWidget)
{
    ui->setupUi(this);

    foreach (Descriptor d, CORE->definitionManager()->targets())
        ui->targetBox->addItem(d.name, d.id);

    ui->CMinValBox->setMaximum((double)std::numeric_limits<int>::max());
    ui->CMaxValBox->setMaximum((double)std::numeric_limits<int>::max());
    ui->CBestValBox->setMaximum((double)std::numeric_limits<int>::max());

    ui->TMinValBox->setMaximum((double)std::numeric_limits<int>::max());
    ui->TMinValBox->setSuffix(" hours");
    ui->TMaxValBox->setMaximum((double)std::numeric_limits<int>::max());
    ui->TMaxValBox->setSuffix(" hours");
    ui->TBestValBox->setMaximum((double)std::numeric_limits<int>::max());
    ui->TBestValBox->setSuffix(" hours");

    connect(ui->targetBox, SIGNAL(currentIndexChanged(int)), this, SLOT(targetChanged(int)));
}

TargetItemWidget::~TargetItemWidget()
{
    delete ui;
}

bool TargetItemWidget::setNodeImpl(QDomElement node)
{
    ui->targetBox->setCurrentIndex(-1);

    _parser.setNode(node);
    return _parser.isValid();
}

void TargetItemWidget::setData()
{
    ui->targetBox->setCurrentIndex(ui->targetBox->findData(_parser.targetType()));
    ui->targetBox->setEnabled(true);

    ui->CMinValBox->setValue(_parser.concMinValue());
    ui->CMinValBox->setEnabled(true);

    ui->CMinForText->setPlainText(_parser.concMinFormula().trimmed());
    ui->CMinForText->setEnabled(true);

    ui->CMaxValBox->setValue(_parser.concMaxValue());
    ui->CMaxValBox->setEnabled(true);

    ui->CMaxForText->setPlainText(_parser.concMaxFormula().trimmed());
    ui->CMaxForText->setEnabled(true);

    ui->CBestValBox->setValue(_parser.concBestValue());
    ui->CBestValBox->setEnabled(true);

    ui->CBestForText->setPlainText(_parser.concBestFormula().trimmed());
    ui->CBestForText->setEnabled(true);
}

void TargetItemWidget::clearData()
{
    ui->targetBox->setCurrentIndex(-1);
    ui->targetBox->setEnabled(false);

    ui->CMinValBox->clear();
    ui->CMinValBox->setEnabled(false);

    ui->CMinForText->clear();
    ui->CMinForText->setEnabled(false);

    ui->CMaxValBox->clear();
    ui->CMaxValBox->setEnabled(false);

    ui->CMaxForText->clear();
    ui->CMaxForText->setEnabled(false);

    ui->CBestValBox->clear();
    ui->CBestValBox->setEnabled(false);

    ui->CBestForText->clear();
    ui->CBestForText->setEnabled(false);
}

void TargetItemWidget::targetChanged(int index)
{
    if (ui->targetBox->itemData(index) == "peak") {
        ui->TMinValBox->setValue(_parser.timeMinValue());
        ui->TMaxValBox->setValue(_parser.timeMaxValue());
        ui->TBestValBox->setValue(_parser.timeBestValue());
        ui->TMinForText->setPlainText(_parser.timeMinFormula().trimmed());
        ui->TMaxForText->setPlainText(_parser.timeMaxFormula().trimmed());
        ui->TBestForText->setPlainText(_parser.timeBestFormula().trimmed());

        ui->timeBox->show();
    } else {
        ui->TMinValBox->clear();
        ui->TMaxValBox->clear();
        ui->TBestValBox->clear();
        ui->TMinForText->clear();
        ui->TMaxForText->clear();
        ui->TBestForText->clear();

        ui->timeBox->hide();
    }
}
