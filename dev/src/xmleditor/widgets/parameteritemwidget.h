#ifndef PARAMETERITEMWIDGET_H
#define PARAMETERITEMWIDGET_H

#include "domitemwidget.h"
#include "parameterparser.h"

namespace Ui {
    class ParameterItemWidget;
}

class ParameterItemWidget : public DomItemWidget
{
    Q_OBJECT
    
public:
    explicit ParameterItemWidget(QWidget *parent = 0);
    ~ParameterItemWidget();
    
private:
    bool setNodeImpl(QDomElement node);
    void setData();
    void clearData();

    Ui::ParameterItemWidget *ui;
    ParameterParser _parser;
};

#endif // PARAMETERITEMWIDGET_H
