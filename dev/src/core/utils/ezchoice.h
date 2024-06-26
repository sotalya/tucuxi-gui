/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef EZCHOICE_H
#define EZCHOICE_H

#include <QObject>
#include <QString>
#include <QMetaType>

#include "core/utils/ezutils.h"

namespace Tucuxi {
namespace Gui {
namespace Core {



/** Represent a multi-element choice.
 * \ingroup utils
 * As no GUI objects are allowed in the \ref core this object replace a combo-box style list.
 * A list of choice, each choice (or element) being represented by a descriptor is stored.
 * One of them can be selected, and then retrieved.
 * The order in which the choices are inserted is saved, and can be laterused to retrieve them
 */
class EzChoice : public QObject
{
   Q_OBJECT
public:

   EzChoice(QObject *parent = nullptr);
   EzChoice(const EzChoice &other);

   //---------------- GETTERS ------------------

   /** Fetch a choice from its ID.
    * @param id The ID (in the Descriptor) of the wanted choice
    * @return The full Descriptor for the wanted choice, or an empty one if not found
    * \sa choice(uint)
    */
   Descriptor choice (const QString &id) const;

   /** Fetch a choice from its place.
    * @param place The choice's place
    * @return The full Descriptor for the wanted choice, or an empty one if not found
    * \sa choice(QString)
    */
   Descriptor choice (uint place) const;

   /** The list of choices.
    * @return The full list of choices
    */
   QList<Descriptor> choices () const;

   /** The currently selected choice.
    * @return The selected Descriptor
    * \sa selectedAt selectedId selectAt select
    */
   Descriptor selected () const;

   /** The currently selected choice's place.
    * @return The palce of the currently selected choice
    * \sa selected selectedId selectAt select
    */
   uint selectedAt () const;

   /** The currently selected choice's ID.
    * @return The Descriptor ID of the currently selected choice
    * \sa selected selectedAt selectAt select
    */
   QString selectedId () const;

   //! Is this EzChoice empty?
   bool isEmpty () const;

public slots:

   //---------------- SETTERS ------------------

   /** Add a new choice.
    * @param d The Descriptor representing the choice to add
    * @return The place of the newly inserted choice
    * \sa removeChoice(QString) removeChoice(uint)
    */
   uint addChoice (const Descriptor &d);

   /** Remove a choice from the list.
    * @param id The Descriptor ID of the choice to remove
    * @return True if the ID was found, false otherwise
    * \sa removeChoice(uint) addChoice
    */
   bool removeChoice (const QString &id);

   /** Remove a choice from the list using its place.
    * @param place The choice's place to remove
    * \sa removeChoice(QString) addChoice
    */
   void removeChoice(uint place);

   /** Select the choice at this place.
    * @param place The place of the choice to select
    * \sa select selected selectedAt selectedId
    */
   void selectAt (uint place);

   /** Select the choice with this ID.
    * @param id The choice's ID to select
    * \sa selectAt selected selectedAt selectedId
    */
   void select (const QString &id);

   //! Remove all the choices in the EzCHoice.
   void clear ();
   
signals:
   
   //! Emitted when the choice changed (choice added/removed/selected).
   void changed ();

   /** A choice has been selected.
    * @param id The selected choice's ID
    */
   void selected (QString id);

   /** A choice has been selected.
    * @param place The place of the selected choice
    */
   void selected (uint place);

private:
   QList<Descriptor> _list;
   uint _selected;
   
};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::EzChoice)

#endif // EZCHOICE_H
