#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

// This example uses a common include to avoid repetition
#include "FreeRTOSConfig_examples_common.h"

// Configuration for the memory-mapped timer registers
#define configMTIME_BASE_ADDRESS 0x20000000U    // Update with your MTIME base address
#define configMTIMECMP_BASE_ADDRESS 0x20000008U // Update with your MTIMECMP base address

#endif // FREERTOS_CONFIG_H
