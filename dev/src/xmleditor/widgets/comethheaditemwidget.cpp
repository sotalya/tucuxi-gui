#include "comethheaditemwidget.h"
#include "ui_comethheaditemwidget.h"
#include "core.h"
#include "pluginmanager.h"

ComethHeadItemWidget::ComethHeadItemWidget(QWidget *parent) : DomItemWidget(parent), ui(new Ui::ComethHeadItemWidget)
{
    ui->setupUi(this);

    connect(ui->pluginCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(pluginChanged(int)));

    foreach (ModelDescriptor d, CORE->pluginManager()->models())
        ui->pluginCombo->addItem(d.name, d.id);
}

ComethHeadItemWidget::~ComethHeadItemWidget()
{
    delete ui;
}

bool ComethHeadItemWidget::setNodeImpl(QDomElement node)
{
    _parser.setNode(node);
    return _parser.isValid();
}

void ComethHeadItemWidget::setData()
{
    ui->idLine->setText(_parser.id());
    ui->idLine->setEnabled(true);

    ui->pluginCombo->setCurrentIndex(ui->pluginCombo->findData(_parser.comethId()));
    ui->pluginCombo->setEnabled(true);
}

void ComethHeadItemWidget::clearData()
{
    ui->idLine->clear();
    ui->idLine->setEnabled(false);

    ui->pluginCombo->setCurrentIndex(-1);
    ui->pluginCombo->setEnabled(false);

    ui->pluginIdLine->clear();
    ui->pluginIdLine->setEnabled(false);
}

void ComethHeadItemWidget::pluginChanged(int index)
{
    ui->pluginIdLine->setText(ui->pluginCombo->itemData(index).toString());
    ui->pluginIdLine->setEnabled(true);
}
