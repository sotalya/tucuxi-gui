#ifndef DOSAGEITEMWIDGET_H
#define DOSAGEITEMWIDGET_H

#include "domitemwidget.h"
#include "dosageparser.h"

namespace Ui {
    class DosageItemWidget;
}

class DosageItemWidget : public DomItemWidget
{
    Q_OBJECT
    
public:
    explicit DosageItemWidget(QWidget *parent = 0);
    ~DosageItemWidget();
    
private:
    bool setNodeImpl(QDomElement node);
    void setData();
    void clearData();

    Ui::DosageItemWidget *ui;
    DosageParser _parser;

private slots:
    void intakeChanged(int index);
};

#endif // DOSAGEITEMWIDGET_H
