#ifndef CORRELATIONITEMWIDGET_H
#define CORRELATIONITEMWIDGET_H

#include "domitemwidget.h"
#include "correlationparser.h"

namespace Ui {
    class CorrelationItemWidget;
}

class CorrelationItemWidget : public DomItemWidget
{
    Q_OBJECT
    
public:
    explicit CorrelationItemWidget(QWidget *parent = 0);
    ~CorrelationItemWidget();
    
private:
    bool setNodeImpl(QDomElement node);
    void setData();
    void clearData();

    Ui::CorrelationItemWidget *ui;
    CorrelationParser _parser;
};

#endif // CORRELATIONITEMWIDGET_H
