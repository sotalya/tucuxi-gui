#ifndef INTAKESITEM_H
#define INTAKESITEM_H

#include "inneritem.h"

class IntakesItem : public InnerItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the intakes parser class name.
     * @return The intakes parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    IntakesItem(QDomElement node, DomItem *parent);
};

#endif // INTAKESITEM_H
