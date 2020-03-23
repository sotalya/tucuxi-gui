#ifndef EMPTYITEMWIDGET_H
#define EMPTYITEMWIDGET_H

#include "domitemwidget.h"

namespace Ui {
    class EmptyItemWidget;
}

class EmptyItemWidget : public DomItemWidget
{
    Q_OBJECT

public:

    /** \brief Default constructor.
     * @param parent An optional parent widget.
     * @param f An optional Qt::WindowFlags.
     */
    EmptyItemWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);

    //! Destructor.
    ~EmptyItemWidget();

private:
    //Returns true.
    bool setNodeImpl(QDomElement node);

    //Does nothing
    void setData();

    //Does nothing
    void clearData();

    //Widget form
    Ui::EmptyItemWidget *ui;
};

#endif // EMPTYITEMWIDGET_H
