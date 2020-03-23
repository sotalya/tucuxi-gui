#ifndef HEADITEM_H
#define HEADITEM_H

#include "outeritem.h"

class HeadItem : public OuterItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the correpsonding head parser class name.
     * @return The corresponding head parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    HeadItem(QDomElement node, DomItem *parent);
};

#endif // HEADITEM_H
