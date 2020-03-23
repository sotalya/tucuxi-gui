#include "covariatesitem.h"
#include "covariatesparser.h"

//Constructor
CovariatesItem::CovariatesItem(QDomElement node, DomItem *parent) : InnerItem(node, parent)
{

}

//Returns a covariates parser class name
QByteArray CovariatesItem::parserType() const
{
    return CovariatesParser::staticMetaObject.className();
}
