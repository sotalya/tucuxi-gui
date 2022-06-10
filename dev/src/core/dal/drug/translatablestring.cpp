//@@license@@

#include "core/dal/drug/translatablestring.h"





namespace Tucuxi {
namespace Gui {
namespace Core {


typedef QMap<QString,QString> QStringTranslation;

//AUTO_PROPERTY_IMPL(TranslatableString,QStringTranslation,transMap,TransMap)

//TranslatableString::TranslatableString(TranslatableString& transString):
//_transMap(transString)
//{

//}

QString TranslatableString::defaultLang()
{
    return value();
}

std::string TranslatableString::toStdString()
{
    return defaultLang().toStdString();
}


QString TranslatableString::value(QString lang)
{
    //TODO ASSERTS
//return    QMap<QString,QString>::value(lang,"");
return    QMap<QString,QString>::value("en","");
}
void TranslatableString::insert(const QString &value)
{
    QMap<QString,QString>::insert(CORE->language(),value);
    emit keysChanged(getKeys());
}
void TranslatableString::insert(const QString &key, const QString &value)
{
    QMap<QString,QString>::insert(key,value);
    emit keysChanged(getKeys());
}
QStringList& TranslatableString::getKeys()
{
    _keys = QMap<QString,QString>::keys();
     return _keys;
}
void TranslatableString::setKeys(  QList<QString> value)
{

}

bool TranslatableString::isEmpty()
{
    return QMap<QString,QString>::isEmpty();
}

bool TranslatableString::operator==(QString &notTransString)
{
    return value() == notTransString;
}

bool TranslatableString::operator !=(QString &notTransString)
{
    return !operator ==(notTransString);
}

bool TranslatableString::operator==(const QString &notTransString){
    return value() == notTransString;
}

QML_POINTERLIST_CLASS_IMPL(TranslatableStringList,TranslatableString)
} // namespace Core
} // namespace Gui
} // namespace Tucuxi

