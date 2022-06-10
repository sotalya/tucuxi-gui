//@@license@@

#ifndef COMMENTS_H
#define COMMENTS_H

#include "core/dal/entity.h"
#include "core/core.h"
#include <string>

namespace Tucuxi {
namespace GuiCore {

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
} // namespace core
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::GuiCore::TranslatableString*)
//Q_DECLARE_METATYPE(Tucuxi::GuiCore::TranslatableString)
//Q_DECLARE_METATYPE(QList<Tucuxi::GuiCore::TranslatableString*>)


#endif // COMMENTS_H
