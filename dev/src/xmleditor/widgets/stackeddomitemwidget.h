#ifndef STACKEDDOMITEMWIDGET_H
#define STACKEDDOMITEMWIDGET_H

#include <QStackedWidget>
#include <QHash>

class DomItemWidget;
class DomItem;

class StackedDomItemWidget : public QStackedWidget
{
    Q_OBJECT

public:

    /** \brief Default constructor.
     * @param parent An optional parent widget.
     */
    StackedDomItemWidget(QWidget *parent = 0);

public slots:

    //! Set the current widget corresponding to the given DOM item.
    /** This function will set the item widget corresponding to the given item as the current widget.
     * If the widget has not been build yet, it will be constructed and added to the internal map.
     * @param item A pointer to a DomItem.
     */
    void setCurrentItemWidget(DomItem *item);

    /** \brief Set an empty item widget as the current widget.
     * \sa EmptyItemWidget
     */
    void setEmptyItemWidget();

private:
    //QHash of <Parser class name, DomItemWidget *>
    QHash<QByteArray, DomItemWidget *> _parserToWidget;
};

#endif // STACKEDDOMITEMWIDGET_H
