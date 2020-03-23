#ifndef INTERVALSITEM_H
#define INTERVALSITEM_H

#include "outeritem.h"

class IntervalsItem : public OuterItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the intervals parser class name.
     * @return The intervals parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    IntervalsItem(QDomElement node, DomItem *parent);
};

#endif // INTERVALSITEM_H
