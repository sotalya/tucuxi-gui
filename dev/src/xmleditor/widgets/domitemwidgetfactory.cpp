#include "domitemwidgetfactory.h"
#include "emptyitemwidget.h"

//Returns a new DomItemWidget corresponding to the given the parser class name
DomItemWidget *DomItemWidgetFactory::build(const QByteArray &parserClass, QWidget *parent)
{
    //Get the constructorHelper() function corresponding to the given parser
    Constructor constructor = constructors().value(parserClass);

    //Returns an empty item widget if the constructor helper doesn't exist
    if (constructor == 0)
        return new EmptyItemWidget(parent);

    return (*constructor)(parent);
}

//Returns the hash table containing the "Parser class name" / "DomItemWidget constructorHelper" pairs
QHash<QByteArray, DomItemWidgetFactory::Constructor> &DomItemWidgetFactory::constructors()
{
    static QHash<QByteArray, Constructor> instance;
    return instance;
}
