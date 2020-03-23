#ifndef DOMITEMWIDGETFACTORY_H
#define DOMITEMWIDGETFACTORY_H

#include <QByteArray>
#include <QHash>

#include "domitemwidget.h"

class DomItemWidgetFactory
{

public:

    /** Registers a DomItemWidget for the given parser.
     * @param A parser class name.
     */
    template<typename T>
    static void registerClass(QByteArray parserClass)
    {
        constructors().insert(parserClass, &constructorHelper<T>);
    }

    //! Returns a new DomItemWidget corresponding to the given the parser.
    /** If the parser cannot be found, an empty item widget is returned.
     * The caller has the responsability to delete the widget when it is no longer needed.
     * @param parserClass The class name of the parser.
     * @param parent An optional parent of the QDomItemWidget.
     * @return A pointer to the new DomItemWidget.
     * \sa EmptyWidgetItem
     */
    static DomItemWidget *build(const QByteArray &parserClass, QWidget *parent = 0);

private:
    //Type definition of the constructorHelper() function
    typedef DomItemWidget* (*Constructor)(QWidget *parent);

    //Template function returning a new DomItemWidget
    template<typename T>
    static DomItemWidget* constructorHelper(QWidget *parent)
    {
        return new T(parent);
    }

    //QHash of <Parser class name, constructorHelper<T> >
    static QHash<QByteArray, Constructor> &constructors();
};

#endif // DOMITEMWIDGETFACTORY_H
