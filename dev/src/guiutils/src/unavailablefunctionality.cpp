//@@license@@

#include "unavailablefunctionality.h"
#include <QMessageBox>

UnavailableFunctionality::UnavailableFunctionality()
{

}

void UnavailableFunctionality::displayMessage()
{
    QMessageBox::warning(nullptr, "Not available", "This functionality is not available in the demo version. If you want to participate to the development/evaluation of the beta version, please contact the development team.");
}
