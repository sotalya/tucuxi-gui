#ifndef OPERATIONSITEM_H
#define OPERATIONSITEM_H

#include "inneritem.h"

class OperationsItem : public InnerItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the operations parser class name.
     * @return The operations parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    OperationsItem(QDomElement node, DomItem *parent);
};

#endif // OPERATIONSITEM_H
