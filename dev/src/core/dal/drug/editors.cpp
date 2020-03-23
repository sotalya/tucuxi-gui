#include "core/dal/drug/editors.h"

namespace ezechiel {
namespace core {

AUTO_PROPERTY_IMPL(Editor,QString,name,Name)
AUTO_PROPERTY_IMPL(Editor,QString, institution,Institution)
AUTO_PROPERTY_IMPL(Editor,QString, email, Email)
AUTO_PROPERTY_IMPL(Editor,QDateTime,date,Date)

AUTO_PROPERTY_IMPL(Editor,TranslatableString*,comments,Comments)

QML_POINTERLIST_CLASS_IMPL(EditorList,Editor)

/*
// Because an EditorSet is not an Entity but a QList we need to delete manually the objects
// YTA : I think we could use EditorList instead of EditorSet
EditorSet::~EditorSet()
{
    while (!this->isEmpty()) {
        Editor *e = this->at(this->size()-1);
        this->pop_back();
        delete e;
    }
}
*/

} // namespace core
} // namespace ezechiel



