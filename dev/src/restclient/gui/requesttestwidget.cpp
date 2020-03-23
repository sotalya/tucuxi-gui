#include "requesttestwidget.h"
#include "ui_requesttestwidget.h"

RequestTestWidget::RequestTestWidget(MirthRequest *request, QWidget *parent) :
    AbstractTestWidget(request, parent),
    _ui(new Ui::RequestTestWidget)
{
    _ui->setupUi(this);

    connect(_ui->pushButton, SIGNAL(clicked()), this, SLOT(start()));
}

RequestTestWidget::~RequestTestWidget()
{
    delete _ui;
}

QPushButton *RequestTestWidget::pushButton()
{
    return _ui->pushButton;
}

QProgressBar *RequestTestWidget::progressBar()
{
    return _ui->progressBar;
}

QPlainTextEdit *RequestTestWidget::textArea()
{
    return _ui->textArea;
}
