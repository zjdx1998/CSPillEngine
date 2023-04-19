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
set(INC_UTILS include/Utils)
list(APPEND CORE_SRCS_DIRS ${SRC_CORE})
list(APPEND CORE_INC_DIRS ${INC_CORE})
list(APPEND CORE_INC_DIRS ${INC_UTILS})
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

add_executable(CSPillEditor ${EDITOR_SRCS})
target_include_directories(CSPillEditor PUBLIC ${INCLUDE_DIRS})
add_dependencies(CSPillEditor SDL2 nlohmann_json)
target_link_libraries(CSPillEditor SDL2 nlohmann_json Core SDL2_image)

# Pybind11
add_subdirectory(thirdparty/pybind11)
set(PYBIND11_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/pybind11/include)
set(INC_PYBIND include/pybind)
file(GLOB SRC_PYBIND src/pybind/*.cpp)
# PyCSPillEngine
pybind11_add_module(PyCSPillEngine ${SRC_PYBIND})
target_include_directories(PyCSPillEngine PRIVATE ${INC_CORE} ${SDL2_INCLUDE_DIRS} ${Python_INCLUDE_DIRS} ${INC_PYBIND})
target_link_libraries(PyCSPillEngine PRIVATE Core SDL2)

# Install rules
set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/CSPillEngine")
install(TARGETS CSPillEditor PyCSPillEngine
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        )

install(TARGETS Core
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY include/
        DESTINATION include
        FILES_MATCHING PATTERN "*.h"
        )

install(DIRECTORY thirdparty/pybind11/include/pybind11/
        DESTINATION include/pybind11
        FILES_MATCHING PATTERN "*.h"
        )

install(TARGETS SDL2 SDL2_image SDL2_mixer SDL2_ttf nlohmann_json
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

install(DIRECTORY thirdparty/SDL/include/
        DESTINATION include
        )

install(DIRECTORY thirdparty/SDL_mixer/external/libogg/include/ogg
        thirdparty/SDL_mixer/external/libvorbis/include/vorbis
        DESTINATION include
        FILES_MATCHING PATTERN "*.h"
        )

install(DIRECTORY thirdparty/SDL_ttf/include/
        DESTINATION include
        FILES_MATCHING PATTERN "*.h"
        )

install(DIRECTORY thirdparty/SDL_image/include/
        DESTINATION include
        FILES_MATCHING PATTERN "*.h"
        )

install(DIRECTORY thirdparty/json/include/nlohmann/
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp"
        )

install(DIRECTORY thirdparty/pybind11/include/pybind11/
        DESTINATION include/pybind11
        FILES_MATCHING PATTERN "*.h"
        )