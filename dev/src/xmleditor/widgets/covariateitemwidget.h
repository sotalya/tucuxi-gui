#ifndef COVARIATEITEMWIDGET_H
#define COVARIATEITEMWIDGET_H

#include "domitemwidget.h"
#include "covariateparser.h"
#include <QFormLayout>

namespace Ui {
    class CovariateItemWidget;
}

class CovariateItemWidget : public DomItemWidget
{
    Q_OBJECT
    
public:
    explicit CovariateItemWidget(QWidget *parent = 0);
    ~CovariateItemWidget();
    
private:
    bool setNodeImpl(QDomElement node);
    void setData();
    void clearData();

    Ui::CovariateItemWidget *ui;
    CovariateParser _parser;
    QScopedPointer<QWidget> _defaultWidget;
    QFormLayout::ItemRole _defaultWidgetRole;
    int _defaultWidgetPosition;

private slots:
    void typeChanged(int index);
};

#endif // COVARIATEITEMWIDGET_H
