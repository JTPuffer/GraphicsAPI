cmake_minimum_required(VERSION 3.10)

# Global variable for the project name
set(PROJECT_NAME GraphicsAPI)

# Set the project name using the global variable
project(${PROJECT_NAME})

# Set C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Option to build for either Desktop or Pico
option(PLATFORM_DESKTOP "Build for Desktop" ON)  # Default is Desktop
option(PLATFORM_PICO "Build for Pico" OFF)       # Pico is OFF by default

# Add extra compile options
add_compile_options(
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wcast-align
    -Wunused
    -Woverloaded-virtual
    -Wnull-dereference
    -Wdouble-promotion
    -Wformat=2
    #-O3
    -fexceptions
    -Winline
    -Wpessimizing-move
)

include_directories(${CMAKE_SOURCE_DIR})

# Add subdirectories for different components
add_subdirectory(desktop)
add_subdirectory(Graphics)

# Only add the pico subdirectory if PLATFORM_PICO is enabled
if(PLATFORM_PICO)
    add_subdirectory(pico)
endif()

# Define the executable using the project name
add_executable(${PROJECT_NAME} main.cpp)

# Link the correct platform-specific libraries
if(PLATFORM_DESKTOP)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_DESKTOP)
    target_link_libraries(${PROJECT_NAME} PRIVATE DesktopLib GraphicsLib)
elseif(PLATFORM_PICO)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_PICO)
    target_link_libraries(${PROJECT_NAME} PRIVATE PicoLib GraphicsLib)
endif()
