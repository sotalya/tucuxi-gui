//@@license@@

#ifndef FAKEPATIENTSCREATOR_H
#define FAKEPATIENTSCREATOR_H

namespace Tucuxi {
namespace Gui {
namespace Core {

class CorePatientList;
}
}
}

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class FakePatientsCreator
{
public:
    FakePatientsCreator();

    void createFakePatients(Tucuxi::Gui::Core::CorePatientList* patientsList);
};

}
}
}

#endif // FAKEPATIENTSCREATOR_H
