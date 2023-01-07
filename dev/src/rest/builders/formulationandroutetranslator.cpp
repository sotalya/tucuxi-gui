//@@license@@

#include "formulationandroutetranslator.h"
#include <QSettings>

namespace Tucuxi {
namespace Gui {
namespace Rest {

Tucuxi::Core::Formulation extractFormulation(QString s)
{
    static std::map<std::string, Tucuxi::Core::Formulation> m =
    {
        {"undefined", Tucuxi::Core::Formulation::Undefined},
        {"parenteral solution", Tucuxi::Core::Formulation::ParenteralSolution},
        {"oral solution", Tucuxi::Core::Formulation::OralSolution},
        {"parenteralSolution", Tucuxi::Core::Formulation::ParenteralSolution},
        {"oralSolution", Tucuxi::Core::Formulation::OralSolution},
        {"test", Tucuxi::Core::Formulation::Test}
    };

    auto it = m.find(s.toStdString());
    if (it != m.end()) {
        return it->second;
    }
    return Tucuxi::Core::Formulation::Undefined;
}

Tucuxi::Core::AdministrationRoute extractAdministrationRoute(QString s)
{
    static std::map<std::string, Tucuxi::Core::AdministrationRoute> m =
    {
        {"undefined", Tucuxi::Core::AdministrationRoute::Undefined},
        {"intramuscular", Tucuxi::Core::AdministrationRoute::Intramuscular},
        {"intravenousBolus", Tucuxi::Core::AdministrationRoute::IntravenousBolus},
        {"intravenousDrip", Tucuxi::Core::AdministrationRoute::IntravenousDrip},
        {"nasal", Tucuxi::Core::AdministrationRoute::Nasal},
        {"oral", Tucuxi::Core::AdministrationRoute::Oral},
        {"rectal", Tucuxi::Core::AdministrationRoute::Rectal},
        {"subcutaneous", Tucuxi::Core::AdministrationRoute::Subcutaneous},
        {"sublingual", Tucuxi::Core::AdministrationRoute::Sublingual},
        {"transdermal", Tucuxi::Core::AdministrationRoute::Transdermal},
        {"vaginal", Tucuxi::Core::AdministrationRoute::Vaginal}
    };

    auto it = m.find(s.toStdString());
    if (it != m.end()) {
        return it->second;
    }
    return Tucuxi::Core::AdministrationRoute::Undefined;
}

Tucuxi::Core::AbsorptionModel extractAbsorptionModel(QString s)
{

    static std::map<std::string, Tucuxi::Core::AbsorptionModel> m =
    {
        {"undefined", Tucuxi::Core::AbsorptionModel::Undefined},
        {"bolus", Tucuxi::Core::AbsorptionModel::Intravascular},
        {"extra", Tucuxi::Core::AbsorptionModel::Extravascular},
        {"extra.lag", Tucuxi::Core::AbsorptionModel::ExtravascularLag},
        {"infusion", Tucuxi::Core::AbsorptionModel::Infusion}
    };

    auto it = m.find(s.toStdString());
    if (it != m.end()) {
        return it->second;
    }

    return Tucuxi::Core::AbsorptionModel::Undefined;
}

FormulationAndRouteTranslator::FormulationAndRouteTranslator()
{

}

FormulationAndRouteTranslator::~FormulationAndRouteTranslator()
{

}

ChuvFormulationAndRouteTranslator::ChuvFormulationAndRouteTranslator()
{
    {
        Tucuxi::Core::FormulationAndRoute formulationAndRoute(
                Tucuxi::Core::Formulation::ParenteralSolution,
                Tucuxi::Core::AdministrationRoute::IntravenousDrip,
                Tucuxi::Core::AbsorptionModel::Infusion,
                "");

        mapIntake.insert("i.v.", formulationAndRoute);
        map.insert("cefepime", mapIntake);
    }

    {
        Tucuxi::Core::FormulationAndRoute formulationAndRoute(
                Tucuxi::Core::Formulation::ParenteralSolution,
                Tucuxi::Core::AdministrationRoute::Oral,
                Tucuxi::Core::AbsorptionModel::Extravascular,
                "");

        mapIntake.insert("p.o", formulationAndRoute);
        map.insert("tacrolimus", mapIntake);
    }

    {
        Tucuxi::Core::FormulationAndRoute formulationAndRoute(
                Tucuxi::Core::Formulation::ParenteralSolution,
                Tucuxi::Core::AdministrationRoute::IntravenousDrip,
                Tucuxi::Core::AbsorptionModel::Infusion,
                "");

        mapIntake.insert("injection iv", formulationAndRoute);
        map.insert("vancomycin", mapIntake);
    }

    {
        Tucuxi::Core::FormulationAndRoute formulationAndRoute(
                Tucuxi::Core::Formulation::ParenteralSolution,
                Tucuxi::Core::AdministrationRoute::IntravenousDrip,
                Tucuxi::Core::AbsorptionModel::Infusion,
                "");

        mapIntake.insert("i.v.", formulationAndRoute);
        map.insert("vancomycin", mapIntake);
    }

    {
        Tucuxi::Core::FormulationAndRoute formulationAndRoute(
                Tucuxi::Core::Formulation::ParenteralSolution,
                Tucuxi::Core::AdministrationRoute::IntravenousDrip,
                Tucuxi::Core::AbsorptionModel::Infusion,
                "");

        mapIntake.insert("manque", formulationAndRoute);
        map.insert("vancomycin", mapIntake);
    }
}

Tucuxi::Core::FormulationAndRoute ChuvFormulationAndRouteTranslator::restToInternalFormulationAndRoute(QString drugId, QString intake)
{
    Tucuxi::Core::FormulationAndRoute formulationAndRoute(
                Tucuxi::Core::Formulation::Undefined,
                Tucuxi::Core::AdministrationRoute::Undefined,
                Tucuxi::Core::AbsorptionModel::Undefined,
                "");

    mapIntake = map.value(drugId);
    return mapIntake.value(intake, formulationAndRoute);
}

ExternalFormulationAndRouteTranslator::ExternalFormulationAndRouteTranslator()
{

}

void ExternalFormulationAndRouteTranslator::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}

Tucuxi::Core::FormulationAndRoute ExternalFormulationAndRouteTranslator::restToInternalFormulationAndRoute(QString drugId, QString intake)
{
    QSettings settingsFile(m_fileName, QSettings::IniFormat);

    Tucuxi::Core::Formulation formulation = Tucuxi::Core::Formulation::Undefined;
    Tucuxi::Core::AdministrationRoute administrationRoute = Tucuxi::Core::AdministrationRoute::Undefined;
    Tucuxi::Core::AbsorptionModel absorptionModel = Tucuxi::Core::AbsorptionModel::Undefined;
    std::string administrationName = "";

    settingsFile.beginGroup(drugId + "/" + intake);

    QStringList childKeys = settingsFile.childKeys();

    foreach (const QString &childKey, childKeys) {
        if (childKey == "formulation")
            formulation = extractFormulation(settingsFile.value(childKey).toString());
        else if (childKey == "administrationName")
            administrationName = settingsFile.value(childKey).toString().toStdString();
        else if (childKey == "administrationRoute")
            administrationRoute = extractAdministrationRoute(settingsFile.value(childKey).toString());
        else if (childKey == "absorptionModel")
            absorptionModel = extractAbsorptionModel(settingsFile.value(childKey).toString());
    }

    settingsFile.endGroup();

    return Tucuxi::Core::FormulationAndRoute(
                formulation,
                administrationRoute,
                absorptionModel,
                administrationName);
}

}
}
}
