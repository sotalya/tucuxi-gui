#ifndef INTAKEHEADITEMWIDGET_H
#define INTAKEHEADITEMWIDGET_H

#include "domitemwidget.h"
#include "intakeheadparser.h"

namespace Ui {
    class IntakeHeadItemWidget;
}

class IntakeHeadItemWidget : public DomItemWidget
{
    Q_OBJECT
    
public:
    explicit IntakeHeadItemWidget(QWidget *parent = 0);
    ~IntakeHeadItemWidget();
    
private:
    bool setNodeImpl(QDomElement node);
    void setData();
    void clearData();

    Ui::IntakeHeadItemWidget *ui;
    IntakeHeadParser _parser;
};

#endif // INTAKEHEADITEMWIDGET_H
