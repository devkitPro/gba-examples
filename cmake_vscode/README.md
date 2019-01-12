# Example to develop for GBA using CMake and Visual Studio Code

CMake scripts and VS Code files for GBA homebrew development. This was inspired by https://github.com/Lectem/3ds-cmake resp. its forks, especially https://github.com/Xtansia/3ds-cmake. See the MIT [LICENSE](LICENSE).

# How to use this?

## General

For a new project make sure you have `DevkitArmGBA.cmake` and the `cmake` folder at the root of your project (where your CMakeLists.txt is). Then go to your build directory and start cmake with

```sh
cmake -DCMAKE_TOOLCHAIN_FILE=DevkitArmGBA.cmake
```

Or, put the following at the top of your `CMakeLists.txt` file:

```cmake
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/DevkitArmGBA.cmake)
```

If you are on windows, I suggest using the `Unix Makefiles` generator.

`cmake-gui` is also a good alternative, you can specify the toolchain file the first time you configure a build.

You can use the macros and find scripts of the `cmake` folder by adding the following line to your CMakeLists.cmake :

```cmake
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
```

## To build from the command line

Navigate to this examples folder, then:
```sh
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=DevkitArmGBA.cmake
make -j 4
```

## To build from Visual Studio Code

* Must: Install the "C/C++ extension" by Microsoft.
* Recommended: If you want intellisense functionality install the "C++ intellisense" extension by austin.
* Must: Install the "CMake Tools" extension by vector-of-bool.
* Restart / Reload Visual Studio Code if you have installed extensions.
* Open this examples folder using "Open folder...".
* Choose "Unspecified" as your active CMake kit if asked. It then should be autodetected correctly.
* Make sure `.vscode/c_cpp_properties.json` contains the proper paths to your DevKitPro installation!
* Make sure `.vscode/launch.json` points to a proper emulator on your system and your .gba executable!
* Make sure `.vscode/tasks.json` points your .gba executable! (This seems a bit redundant, because CMake can build the binary just fine without it, but it is necessary for compiling on F5 to work. Currently the CMake extension builds upon launch, but only the .elf will be generated! If here's an easier way to do this, I'd love to hear it).
* You should be able to build now using F7 and build + run using F5.

# GBA CMake files

## ToolsGBA.cmake

This file must be included with `include(ToolsGBA)`. It provides several macros related to GBA development.

### add_gba_executable( elf_executable )

Will generate a .gba file from the elf_executable.

### target_maxmod_file( elf_executable sound_files )

Will generate a soundbank file for the MaxMOD music player from sound_files and add it to elf_executable.

## Example of CMakeLists.txt using assembler files, images and sounds

```cmake
cmake_minimum_required(VERSION 3.1.0)

# Note that you must copy the cmake folder and the DevkitArmGBA.cmake file in this directory
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/DevkitArmGBA.cmake)
# Add the cmake folder to the modules paths, so that we can use the tools
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
# ASM must be enabled to support .S files
enable_language(ASM)
# Include all the macros and tools needed for GBA development.
include(ToolsGBA)

project(hello_world)

# List all the source files in our directory
LIST(APPEND SOURCE_FILES
	./main.cpp
	./memcpy.s
)
# List all the data files to be included
LIST(APPEND EXTRA_DATA_FILES
	./data/dkp_logo.c
)
# List all libGBA directories
LIST(APPEND INCLUDE_DIRECTORIES
	${DEVKITPRO}/libgba/include
	${DEVKITARM}/arm-none-eabi/include/
)
# List all library directories
LIST(APPEND TARGET_LIBRARIES
	${DEVKITPRO}/libgba/lib
)

link_directories(${TARGET_LIBRARIES})
include_directories(${INCLUDE_DIRECTORIES})
# Create elf file
add_executable(hello_world.elf ${SOURCE_FILES} ${INCLUDE_FILES} ${EXTRA_DATA_FILES})
# Generate the .gba from the elf
add_gba_executable(hello_world.elf)
# Link the application, libgba and maxmod
target_link_libraries(hello_world.elf gba mm)

# List all the MaxMOD music files
file(GLOB_RECURSE MUSIC_FILES
	./music/*
)
# Build soundbank file from music files
target_maxmod_file(hello_world.elf ${MUSIC_FILES})

# List all the binary data files to be included
file(GLOB_RECURSE DATA_FILES
	./data/*.bin
)
``` 
