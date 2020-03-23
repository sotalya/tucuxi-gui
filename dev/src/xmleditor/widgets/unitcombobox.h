#ifndef UNITCOMBOBOX_H
#define UNITCOMBOBOX_H

#include <QComboBox>

class UnitComboBox : public QComboBox
{

public:

    /** \brief Default constructor.
     * @param parent An optional parent widget.
     */
    UnitComboBox(QWidget *parent = 0);

    /** \brief Sets the current unit, if any, as the current item.
     * @param unit The current unit.
     */
    void setCurrentUnit(const QString &unit);

    //! Cleans the combobox, removing any custom units.
    void clean();

private:
    //The number of default items
    int _count;
};

#endif // UNITCOMBOBOX_H
