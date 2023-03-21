cmake_minimum_required(VERSION 3.21)
PROJECT(CSPillsEngine)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(thirdparty/SDL)
add_subdirectory(thirdparty/SDL_ttf)

#find_package(SDL2 REQUIRED COMPONENTS SDL2)
#find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

# imgui
file(GLOB SRC_IMGUI thirdparty/imgui/*.cpp)
file(GLOB SRC_IMGUI_BACKEND thirdparty/imgui/backends/imgui_impl_sdlrenderer.cpp thirdparty/imgui/backends/imgui_impl_sdl2.cpp )

set(IMGUI_INC thirdparty/imgui)
set(IMGUI_BACKEND thirdparty/imgui/backends)

file(GLOB SRC_EDITOR src/Editor/*.cpp)

list (APPEND INCLUDE_DIRS include)
list (APPEND INCLUDE_DIRS ${IMGUI_INC})
list (APPEND INCLUDE_DIRS ${IMGUI_BACKEND})

list (APPEND EDITOR_SRCS ${SRC_EDITOR})
list (APPEND EDITOR_SRCS ${SRC_IMGUI})
list (APPEND EDITOR_SRCS ${SRC_IMGUI_BACKEND})

add_executable(Editor ${EDITOR_SRCS} )
target_include_directories(Editor PUBLIC ${INCLUDE_DIRS})
target_link_libraries(Editor SDL2)