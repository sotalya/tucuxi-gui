#ifndef INTERPRETATIONREQUEST_H
#define INTERPRETATIONREQUEST_H


#include "core/dal/entity.h"

Q_MOC_INCLUDE("admin/src/dal/clinical.h")
Q_MOC_INCLUDE("core/dal/drugtreatment.h")
Q_MOC_INCLUDE("admin/src/dal/patient.h")
Q_MOC_INCLUDE("admin/src/dal/practician.h")

namespace Tucuxi{
namespace Gui {
namespace Core {
    class DrugTreatment;
}
}
}

namespace Tucuxi {
namespace Gui {
namespace Admin {

class Patient;
class Practician;
class ClinicalSet;

class InterpretationRequest : public Tucuxi::Gui::Core::Entity
{
    Q_OBJECT

    ADMIN_ENTITY_UTILS(InterpretationRequest)

    AUTO_PROPERTY_DECL(Tucuxi::Gui::Core::DrugTreatment*, treatment, Treatment)
    AUTO_PROPERTY_DECL(Patient*, patient, Patient)
    AUTO_PROPERTY_DECL(Practician*, practician, Practician)

    AUTO_PROPERTY_DECL(QString, comedication, Comedication)
    AUTO_PROPERTY_DECL(QString, diagnosis, Diagnosis)
    AUTO_PROPERTY_DECL(QString, indication, Indication)
    AUTO_PROPERTY_DECL(QString, response, Response)
    AUTO_PROPERTY_DECL(QString, toxicity, Toxicity)
    AUTO_PROPERTY_DECL(ClinicalSet*, clinicals, Clinicals)

    //! This property will contain the source received from an external database
    //! or client
    AUTO_PROPERTY_DECL(QString, source, Source)

protected:

    Q_INVOKABLE InterpretationRequest(Tucuxi::Gui::Core::AbstractRepository *repository = 0, QObject *parent = nullptr);
    Q_INVOKABLE InterpretationRequest(Tucuxi::Gui::Core::AbstractRepository *repository, const int &id, QObject *parent = nullptr);

private:

public:
    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

};

}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::InterpretationRequest*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Admin::InterpretationRequest*>)


#endif // INTERPRETATIONREQUEST_H
