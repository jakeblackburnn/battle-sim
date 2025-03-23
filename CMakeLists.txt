# CMake minimum version required
cmake_minimum_required(VERSION 3.10)

# Set project name
project(MyGame)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)

include_directories(${PROJECT_SOURCE_DIR}/src)

set(SOURCE_FILES
	src/main.cpp
	src/Game.cpp
	src/EventHandler.cpp
   )

# Set platform-specific options
if(APPLE)
    message(STATUS "Compiling for macOS")

    # Find SDL2 installed via Homebrew
    find_package(SDL2 REQUIRED)
    find_package(SDL2_mixer REQUIRED)

set(SDL2_INCLUDE_DIR "/opt/homebrew/Cellar/sdl2/2.32.2/include/SDL2/")
set(SDL2_LIB_DIR "/opt/homebrew/Cellar/sdl2/2.32.2/lib/")

set(SDL2_MIXER_INCLUDE_DIR "/opt/homebrew/Cellar/sdl2_mixer/2.8.1_1/include/SDL2/")
set(SDL2_MIXER_LIB_DIR "/opt/homebrew/Cellar/sdl2_mixer/2.8.1_1/lib/")

    include_directories(${SDL2_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR})
    link_directories(${SDL2_LIB_DIR} ${SDL2_MIXER_LIB_DIR})

    # Set SDL2 library
    set(SDL2_LIBRARIES SDL2)
    set(SDL2_MIXER_LIBRARIES SDL2_mixer)







elseif(WIN32)
    message(STATUS "Compiling for Windows from macOS")

    # Set MinGW-w64 cross-compiler
    set(CMAKE_CXX_COMPILER /usr/local/bin/x86_64-w64-mingw32-g++)
    set(CMAKE_C_COMPILER /usr/local/bin/x86_64-w64-mingw32-gcc)

    # Set SDL2 paths for Windows
    set(SDL2_INCLUDE_DIR "./SDL-Windows/SDL3/")
    set(SDL2_LIB_DIR "/path/to/SDL2_windows/lib")

    include_directories(${SDL2_INCLUDE_DIR})
    link_directories(${SDL2_LIB_DIR})

    # Define the SDL2 libraries for Windows
    set(SDL2_LIBRARIES SDL2main SDL2)

endif()

# Add source files
add_executable(BattleSim ${SOURCE_FILES})

# Link SDL2 libraries
target_include_directories(BattleSim PRIVATE ${SDL2_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR})
target_link_libraries(BattleSim PRIVATE ${SDL2_LIBRARIES} ${SDL2_MIXER_LIBRARIES})
