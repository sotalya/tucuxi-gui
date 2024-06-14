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


#ifndef COMMENTS_H
#define COMMENTS_H

#include "core/dal/entity.h"
#include "core/core.h"
#include <string>

namespace Tucuxi {
namespace Gui {
namespace Core {


class TranslatableString : public Entity, public QMap<QString,QString>
{
    Q_OBJECT

    ENTITY_UTILS(TranslatableString)

//    AUTO_PROPERTY_DECL(QList<QString>, keys, Keys)
    Q_PROPERTY(QStringList keys READ getKeys WRITE setKeys NOTIFY keysChanged)
    //TODO take care of .keys()
    //TODO take care of .Values()
    //TODO use the inherited QMap (or at least make sure it returns usual functions properly)
    //TODO a method call toStdString()
    //TODO a method returning default value


//    typedef QMap<QString,QString> QStringTranslation;

    ///note to self: use CORE->language() for default language

//    AUTO_PROPERTY_DECL(QStringTranslation,transMap,TransMap)

    protected:

        Q_INVOKABLE explicit TranslatableString(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent){}
//    Q_INVOKABLE explicit TranslatableString(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent) {}

public:
    //    TranslatableString(TranslatableString& transString);



    void setKeys(QList<QString>);
    QStringList& getKeys();
    QString defaultLang();
    std::string toStdString();
    Q_INVOKABLE QString at(const int index) {return keys().at(index);}
    Q_INVOKABLE QString value(QString lang = CORE->language());
    Q_INVOKABLE void insert(const QString &value);
    Q_INVOKABLE void insert(const QString &key, const QString &value);
//    Q_INVOKABLE QList<QString> getKeys();
    Q_INVOKABLE int size() { return QMap<QString,QString>::size();}
    bool isEmpty();
    bool operator==(QString &notTransString);
    bool operator !=(QString &notTransString);
    bool operator==(const QString &notTransString);
signals:
    void keysChanged(QList<QString>);

private:
    QStringList _keys;

    //    friend bool operator==(TranslatableString* me,QString notTransString);
    //    friend bool operator==(QString notTransString, TranslatableString* me);
};

QML_POINTERLIST_CLASS_DECL(TranslatableStringList, TranslatableString)
} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::TranslatableString*)
//Q_DECLARE_METATYPE(Tucuxi::Gui::Core::TranslatableString)
//Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::TranslatableString*>)


#endif // COMMENTS_H
