# CMake minimum version required
cmake_minimum_required(VERSION 3.10)

# Set project name
project(MyGame)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/src)

set(SOURCE_FILES
	src/main.cpp
	src/Game.cpp
	src/EventHandler.cpp
	src/Renderer.cpp
	src/combatants/Combatant.cpp
	src/combatants/Attack.cpp
	src/combatants/Ranged.cpp
	src/combatants/Intel.cpp
   )

# Set platform-specific options
if(APPLE)
    message(STATUS "Compiling for macOS")

set(SDL2_ROOT "/opt/homebrew/opt/sdl2")
set(SDL2_MIXER_ROOT "/opt/homebrew/opt/sdl2_mixer")
set(SDL2_TTF_ROOT "/opt/homebrew/opt/sdl2_ttf")


include_directories(${SDL2_ROOT}/include/SDL2 ${SDL2_MIXER_ROOT}/include/SDL2 ${SDL2_TTF_ROOT}/include/SDL2)
link_directories(${SDL2_ROOT}/lib ${SDL2_MIXER_ROOT}/lib ${SDL2_TTF_ROOT}/lib)


endif()

# Add source files
add_executable(BattleSim ${SOURCE_FILES})

# Link SDL2 libraries
target_link_libraries(BattleSim PRIVATE SDL2 SDL2_mixer SDL2_ttf)
