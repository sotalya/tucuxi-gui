#ifndef DOMITEMWIDGET_H
#define DOMITEMWIDGET_H

#include <QWidget>
#include <QDomElement>

class DomItemWidget : public QWidget
{
    Q_OBJECT

public:

    /** \brief Default constructor.
     * @param parent An optional parent widget.
     * @param f An optional Qt::WindowFlags.
     */
    DomItemWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);

    //! Virtual destructor
    ~DomItemWidget() = 0;

    /** Sets the item widget's node.
     * @param node A QDomElement node.
     */
    void setNode(QDomElement node);

signals:
    //! Sent when the widget's parser is in an invalid state.
    void invalidParser();

private:
    /** \brief Assigns this node to the widget's internal parser.
     * @param node A QDomElement node.
     * @return Returns true if the parser is in a valid state, otherwise returns false.
     */
    virtual bool setNodeImpl(QDomElement node) = 0;

private slots:
    //! Sets the widget data according to its type and internal parser.
    virtual void setData() = 0;

    //! Clears the widget data and disable its children widgets.
    virtual void clearData() = 0;
};

#endif // DOMITEMWIDGET_H
