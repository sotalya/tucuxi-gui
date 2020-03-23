#ifndef CREATORITEM_H
#define CREATORITEM_H

#include "outeritem.h"

class CreatorItem : public OuterItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the creator parser class name.
     * @return The creator parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    CreatorItem(QDomElement node, DomItem *parent);
};

#endif // CREATORITEM_H
