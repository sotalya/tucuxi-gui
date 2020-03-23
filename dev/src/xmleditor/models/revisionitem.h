#ifndef REVISIONITEM_H
#define REVISIONITEM_H

#include "outeritem.h"

class RevisionItem : public OuterItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the date of the revision.
     * @return The date of the revision.
     */
    virtual QString name() const;

    /** \brief Returns the revision parser class name.
     * @return The revision parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    RevisionItem(QDomElement node, DomItem *parent);
};

#endif // REVISIONITEM_H
