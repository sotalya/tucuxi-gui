#include "unitcombobox.h"

//Default constructor
UnitComboBox::UnitComboBox(QWidget *parent) : QComboBox(parent)
{
    //Set the combobox as editable
    setEditable(true);
    setInsertPolicy(QComboBox::NoInsert);

    //Allow only letter and some special caracters
    QRegExp unitRegExp("^[a-zA-Z0-9/%&#!-]+$");
    QValidator *validator = new QRegExpValidator(unitRegExp, this);
    setValidator(validator);

    //Init and set the default and common units
    QStringList units;
    units << "ng" << "ug" << "mg" << "g" << "kg";
    units << "ng/l" << "ug/l" << "mg/l" << "g/l";
    addItems(units);

    //Store the number of default items (units)
    _count = count();
}

//Sets the current unit
void UnitComboBox::setCurrentUnit(const QString &unit)
{
    //Search for the current unit's index
    int index = findText(unit);

    //If not found and not empty, add it
    if (index == -1 && !unit.isEmpty())
        insertItem(index = 0, unit);

    //Set the current index
    setCurrentIndex(index);
}

//Cleans the combobox
void UnitComboBox::clean()
{
    //Remove the custom units if any
    while (_count < count())
        removeItem(0);

    //Clear the combobox line edit
    setCurrentIndex(-1);
}
