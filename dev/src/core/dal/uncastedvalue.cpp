//@@license@@

#include "core/dal/uncastedvalue.h"

namespace Tucuxi {
namespace GuiCore {


Q_INVOKABLE UncastedValue::UncastedValue(Tucuxi::GuiCore::AbstractRepository *repository, QObject *parent)
    : Entity(repository, parent), _field(""), _text(""), _comment(""), _status(UncastedStatus::Uncasted), _validated(false)
{

}

bool UncastedValue::isValid()
{
    return true;
}

AUTO_PROPERTY_IMPL(UncastedValue, QString, field, Field)
AUTO_PROPERTY_IMPL(UncastedValue, QString, text, Text)
AUTO_PROPERTY_IMPL(UncastedValue, QString, comment, Comment)
AUTO_PROPERTY_IMPL(UncastedValue, UncastedStatus, status, Status)
AUTO_PROPERTY_IMPL(UncastedValue, bool, validated, Validated)


QML_POINTERLIST_CLASS_IMPL(UncastedValueList, UncastedValue)


} // namespace core
} // namespace Tucuxi
