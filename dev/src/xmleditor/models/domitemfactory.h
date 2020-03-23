#ifndef DOMITEMFACTORY_H
#define DOMITEMFACTORY_H

#include <QDomElement>
#include <QCoreApplication>

class DomItem;

class DomItemFactory
{
    Q_DECLARE_TR_FUNCTIONS(DomItemFactory)

public:

    //! Build a new dom item.
    /** This function will initialize and return a dom item corresponding to the given node element.
     * The node element must be the root element of any drug item (covariate, parameter, group, model, etc.).
     * The caller has the responsability to delete the item when it is no longer needed.
     * @param node A dom element.
     * @param parent An optional parent item.
     * @return The corresponding item.
     */
    DomItem *build(QDomElement node, DomItem *parent = 0);
};

#endif // DOMITEMFACTORY_H
