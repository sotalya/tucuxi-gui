#ifndef PARAMETERSITEM_H
#define PARAMETERSITEM_H

#include "inneritem.h"

class ParametersItem : public InnerItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the parameters parser class name.
     * @return The parameters parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    ParametersItem(QDomElement node, DomItem *parent);
};

#endif // PARAMETERSITEM_H
