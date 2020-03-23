#ifndef CORRELATIONSITEM_H
#define CORRELATIONSITEM_H

#include "inneritem.h"

class CorrelationsItem : public InnerItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the correlations parser class name.
     * @return The correlations parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    CorrelationsItem(QDomElement node, DomItem *parent);
};

#endif // CORRELATIONSITEM_H
