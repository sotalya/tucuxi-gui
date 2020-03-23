#ifndef INTERVALSITEMWIDGET_H
#define INTERVALSITEMWIDGET_H

#include "domitemwidget.h"
#include "intervalsparser.h"

class QListWidgetItem;

namespace Ui {
    class IntervalsItemWidget;
}

class IntervalsItemWidget : public DomItemWidget
{
    Q_OBJECT
    
public:
    explicit IntervalsItemWidget(QWidget *parent = 0);
    ~IntervalsItemWidget();
    
private:
    bool setNodeImpl(QDomElement node);
    void setData();
    void clearData();

    Ui::IntervalsItemWidget *ui;
    IntervalsParser _parser;
};

#endif // INTERVALSITEMWIDGET_H
