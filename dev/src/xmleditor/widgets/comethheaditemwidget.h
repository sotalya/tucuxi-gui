#ifndef COMETHHEADITEMWIDGET_H
#define COMETHHEADITEMWIDGET_H

#include "domitemwidget.h"
#include "comethheadparser.h"

namespace Ui {
    class ComethHeadItemWidget;
}

class ComethHeadItemWidget : public DomItemWidget
{
    Q_OBJECT
    
public:
    explicit ComethHeadItemWidget(QWidget *parent = 0);
    ~ComethHeadItemWidget();
    
private:
    bool setNodeImpl(QDomElement node);
    void setData();
    void clearData();

    Ui::ComethHeadItemWidget *ui;
    ComethHeadParser _parser;

private slots:
    void pluginChanged(int index);
};

#endif // COMETHHEADITEMWIDGET_H
