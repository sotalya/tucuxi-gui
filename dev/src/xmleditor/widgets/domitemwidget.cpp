#include "domitemwidget.h"

//Default constructor
DomItemWidget::DomItemWidget(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f)
{
    connect(this, SIGNAL(invalidParser()), this, SLOT(clearData()));
}

//Destructor
DomItemWidget::~DomItemWidget()
{

}

//Set the widget node
void DomItemWidget::setNode(QDomElement node)
{
    if (setNodeImpl(node))
        setData();
    else
        emit invalidParser();
}
