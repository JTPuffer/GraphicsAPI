# Basic CMake Project

## Overview

This project is a CMake-based system that supports both desktop and Raspberry Pi Pico platforms. It provides a simple graphics pipeline and shaders, with modules for handling window management and basic rasterization. The project is organized into different directories based on functionality.

## Project Structure

├── desktop/ │ ├── CMakeLists.txt │ ├── Desktop_window.h │ └── DesktopBuffer.h ├── Graphics/ │ ├── Shaders/ │ │ ├── FragmentShader.h │ │ ├── FragmentShaderImpl.h │ │ ├── VertexShader.h │ │ └── VertexShaderImpl.h │ ├── BasicRasterizer.h │ ├── CMakeLists.txt │ ├── GraphicsPipe.cpp │ ├── GraphicsPipe.h │ ├── IGraphicsPipe.h │ ├── IRasterizer.h │ └── VertexBuffer.h ├── pico/ │ ├── CMakeLists.txt │ ├── Pico_Buffer.h │ ├── pico_sdk_import.cmake │ ├── Pico_window.h │ └── pimoroni_pico_import.cmake ├── PicoB/ │ ├── .clang-format │ ├── .gitignore │ ├── Buffer.h │ ├── CMakeLists.txt │ ├── FixedPoint.h │ ├── IWindow.h │ ├── LICENSE │ └── main.cpp └── Math.h



### Folders:

- **desktop/**: Contains platform-specific code for desktop environments.
  - `Desktop_window.h`: Desktop window management interface.
  - `DesktopBuffer.h`: Desktop-specific buffer handling.

- **Graphics/**: Core graphics module that contains pipeline setup, shaders, and rasterization.
  - `Shaders/`: Contains header files for handling fragment and vertex shaders.
  - `GraphicsPipe.h`: Interface for the graphics pipeline.
  - `IRasterizer.h`: Interface for rasterization operations.
  
- **PicoB/**: Root of the project containing the main entry point (`main.cpp`), CMake build files, and utility headers (e.g., `Buffer.h`, `FixedPoint.h`, `Math.h`).

## Prerequisites

- CMake 3.12 or higher
- A C++ compiler (e.g., GCC, Clang)

## Building the Project

* Step-by-step bullets
```
$mkdir build
```
```
$cd build
```
```
$cmake ..
```
```
$make
```
```
$ ./GraphicsAPI
```
