#ifndef PARAMETERITEM_H
#define PARAMETERITEM_H

#include "outeritem.h"

class ParameterItem : public OuterItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the name (ID) of the parameter.
     * @return The name of the parameter.
     */
    virtual QString name() const;

    /** \brief Returns the parameter parser class name.
     * @return The parameter parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private factory
    ParameterItem(QDomElement node, DomItem *parent);
};

#endif // PARAMETERITEM_H
