#ifndef COMETHSITEM_H
#define COMETHSITEM_H

#include "inneritem.h"

class ComethsItem : public InnerItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the comeths parser class name.
     * @return The comeths parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    ComethsItem(QDomElement node, DomItem *parent);
};

#endif // COMETHSITEM_H
