#include "ezchoice.h"

//#include "coremessagehandler.h"
#include "core/core.h"
#include "core/utils/logging.h"
#include "core/errors_core.h"

namespace ezechiel {
namespace core {


EzChoice::EzChoice(QObject *parent) :
   QObject(parent),
   _selected(0)
{
}

//Simple data copy
EzChoice::EzChoice(const EzChoice &other) :
   QObject()
{
   _list = other._list;
   _selected = other._selected;
}

//---------------- GETTERS ------------------

Descriptor EzChoice::choice(const QString &id) const
{
   foreach (Descriptor d, _list)
      if (d.id == id)
         return d;

   LOG(QtWarningMsg, NOEZERROR, tr("ID not found in choices : %1").arg(id));
   return Descriptor();
}

Descriptor EzChoice::choice(uint place) const
{
   return _list.at(place);
}

QList<Descriptor> EzChoice::choices() const
{
   return _list;
}

Descriptor EzChoice::selected() const
{
   return _list.at(_selected);
}

uint EzChoice::selectedAt() const
{
   return _selected;
}

QString EzChoice::selectedId() const
{
   return _list.at(_selected).id;
}

bool EzChoice::isEmpty() const
{
   return _list.isEmpty();
}

//---------------- SETTERS ------------------

uint EzChoice::addChoice(const Descriptor &d)
{
   _list.append(d);
   emit changed();
   return _list.size()-1;
}

//Must search in the list (baaah, not THAT bad!)
bool EzChoice::removeChoice(const QString &id)
{
   for (int i=0; i<_list.size(); i++)
      if (_list.at(i).id == id) {
         _list.removeAt(i);
         emit changed();
         return true;
      }

   LOG(QtWarningMsg, NOEZERROR, tr("ID '%1' not found in choices when deleting").arg(id));
   return false;
}

void EzChoice::removeChoice(uint place)
{
   _list.removeAt(place);
   emit changed();
}

void EzChoice::selectAt(uint place)
{
   _selected = place;
   emit selected(_list.at(place).id);
   emit selected(place);
}

//Again, search in the list
void EzChoice::select(const QString &id)
{
   for (int i=0; i<_list.size(); i++)
      if (_list.at(i).id == id) {
         _selected = i;
         emit selected(_list.at(i).id);
         emit selected(i);
         return;
      }

   LOG(QtWarningMsg, NOEZERROR, tr("ID '%1' not found in choices when selecting").arg(id));
}

void EzChoice::clear()
{
   _list.clear();
   _selected = 0;
   emit changed();
}

} // namespace core
} // namespace ezechiel

