#ifndef DRUGITEM_H
#define DRUGITEM_H

#include "inneritem.h"

class DrugItem : public InnerItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the name of the drug.
     * @return The name of the drug.
     */
    virtual QString name() const;

    /** \brief Returns the drug parser class name.
     * @return The drug parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    DrugItem(QDomElement node, DomItem *parent);
};

#endif // DRUGITEM_H
