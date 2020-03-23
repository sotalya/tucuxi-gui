#include "dosageitem.h"
#include "dosageparser.h"
#include "core.h"
#include "definitionmanager.h"

//Constructor
DosageItem::DosageItem(QDomElement node, DomItem *parent) : OuterItem(node, parent)
{

}

//Returns the dosage name
QString DosageItem::name() const
{
    //Get and return the intake name
    QString name = CORE->definitionManager()->intake(DosageParser(node()).intakeId()).name;

    if (!name.isEmpty())
        return name;

    //If it's empty, return the default tag name
    return OuterItem::name();
}

//Returns a dosage parser class name
QByteArray DosageItem::parserType() const
{
    return DosageParser::staticMetaObject.className();
}
