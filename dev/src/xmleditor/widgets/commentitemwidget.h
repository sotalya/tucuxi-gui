#ifndef COMMENTITEMWIDGET_H
#define COMMENTITEMWIDGET_H

#include "domitemwidget.h"
#include "commentparser.h"

namespace Ui {
    class CommentItemWidget;
}

class CommentItemWidget : public DomItemWidget
{
    Q_OBJECT
    
public:
    explicit CommentItemWidget(QWidget *parent = 0);
    ~CommentItemWidget();
    
private:
    bool setNodeImpl(QDomElement node);
    void setData();
    void clearData();

    Ui::CommentItemWidget *ui;
    CommentParser _parser;
};

#endif // COMMENTITEMWIDGET_H
