#ifndef COMETHITEM_H
#define COMETHITEM_H

#include "inneritem.h"

class ComethItem : public InnerItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the name of the cometh.
     * @return The name of the cometh.
     */
    virtual QString name() const;

    /** \brief Returns the cometh parser class name.
     * @return The target cometh class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    ComethItem(QDomElement node, DomItem *parent);
};

#endif // COMETHITEM_H
