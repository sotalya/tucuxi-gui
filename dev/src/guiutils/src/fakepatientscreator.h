//@@license@@

#ifndef FAKEPATIENTSCREATOR_H
#define FAKEPATIENTSCREATOR_H

namespace Tucuxi {
namespace GuiCore {
class CorePatientList;
}

}

class FakePatientsCreator
{
public:
    FakePatientsCreator();

    void createFakePatients(Tucuxi::GuiCore::CorePatientList* patientsList);
};

#endif // FAKEPATIENTSCREATOR_H
