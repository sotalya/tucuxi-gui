#ifndef OPERATIONITEMWIDGET_H
#define OPERATIONITEMWIDGET_H

#include "domitemwidget.h"
#include "operationparser.h"

namespace Ui {
    class OperationItemWidget;
}

class OperationItemWidget : public DomItemWidget
{
    Q_OBJECT
    
public:
    explicit OperationItemWidget(QWidget *parent = 0);
    ~OperationItemWidget();
    
private:
    bool setNodeImpl(QDomElement node);
    void setData();
    void clearData();

    Ui::OperationItemWidget *ui;
    OperationParser _parser;
};

#endif // OPERATIONITEMWIDGET_H
