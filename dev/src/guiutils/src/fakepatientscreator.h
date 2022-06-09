//@@license@@

#ifndef FAKEPATIENTSCREATOR_H
#define FAKEPATIENTSCREATOR_H

namespace ezechiel {
namespace GuiCore {
class CorePatientList;
}

}

class FakePatientsCreator
{
public:
    FakePatientsCreator();

    void createFakePatients(ezechiel::GuiCore::CorePatientList* patientsList);
};

#endif // FAKEPATIENTSCREATOR_H
