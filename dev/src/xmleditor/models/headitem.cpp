#include "headitem.h"
#include "drugheadparser.h"
#include "dispositionheadparser.h"
#include "intakeheadparser.h"
#include "comethheadparser.h"
#include "xmleditormessagehandler.h"
#include "core_errors.h"

//Constructor
HeadItem::HeadItem(QDomElement node, DomItem *parent) : OuterItem(node, parent)
{

}

//Returns the corresponding head parser class name
QByteArray HeadItem::parserType() const
{
    //A parent should always exist
    if (parent() == 0)
        ERROR(UNINITIALISED, "The parent item of the header is missing");

    //Get the parent tag type
    DrugTag::Tag tag = Tag::drugTag(parent()->node().tagName());

    //Return the corresponding header
    if (tag == DrugTag::Drug)
        return DrugHeadParser::staticMetaObject.className();
    if (tag == DrugTag::Disposition)
        return DispositionHeadParser::staticMetaObject.className();
    if (tag == DrugTag::Intake)
        return IntakeHeadParser::staticMetaObject.className();
    if (tag == DrugTag::Cometh)
        return ComethHeadParser::staticMetaObject.className();

    //A parent should always be a model
    ERROR(DATAERROR, "The parent item of the header is invalid"); return 0;
}
