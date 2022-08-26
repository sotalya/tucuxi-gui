//@@license@@

#ifndef FORMULATIONANDROUTETRANSLATOR_H
#define FORMULATIONANDROUTETRANSLATOR_H


#include <QString>
#include <QMap>

#include "tucucore/drugmodel/formulationandroute.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

class FormulationAndRouteTranslator
{
public:
    FormulationAndRouteTranslator();
    virtual ~FormulationAndRouteTranslator();

    virtual Tucuxi::Core::FormulationAndRoute restToInternalFormulationAndRoute(QString drugId, QString intake) = 0;
};

class ChuvFormulationAndRouteTranslator : public FormulationAndRouteTranslator
{
public:
    ChuvFormulationAndRouteTranslator();

    virtual Tucuxi::Core::FormulationAndRoute restToInternalFormulationAndRoute(QString drugId, QString intake);

private:
    QMap<QString, QMap<QString, Tucuxi::Core::FormulationAndRoute>> map;
    QMap<QString, Tucuxi::Core::FormulationAndRoute> mapIntake;

};

class ExternalFormulationAndRouteTranslator : public FormulationAndRouteTranslator
{
public:
    ExternalFormulationAndRouteTranslator();

    void setFileName(const QString &fileName);

    virtual Tucuxi::Core::FormulationAndRoute restToInternalFormulationAndRoute(QString drugId, QString intake);

private:
    QString m_fileName;
};


}
}
}

#endif // FORMULATIONANDROUTETRANSLATOR_H
