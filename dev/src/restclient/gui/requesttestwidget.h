#ifndef REQUESTTESTWIDGET_H
#define REQUESTTESTWIDGET_H

#include "abstracttestwidget.h"

namespace Ui {
    class RequestTestWidget;
}

class RequestTestWidget : public AbstractTestWidget
{
    Q_OBJECT

public:
    explicit RequestTestWidget(MirthRequest *request, QWidget *parent = 0);
    virtual ~RequestTestWidget() Q_DECL_OVERRIDE;

protected:
    virtual QPushButton *pushButton();
    virtual QProgressBar *progressBar();
    virtual QPlainTextEdit *textArea();

private:
    Ui::RequestTestWidget *_ui;
};

#endif // REQUESTTESTWIDGET_H
