add_executable(hello_universal
    hello_universal.c
)

if (NOT FREERTOS_KERNEL_PATH AND NOT DEFINED ENV{FREERTOS_KERNEL_PATH})
    message("Skipping FreeRTOS examples as FREERTOS_KERNEL_PATH not defined")
    return()
endif()
include(FreeRTOS_Kernel_import.cmake)

# Include FreeRTOS headers
target_include_directories(hello_universal PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}   
)

# Link FreeRTOS libraries
target_link_libraries(hello_universal PRIVATE
    FreeRTOS-Kernel
    pico_async_context_freertos
    FreeRTOS-Kernel-Heap4
    pico_stdlib
)

# Define FreeRTOS configuration (if needed)
target_compile_definitions(hello_universal PRIVATE
    configNUMBER_OF_CORES=1
)

# Enable USB and UART output
pico_enable_stdio_uart(hello_universal 1)
pico_enable_stdio_usb(hello_universal 1)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(hello_universal)
