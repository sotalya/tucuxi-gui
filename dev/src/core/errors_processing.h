//@@license@@

#ifndef PROCESSINGERRORS
#define PROCESSINGERRORS

#include "core/utils/logging.h"

namespace Tucuxi {
namespace processing {

EZERROR(PROCESSING, NOEZERROR,"There is no error.")
EZERROR(PROCESSING, FAILEDVALIDDOSAGES,"Failed to get valid doses.")
EZERROR(PROCESSING, FAILEDVALIDIVALS,"Failed to get valid intervals.")
EZERROR(PROCESSING, FAILEDVALIDINFS,"Failed to get valid infusions.")
EZERROR(PROCESSING, FAILEDTARGETS,"Failed to get targets.")
EZERROR(PROCESSING, NOPERCENTILES,"Percentiles method did not calculate percentiles correctly.")
EZERROR(PROCESSING, PERCENTILESERROR,"No values set to percentiles.")
EZERROR(PROCESSING, FAILEDSCRIPT,"Failed to evaluate the script.")
}
}
#endif // PROCESSINGERRORS

