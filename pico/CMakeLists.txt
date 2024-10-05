# Minimum CMake version required
cmake_minimum_required(VERSION 3.19)

# Set up Pico SDK and additional dependencies
set(PICO_CXX_ENABLE_EXCEPTIONS 1)   # Enable exceptions for C++
include(pico_sdk_import.cmake)      # Import Pico SDK
pico_sdk_init()                     # Initialize Pico SDK

# Create a static library for Pico-specific code
add_library(PicoLib STATIC
    Pico_Buffer.h
    Pico_Buffer.cpp
    PICOWindow.h
    PICOWindow.cpp
)


# Include directories for Pico-specific headers
target_include_directories(PicoLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Add external libraries and dependencies for Pico
# (Include pimoroni libraries and any other external dependencies)
include(common/pimoroni_i2c)
include(drivers/hub75/hub75)
include(libraries/pico_explorer/pico_explorer)

# Link external dependencies to PicoLib
target_link_libraries(PicoLib
    pico_stdlib            # Pico standard library
    pimoroni_i2c           # Pimoroni I2C
    pico_explorer          # Pico Explorer library
    hub75                  # Hub75 driver
)

# Enable USB for I/O and disable UART
pico_enable_stdio_usb(PicoLib 1)
pico_enable_stdio_uart(PicoLib 0)

# Add Pico-specific extra outputs (e.g., UF2 file)
pico_add_extra_outputs(PicoLib)
