#ifndef COVARIATEITEM_H
#define COVARIATEITEM_H

#include "outeritem.h"

class CovariateItem : public OuterItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the name of the covariate.
     * @return The name of the covariate.
     */
    virtual QString name() const;

    /** \brief Returns the covariate parser class name.
     * @return The covariate parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    CovariateItem(QDomElement node, DomItem *parent);
};

#endif // COVARIATEITEM_H
