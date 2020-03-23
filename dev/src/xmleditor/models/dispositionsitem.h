#ifndef DISPOSITIONSITEM_H
#define DISPOSITIONSITEM_H

#include "inneritem.h"

class DispositionsItem : public InnerItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the dispositions parser class name.
     * @return The dispositions parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    DispositionsItem(QDomElement node, DomItem *parent);
};

#endif // DISPOSITIONSITEM_H
