#ifndef ERRORS_DRUGEDITOR_H
#define ERRORS_DRUGEDITOR_H

#include "core/utils/logging.h"


namespace ezechiel {
namespace drugeditor {

EZERROR(DRUGEDITOR, NOEZERROR,"There is no error.")
EZERROR(DRUGEDITOR, QMLERROR,"An error occurred in the QML.")
EZERROR(DRUGEDITOR, MODELIOERROR, "An error occurred in reading or writing of data using a model.")

}//namespace drugeditor
}//namespace ezechiel

#endif // ERRORS_DRUGEDITOR_H
