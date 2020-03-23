#ifndef TARGETITEMWIDGET_H
#define TARGETITEMWIDGET_H

#include "domitemwidget.h"
#include "targetparser.h"

namespace Ui {
    class TargetItemWidget;
}

class TargetItemWidget : public DomItemWidget
{
    Q_OBJECT
    
public:
    explicit TargetItemWidget(QWidget *parent = 0);
    ~TargetItemWidget();
    
private:
    bool setNodeImpl(QDomElement node);
    void setData();
    void clearData();

    Ui::TargetItemWidget *ui;
    TargetParser _parser;

private slots:
    void targetChanged(int index);
};

#endif // TARGETITEMWIDGET_H
