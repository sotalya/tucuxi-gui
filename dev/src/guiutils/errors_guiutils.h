//@@license@@

#ifndef GUIERRORS
#define GUIERRORS

#include <QDebug>
#include "core/utils/logging.h"

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

EZERROR(GUIUTILS, NOEZERROR,"There is no error.")
EZERROR(GUIUTILS, QMLERROR,"An error occurred in the QML.")
EZERROR(GUIUTILS, MODELIOERROR, "An error occurred in reading or writing of data using a model.")
EZERROR(GUIUTILS, NOPATIENT, "Patient not set.")
EZERROR(GUIUTILS, NODRUGS, "No Drugs in database.")

}//namespace GuiUtils
}//namespace Gui
}//namespace Tucuxi
#endif // GUIERRORS

