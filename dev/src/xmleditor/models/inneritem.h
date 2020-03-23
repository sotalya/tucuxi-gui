#ifndef INNERITEM_H
#define INNERITEM_H

#include "domitem.h"

class InnerItem : public DomItem
{

public:

    /** \brief Constructor.
     * @param node The node of the item.
     * @param parent The parent of the item.
     */
    InnerItem(QDomElement node, DomItem *parent);

    //! Pure virtual destructor.
    ~InnerItem() = 0;

private:
    //Initializes the item children
    virtual void initChildrenImpl();
};

#endif // INNERITEM_H
