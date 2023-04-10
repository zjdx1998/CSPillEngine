cmake_minimum_required(VERSION 3.21)
include(CMakePrintHelpers)
PROJECT(CSPillEngine)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(thirdparty/SDL)

# SDL_ttf
set(SDL2TTF_VENDORED ON)
add_subdirectory(thirdparty/SDL_ttf)

# SDL_mixer
set(SDL2MIXER_VENDORED ON)
set(SDL2MIXER_INSTALL OFF)
add_subdirectory(thirdparty/SDL_mixer)

# SDL_image
set(SDL2IMAGE_VENDORED ON)
set(SDL2IMAGE_INSTALL OFF)
add_subdirectory(thirdparty/SDL_image)

# Pybind11
add_subdirectory(thirdparty/pybind11)
file(GLOB SRC_PYBIND src/pybind/*.cpp)
pybind11_add_module(PyCSPillEngine ${SRC_PYBIND})

# imgui
file(GLOB SRC_IMGUI thirdparty/imgui/*.cpp)
file(GLOB SRC_IMGUI_BACKEND thirdparty/imgui/backends/imgui_impl_sdlrenderer.cpp thirdparty/imgui/backends/imgui_impl_sdl2.cpp)

# ImGuiFileDialog
set(INC_IMGUI_FILE_DIALOG thirdparty/ImGuiFileDialog/)
set(SRC_IMGUI_FILE_DIALOG thirdparty/ImGuiFileDialog/ImGuiFileDialog.cpp)

set(IMGUI_INC thirdparty/imgui)
set(IMGUI_BACKEND thirdparty/imgui/backends)

# JSON
add_subdirectory(thirdparty/json)

# Core
file(GLOB SRC_CORE src/Core/*.cpp)
set(INC_CORE include/Core)
list(APPEND CORE_SRCS_DIRS ${SRC_CORE})
list(APPEND CORE_INC_DIRS ${INC_CORE})
add_library(Core ${CORE_SRCS_DIRS})
target_include_directories(Core PUBLIC ${CORE_INC_DIRS})
add_dependencies(Core SDL2 nlohmann_json SDL2_ttf SDL2_mixer)
target_link_libraries(Core SDL2 nlohmann_json SDL2_ttf SDL2_mixer SDL2_image)

# Editor
file(GLOB SRC_EDITOR src/Editor/*.cpp)

list(APPEND INCLUDE_DIRS include/Editor)
list(APPEND INCLUDE_DIRS ${IMGUI_INC})
list(APPEND INCLUDE_DIRS ${IMGUI_BACKEND})
list(APPEND INCLUDE_DIRS ${INC_IMGUI_FILE_DIALOG})

list(APPEND EDITOR_SRCS ${SRC_EDITOR})
list(APPEND EDITOR_SRCS ${SRC_IMGUI})
list(APPEND EDITOR_SRCS ${SRC_IMGUI_BACKEND})
list(APPEND EDITOR_SRCS ${SRC_IMGUI_FILE_DIALOG})

add_executable(Editor ${EDITOR_SRCS})
target_include_directories(Editor PUBLIC ${INCLUDE_DIRS})
add_dependencies(Editor SDL2 nlohmann_json)
target_link_libraries(Editor SDL2 nlohmann_json Core SDL2_image)