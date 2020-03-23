#ifndef DOSAGEITEM_H
#define DOSAGEITEM_H

#include "outeritem.h"

class DosageItem : public OuterItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the intake type of the dosage.
     * @return The intake type's name.
     */
    virtual QString name() const;

    /** \brief Returns the dosage parser class name.
     * @return The dosage parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    DosageItem(QDomElement node, DomItem *parent);
};

#endif // DOSAGEITEM_H
