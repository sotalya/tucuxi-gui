#ifndef COMMENTITEM_H
#define COMMENTITEM_H

#include "outeritem.h"

class CommentItem : public OuterItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the comment parser class name.
     * @return The comment parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    CommentItem(QDomElement node, DomItem *parent);
};

#endif // COMMENTITEM_H
