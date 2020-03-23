#include "covariateitem.h"
#include "covariateparser.h"

//Constructor
CovariateItem::CovariateItem(QDomElement node, DomItem *parent) : OuterItem(node, parent)
{

}

//Return the name of the covariate
QString CovariateItem::name() const
{
    //Get, verify and return the covariate name
    QString name = CovariateParser(node()).name();

    if (!name.isEmpty())
        return name;

    //If it's empty, return the default tag name
    return OuterItem::name();
}

//Return a covariate parser class name
QByteArray CovariateItem::parserType() const
{
    return CovariateParser::staticMetaObject.className();
}
