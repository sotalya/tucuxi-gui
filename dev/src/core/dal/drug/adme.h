#ifndef ADME_H
#define ADME_H

#include "core/dal/entity.h"
#include "core/utils/ezutils.h"
#include <QMetaEnum>

namespace ezechiel {
namespace core {

class CommentSet;
class TranslatableString;

class Admin : public Entity
{
    Q_OBJECT
    ENTITY_UTILS(Admin)
    Q_PROPERTY(Route route READ getRoute WRITE setRoute NOTIFY routeChanged)

    Q_PROPERTY(QString label READ getLabel WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(int value READ getValue WRITE setValue NOTIFY valueChanged)

    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
    public:
        enum Route {
                 BOLUS = 0, INFUSION, EXTRA, EXTRALAG, DEFAULT, UNVALID
        };
    Q_ENUM(Route)

    Q_INVOKABLE Admin(AbstractRepository *repository, QObject *parent = nullptr)
        : Entity(repository, parent),
          _route(EXTRA)
    {
    }

    Q_INVOKABLE Admin(AbstractRepository *repository, QObject *parent, Admin* admin)
        : Entity(repository, parent),
          _route(admin->getRoute())
    {
    }

    QString getLabel () const {
        return QMetaEnum::fromType<Route>().key(_route);
    }
    void setLabel(QString label) {
        QMetaEnum menum = QMetaEnum::fromType<Route>();
        _route = static_cast<Route>(menum.keyToValue(&label.toStdString()[0]));
        emit valueChanged(menum.keyToValue(menum.key(_route)));
        emit labelChanged(menum.key(_route));
    }

    QString getLabelString() const {
        switch (_route) {
        case BOLUS : return "Bolus";
        case INFUSION: return "Infusion";
        case EXTRA: return "Extravascular";
        case EXTRALAG: return "Extravascular with lag time";
        case UNVALID: return "Invalid";
        }
    }

    int getValue () const {
        QMetaEnum menum = QMetaEnum::fromType<Route>();
        return menum.keyToValue(menum.key(_route));
    }

    void setValue(int value) {
        Route newRoute = static_cast<Route>(value);
        if (newRoute != _route) {
            _route = newRoute;
            QMetaEnum menum = QMetaEnum::fromType<Route>();
            emit valueChanged(value);
            //                emit valueChanged(menum.keyToValue(menum.key(_route)));
            emit labelChanged(menum.key(_route));
        }
    }

    void setDescription(QString description) { _description = description; emit descriptionChanged(description);}
    QString getDescription() const {return _description;}

    void setRoute(Route route) { _route = route; emit routeChanged(_route); }
    Route getRoute() const { return _route; }
    Q_SIGNAL void labelChanged (QString value);
    Q_SIGNAL void valueChanged (int value);
    Q_SIGNAL void routeChanged (Route route);
    Q_SIGNAL void descriptionChanged(QString description);

    bool isValid() { return _route != Route::UNVALID;}
private:
    Route _route;
    QString _description;
};

QML_POINTERLIST_CLASS_DECL(AdminList, Admin)

} // namespace core
} // namespace ezechiel

Q_DECLARE_METATYPE(ezechiel::core::Admin*)
Q_DECLARE_METATYPE(ezechiel::core::Admin::Route)
Q_DECLARE_METATYPE(QList<ezechiel::core::Admin*>)

namespace ezechiel {
namespace core {

class ADME : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(ADME)

    public:

        AUTO_PROPERTY_DECL(Admin*, defaultIntake, DefaultIntake)
      AUTO_PROPERTY_DECL(QString, distribution, Distribution)
      AUTO_PROPERTY_DECL(QString, elimination, Elimination)
      AUTO_PROPERTY_DECL(AdminList*, intakes, Intakes)

      AUTO_PROPERTY_DECL(TranslatableString*, comments, Comments)

      public:

      //    static QString toString(Route route);

      //    Q_INVOKABLE QString routeToString(QVariant route) {return toString(Route(route.toInt()));}

      protected:

      Q_INVOKABLE ADME(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent),
        _defaultIntake(CoreFactory::createEntity<Admin>(repository, this)), _intakes(nullptr), _comments(nullptr) {}
    //    Q_INVOKABLE ADME(AbstractRepository *repository, QObject * parent, const QString &intake, const QString &distribution, const QString &elimination)
    //        : Entity(repository, parent), _intake(intake), _distribution(distribution), _elimination(elimination) {}
public:
    bool isValid();

};

} // namespace core
} // namespace ezechiel

Q_DECLARE_METATYPE(ezechiel::core::ADME*)
Q_DECLARE_METATYPE(QList<ezechiel::core::ADME*>)
#endif // ADME_H
