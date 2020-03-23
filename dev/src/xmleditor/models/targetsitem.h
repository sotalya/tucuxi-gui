#ifndef TARGETSITEM_H
#define TARGETSITEM_H

#include "inneritem.h"

class TargetsItem : public InnerItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the targets parser class name.
     * @return The targets parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    TargetsItem(QDomElement node, DomItem *parent);

};

#endif // TARGETSITEM_H
