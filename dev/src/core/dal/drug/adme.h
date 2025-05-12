/* 
 * Tucuxi - Tucuxi-gui software. 
 * This software is able to perform prediction of drug concentration in blood 
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


#ifndef ADME_H
#define ADME_H

#include "core/dal/entity.h"
#include "core/utils/ezutils.h"
#include <QMetaEnum>

#include "tucucore/drugmodel/formulationandroute.h"

Q_MOC_INCLUDE("core/dal/drug/translatablestring.h")

namespace Tucuxi {
namespace Gui {
namespace Core {


class CommentSet;
class TranslatableString;



class DMAdmin : public Entity
{
    Q_OBJECT
    ENTITY_UTILS(DMAdmin)
    Q_PROPERTY(Route route READ getRoute WRITE setRoute NOTIFY routeChanged)

    Q_PROPERTY(QString label READ getLabel WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(int value READ getValue WRITE setValue NOTIFY valueChanged)

    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
public:
    enum Route {
        BOLUS = 0, INFUSION, EXTRA, EXTRALAG, DEFAULT, UNVALID
    };
    Q_ENUM(Route)

    Q_INVOKABLE DMAdmin(AbstractRepository *repository, QObject *parent = nullptr)
        : Entity(repository, parent),
        _route(EXTRA),
        _formulationAndRoute(Tucuxi::Core::AbsorptionModel::Undefined)
    {
    }

    Q_INVOKABLE DMAdmin(AbstractRepository *repository, QObject *parent, DMAdmin* admin)
        : Entity(repository, parent),
        _route(admin->getRoute()),
        _formulationAndRoute(Tucuxi::Core::AbsorptionModel::Undefined)
    {
        setFormulationAndRoute(admin->_formulationAndRoute);
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
        case DEFAULT: return "Default route";
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

    Tucuxi::Core::DMFormulationAndRoute getFormulationAndRoute() const { return _formulationAndRoute;}
    void setFormulationAndRoute(Tucuxi::Core::DMFormulationAndRoute formulationAndRoute);

    Q_INVOKABLE QString getAdministrationName() const { return QString::fromStdString(_formulationAndRoute.getAdministrationName());}
    Q_INVOKABLE QString getFormulationString() const {
        static std::map<Tucuxi::Core::Formulation, std::string> map = {
            {Tucuxi::Core::Formulation::Undefined, "undefined"},
            {Tucuxi::Core::Formulation::ParenteralSolution,"parenteral solution"},
            {Tucuxi::Core::Formulation::OralSolution, "oral solution"},
            {Tucuxi::Core::Formulation::Test, "test"}
        };
        return QString::fromStdString(map.at(_formulationAndRoute.getFormulation()));
    }

    Q_INVOKABLE QString getAdministrationRoute() const {

        static std::map<Tucuxi::Core::AdministrationRoute, std::string> m =
            {
                {Tucuxi::Core::AdministrationRoute::Undefined, "undefined"},
                {Tucuxi::Core::AdministrationRoute::Intramuscular, "intramuscular"},
                {Tucuxi::Core::AdministrationRoute::IntravenousBolus, "intravenous bolus"},
                {Tucuxi::Core::AdministrationRoute::IntravenousDrip, "intravenous drip"},
                {Tucuxi::Core::AdministrationRoute::Nasal, "nasal"},
                {Tucuxi::Core::AdministrationRoute::Oral, "oral"},
                {Tucuxi::Core::AdministrationRoute::Rectal, "rectal"},
                {Tucuxi::Core::AdministrationRoute::Subcutaneous, "subcutaneous"},
                {Tucuxi::Core::AdministrationRoute::Sublingual, "sublingual"},
                {Tucuxi::Core::AdministrationRoute::Transdermal, "transdermal"},
                {Tucuxi::Core::AdministrationRoute::Vaginal, "vaginal"}
            };
        return QString::fromStdString(m.at(_formulationAndRoute.getAdministrationRoute()));
    }

private:
    Route _route;
    QString _description;

    Tucuxi::Core::DMFormulationAndRoute _formulationAndRoute;
};

QML_POINTERLIST_CLASS_DECL(DMAdminList, DMAdmin)


class Admin : public Entity
{
    Q_OBJECT
    ENTITY_UTILS(Admin)
    // Q_PROPERTY(DMAdmin::Route route READ getRoute WRITE setRoute NOTIFY routeChanged)

    //Q_PROPERTY(QString label READ getLabel WRITE setLabel NOTIFY labelChanged)
    //Q_PROPERTY(int value READ getValue WRITE setValue NOTIFY valueChanged)

    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
    public:

    Q_INVOKABLE Admin(AbstractRepository *repository, QObject *parent = nullptr)
        : Entity(repository, parent),
            //_route(DMAdmin::EXTRA),
          _formulationAndRoute()
    {
    }

    Q_INVOKABLE Admin(AbstractRepository *repository, QObject *parent, Admin* admin)
        : Entity(repository, parent),
          //_route(admin->getRoute()),
          _formulationAndRoute()
    {
        setFormulationAndRoute(admin->_formulationAndRoute);
    }
/*
    QString getLabel () const {
        return QMetaEnum::fromType<DMAdmin::Route>().key(_route);
    }
    void setLabel(QString label) {
        QMetaEnum menum = QMetaEnum::fromType<DMAdmin::Route>();
        _route = static_cast<DMAdmin::Route>(menum.keyToValue(&label.toStdString()[0]));
        emit valueChanged(menum.keyToValue(menum.key(_route)));
        emit labelChanged(menum.key(_route));
    }

    int getValue () const {
        QMetaEnum menum = QMetaEnum::fromType<DMAdmin::Route>();
        return menum.keyToValue(menum.key(_route));
    }

    void setValue(int value) {
        DMAdmin::Route newRoute = static_cast<DMAdmin::Route>(value);
        if (newRoute != _route) {
            _route = newRoute;
            QMetaEnum menum = QMetaEnum::fromType<DMAdmin::Route>();
            emit valueChanged(value);
            //                emit valueChanged(menum.keyToValue(menum.key(_route)));
            emit labelChanged(menum.key(_route));
        }
    }
*/
    void setDescription(QString description) { _description = description; emit descriptionChanged(description);}
    QString getDescription() const {return _description;}

