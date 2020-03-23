#ifndef DOSAGESITEM_H
#define DOSAGESITEM_H

#include "inneritem.h"

class DosagesItem : public InnerItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the dosages parser class name.
     * @return The dosages parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    DosagesItem(QDomElement node, DomItem *parent);
};

#endif // DOSAGESITEM_H
