#ifndef ERRORMODELITEMWIDGET_H
#define ERRORMODELITEMWIDGET_H

#include "domitemwidget.h"
#include "errormodelparser.h"

namespace Ui {
    class ErrorModelItemWidget;
}

class ErrorModelItemWidget : public DomItemWidget
{
    Q_OBJECT
    
public:
    explicit ErrorModelItemWidget(QWidget *parent = 0);
    ~ErrorModelItemWidget();
    
private:
    bool setNodeImpl(QDomElement node);
    void setData();
    void clearData();

    Ui::ErrorModelItemWidget *ui;
    ErrorModelParser _parser;
};

#endif // ERRORMODELITEMWIDGET_H
