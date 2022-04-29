//@@license@@

#ifndef ACKREQUESTTESTWIDGET_H
#define ACKREQUESTTESTWIDGET_H

#include "requesttestwidget.h"

class AckRequestTestWidget : public RequestTestWidget
{
    Q_OBJECT

public:
    explicit AckRequestTestWidget(QWidget *parent = 0);
    virtual ~AckRequestTestWidget() Q_DECL_OVERRIDE;

private:
    virtual void displaySuccess() Q_DECL_OVERRIDE;
};

#endif // ACKREQUESTTESTWIDGET_H
