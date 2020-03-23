#ifndef ERRORMODELITEM_H
#define ERRORMODELITEM_H

#include "outeritem.h"

class ErrorModelItem : public OuterItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the error model parser class name.
     * @return The error model parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    ErrorModelItem(QDomElement node, DomItem *parent);
};

#endif // ERRORMODELITEM_H
