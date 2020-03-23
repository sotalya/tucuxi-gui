#include "emptyitemwidget.h"
#include "ui_emptyitemwidget.h"

//Default constructor
EmptyItemWidget::EmptyItemWidget(QWidget *parent, Qt::WindowFlags f) : DomItemWidget(parent, f), ui(new Ui::EmptyItemWidget)
{
    ui->setupUi(this);
}

EmptyItemWidget::~EmptyItemWidget()
{
    delete ui;
}

//Returns true
bool EmptyItemWidget::setNodeImpl(QDomElement node)
{
    Q_UNUSED(node)
    return true;
}

//Does nothing
void EmptyItemWidget::setData()
{

}

//Does nothing
void EmptyItemWidget::clearData()
{

}
