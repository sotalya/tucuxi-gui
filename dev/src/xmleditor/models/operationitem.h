#ifndef OPERATIONITEM_H
#define OPERATIONITEM_H

#include "outeritem.h"

class OperationItem : public OuterItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the name (ID) of the operation parameter.
     * @return The name of the operation parameter.
     */
    virtual QString name() const;

    /** \brief Returns the operation parser class name.
     * @return The operation parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    OperationItem(QDomElement node, DomItem *parent);
};

#endif // OPERATIONITEM_H
