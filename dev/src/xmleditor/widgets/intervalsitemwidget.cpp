#include "intervalsitemwidget.h"
#include "ui_intervalsitemwidget.h"
#include "durationspinbox.h"

IntervalsItemWidget::IntervalsItemWidget(QWidget *parent) : DomItemWidget(parent), ui(new Ui::IntervalsItemWidget)
{
    ui->setupUi(this);
}

IntervalsItemWidget::~IntervalsItemWidget()
{
    delete ui;
}

bool IntervalsItemWidget::setNodeImpl(QDomElement node)
{
    _parser.setNode(node);
    return _parser.isValid();
}

void IntervalsItemWidget::setData()
{
    ui->intervalList->clear();
    ui->intervalList->setEnabled(true);
    foreach (double value, _parser.values()) {
        QListWidgetItem *item = new QListWidgetItem(DurationSpinBox::toString(value));
        item->setData(Qt::UserRole, value);

        ui->intervalList->addItem(item);
    }
}

void IntervalsItemWidget::clearData()
{
    ui->intervalList->clear();
    ui->intervalList->setEnabled(false);
}
