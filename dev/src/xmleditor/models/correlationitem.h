#ifndef CORRELATIONITEM_H
#define CORRELATIONITEM_H

#include "outeritem.h"

class CorrelationItem : public OuterItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns a name formed by the two ID of the correlation's parameters.
     * @return The name of the correlation.
     */
    virtual QString name() const;

    /** \brief Returns the correlation parser class name.
     * @return The correlation parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    CorrelationItem(QDomElement node, DomItem *parent);
};

#endif // CORRELATIONITEM_H
