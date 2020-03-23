#include "intakeitem.h"
#include "core.h"
#include "definitionmanager.h"
#include "intakeparser.h"
#include "intakeheadparser.h"

//Constructor
IntakeItem::IntakeItem(QDomElement node, DomItem *parent) : InnerItem(node, parent)
{

}

//Returns the intake name
QString IntakeItem::name() const
{
    //Get a parser for the head
    IntakeHeadParser head(IntakeParser(node()).headNode());

    //Get and return the intake name
    QString name = CORE->definitionManager()->intake(head.intakeId()).name;

    if (!name.isEmpty())
        return name;

    //If it's empty, return the ID
    return head.id();
}

//Returns an intake parser class name
 QByteArray IntakeItem::parserType() const
{
    return IntakeParser::staticMetaObject.className();
}
