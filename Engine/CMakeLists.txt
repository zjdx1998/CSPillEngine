cmake_minimum_required(VERSION 3.21)
include(CMakePrintHelpers)
PROJECT(CSPillEngine)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(thirdparty/SDL)
add_subdirectory(thirdparty/SDL_ttf)

#find_package(SDL2 REQUIRED COMPONENTS SDL2)
#find_package(SDL2_ttf REQUIRED)
#find_package(SDL2_mixer REQUIRED)

# imgui
file(GLOB SRC_IMGUI thirdparty/imgui/*.cpp)
file(GLOB SRC_IMGUI_BACKEND thirdparty/imgui/backends/imgui_impl_sdlrenderer.cpp thirdparty/imgui/backends/imgui_impl_sdl2.cpp)

set(IMGUI_INC thirdparty/imgui)
set(IMGUI_BACKEND thirdparty/imgui/backends)

# JSON
add_subdirectory(thirdparty/json)

# Core
file(GLOB SRC_CORE src/Core/*.cpp)
set(INC_CORE include/Core)
list (APPEND CORE_SRCS_DIRS ${SRC_CORE})
list (APPEND CORE_INC_DIRS ${INC_CORE})
add_library(Core ${CORE_SRCS_DIRS})
target_include_directories(Core PUBLIC ${CORE_INC_DIRS})
target_link_libraries(Core SDL2 nlohmann_json)

# Editor
file(GLOB SRC_EDITOR src/Editor/*.cpp)

list (APPEND INCLUDE_DIRS include)
list (APPEND INCLUDE_DIRS ${IMGUI_INC})
list (APPEND INCLUDE_DIRS ${IMGUI_BACKEND})

list (APPEND EDITOR_SRCS ${SRC_EDITOR})
list (APPEND EDITOR_SRCS ${SRC_IMGUI})
list (APPEND EDITOR_SRCS ${SRC_IMGUI_BACKEND})

add_executable(Editor ${EDITOR_SRCS} )
target_include_directories(Editor PUBLIC ${INCLUDE_DIRS})
target_link_libraries(Editor SDL2 nlohmann_json)