#ifndef DISPOSITIONITEM_H
#define DISPOSITIONITEM_H

#include "inneritem.h"

class DispositionItem : public InnerItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the name or ID of the disposition.
     * @return The name or ID of the disposition.
     */
    virtual QString name() const;

    /** \brief Returns the disposition parser class name.
     * @return The disposition parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    DispositionItem(QDomElement node, DomItem *parent);
};

#endif // DISPOSITIONITEM_H
