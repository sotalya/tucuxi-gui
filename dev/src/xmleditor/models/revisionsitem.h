#ifndef REVISIONSITEM_H
#define REVISIONSITEM_H

#include "inneritem.h"

class RevisionsItem : public InnerItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the revisions parser class name.
     * @return The revisions parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    RevisionsItem(QDomElement node, DomItem *parent);
};

#endif // REVISIONSITEM_H
