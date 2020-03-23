#ifndef DRUGHEADITEMWIDGET_H
#define DRUGHEADITEMWIDGET_H

#include "domitemwidget.h"
#include "drugheadparser.h"

namespace Ui {
    class DrugHeadItemWidget;
}

class DrugHeadItemWidget : public DomItemWidget
{
    Q_OBJECT
    
public:
    explicit DrugHeadItemWidget(QWidget *parent = 0);
    ~DrugHeadItemWidget();

private:
    bool setNodeImpl(QDomElement node);
    void setData();
    void clearData();

    Ui::DrugHeadItemWidget *ui;
    DrugHeadParser _parser;
};

#endif // DRUGHEADITEMWIDGET_H
