#ifndef COVARIATESITEM_H
#define COVARIATESITEM_H

#include "inneritem.h"

class CovariatesItem : public InnerItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the covariates parser class name.
     * @return The covariates parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    CovariatesItem(QDomElement node, DomItem *parent);
};

#endif // COVARIATESITEM_H
