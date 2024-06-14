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

