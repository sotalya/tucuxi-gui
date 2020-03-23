#ifndef OUTERITEM_H
#define OUTERITEM_H

#include "domitem.h"

class OuterItem : public DomItem
{

public:

    /** \brief Constructor.
     * @param node The node of the item.
     * @param parent The parent of the item.
     */
    OuterItem(QDomElement node, DomItem *parent);

    //! Pure virtual destructor.
    ~OuterItem() = 0;

private:
    //Do nothing
    virtual void initChildrenImpl();
};

#endif // OUTERITEM_H
