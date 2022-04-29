//@@license@@

#ifndef FAKEPATIENTSCREATOR_H
#define FAKEPATIENTSCREATOR_H

namespace ezechiel {
namespace core {
class CorePatientList;
}

}

class FakePatientsCreator
{
public:
    FakePatientsCreator();

    void createFakePatients(ezechiel::core::CorePatientList* patientsList);
};

#endif // FAKEPATIENTSCREATOR_H
