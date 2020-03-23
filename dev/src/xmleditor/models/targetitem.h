#ifndef TARGETITEM_H
#define TARGETITEM_H

#include "outeritem.h"

class TargetItem : public OuterItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the target type name.
     * @return The target type name.
     */
    virtual QString name() const;

    /** \brief Returns the target parser class name.
     * @return The target parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    TargetItem(QDomElement node, DomItem *parent);
};

#endif // TARGETITEM_H