//    void setRoute(DMAdmin::Route route) { _route = route; emit routeChanged(_route); }
//    DMAdmin::Route getRoute() const { return _route; }
    Q_SIGNAL void labelChanged (QString value);
//    Q_SIGNAL void valueChanged (int value);
//    Q_SIGNAL void routeChanged (DMAdmin::Route route);
    Q_SIGNAL void descriptionChanged(QString description);

    // bool isValid() { return _route != DMAdmin::Route::UNVALID;}
    bool isValid() { return _formulationAndRoute.getAdministrationRoute() != Tucuxi::Core::AdministrationRoute::Undefined;}

    Tucuxi::Core::FormulationAndRoute getFormulationAndRoute() const { return _formulationAndRoute;}
    void setFormulationAndRoute(Tucuxi::Core::FormulationAndRoute formulationAndRoute);

    Q_INVOKABLE QString getAdministrationName() const { return QString::fromStdString(_formulationAndRoute.getAdministrationName());}
    Q_INVOKABLE QString getFormulationString() const {
        static std::map<Tucuxi::Core::Formulation, std::string> map = {
            {Tucuxi::Core::Formulation::Undefined, "undefined"},
            {Tucuxi::Core::Formulation::ParenteralSolution,"parenteral solution"},
            {Tucuxi::Core::Formulation::OralSolution, "oral solution"},
            {Tucuxi::Core::Formulation::Test, "test"}
        };
        return QString::fromStdString(map.at(_formulationAndRoute.getFormulation()));
    }

    Q_INVOKABLE QString getAdministrationRoute() const {

        static std::map<Tucuxi::Core::AdministrationRoute, std::string> m =
        {
            {Tucuxi::Core::AdministrationRoute::Undefined, "undefined"},
            {Tucuxi::Core::AdministrationRoute::Intramuscular, "intramuscular"},
            {Tucuxi::Core::AdministrationRoute::IntravenousBolus, "intravenous bolus"},
            {Tucuxi::Core::AdministrationRoute::IntravenousDrip, "intravenous drip"},
            {Tucuxi::Core::AdministrationRoute::Nasal, "nasal"},
            {Tucuxi::Core::AdministrationRoute::Oral, "oral"},
            {Tucuxi::Core::AdministrationRoute::Rectal, "rectal"},
            {Tucuxi::Core::AdministrationRoute::Subcutaneous, "subcutaneous"},
            {Tucuxi::Core::AdministrationRoute::Sublingual, "sublingual"},
            {Tucuxi::Core::AdministrationRoute::Transdermal, "transdermal"},
            {Tucuxi::Core::AdministrationRoute::Vaginal, "vaginal"}
        };
        return QString::fromStdString(m.at(_formulationAndRoute.getAdministrationRoute()));
    }

private:
    // DMAdmin::Route _route;
    QString _description;

    Tucuxi::Core::FormulationAndRoute _formulationAndRoute;
};

QML_POINTERLIST_CLASS_DECL(AdminList, Admin)






} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::Admin*)
//Q_DECLARE_METATYPE(Tucuxi::Gui::Core::Admin::Route)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::Admin*>)

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::DMAdmin*)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::DMAdmin::Route)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::DMAdmin*>)

namespace Tucuxi {
namespace Gui {
namespace Core {


class ADME : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(ADME)

    public:

        AUTO_PROPERTY_DECL(DMAdmin*, defaultIntake, DefaultIntake)
      AUTO_PROPERTY_DECL(QString, distribution, Distribution)
      AUTO_PROPERTY_DECL(QString, elimination, Elimination)
      AUTO_PROPERTY_DECL(DMAdminList*, intakes, Intakes)

      AUTO_PROPERTY_DECL(TranslatableString*, comments, Comments)

      public:

      //    static QString toString(Route route);

      //    Q_INVOKABLE QString routeToString(QVariant route) {return toString(Route(route.toInt()));}

      protected:

      Q_INVOKABLE ADME(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent),
        _defaultIntake(CoreFactory::createEntity<DMAdmin>(repository, this)), _intakes(nullptr), _comments(nullptr) {}
    //    Q_INVOKABLE ADME(AbstractRepository *repository, QObject * parent, const QString &intake, const QString &distribution, const QString &elimination)
    //        : Entity(repository, parent), _intake(intake), _distribution(distribution), _elimination(elimination) {}
public:
    bool isValid();

};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::ADME*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::ADME*>)
#endif // ADME_H
