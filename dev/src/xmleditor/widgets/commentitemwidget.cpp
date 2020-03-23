#include "commentitemwidget.h"
#include "ui_commentitemwidget.h"

CommentItemWidget::CommentItemWidget(QWidget *parent) : DomItemWidget(parent), ui(new Ui::CommentItemWidget)
{
    ui->setupUi(this);

    ui->biblioSplitter->setStretchFactor(0, 1);
    ui->biblioSplitter->setStretchFactor(1, 2);
}

CommentItemWidget::~CommentItemWidget()
{
    delete ui;
}

bool CommentItemWidget::setNodeImpl(QDomElement node)
{
    _parser.setNode(node);
    return _parser.isValid();
}

void CommentItemWidget::setData()
{
    ui->textText->setPlainText(_parser.text());
    ui->textText->setEnabled(true);

    ui->biblioList->clear();
    ui->biblioList->setEnabled(true);

    ui->biblioText->clear();
    ui->biblioText->setEnabled(true);
}

void CommentItemWidget::clearData()
{
    ui->textText->clear();
    ui->textText->setEnabled(false);

    ui->biblioList->clear();
    ui->biblioList->setEnabled(false);

    ui->biblioText->clear();
    ui->biblioText->setEnabled(false);
}
