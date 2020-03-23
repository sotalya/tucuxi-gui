#ifndef DISPOSITIONHEADITEMWIDGET_H
#define DISPOSITIONHEADITEMWIDGET_H

#include "domitemwidget.h"
#include "dispositionheadparser.h"

namespace Ui {
    class DispositionHeadItemWidget;
}

class DispositionHeadItemWidget : public DomItemWidget
{
    Q_OBJECT
    
public:
    explicit DispositionHeadItemWidget(QWidget *parent = 0);
    ~DispositionHeadItemWidget();
    
private:
    bool setNodeImpl(QDomElement node);
    void setData();
    void clearData();

    Ui::DispositionHeadItemWidget *ui;
    DispositionHeadParser _parser;
};

#endif // DISPOSITIONHEADITEMWIDGET_H
