#ifndef INTAKEITEM_H
#define INTAKEITEM_H

#include "inneritem.h"

class IntakeItem : public InnerItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the name of the intake.
     * @return The name of the intake.
     */
    virtual QString name() const;

    /** \brief Returns the intake parser class name.
     * @return The intake parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    IntakeItem(QDomElement node, DomItem *parent);
};

#endif // INTAKEITEM_H
