//
// Created by Jeromy Zhang on 4/16/23.
//

#ifndef CSPILLENGINE_ENGINE_INCLUDE_PYBIND_PB_ENGINE_HPP_
#define CSPILLENGINE_ENGINE_INCLUDE_PYBIND_PB_ENGINE_HPP_

#include <pybind11/pybind11.h>

#include "Engine.h"
#include "SDL.h"

namespace py = pybind11;

namespace CSPill::EngineCore {

void PB_Engine(py::module &m) {
  py::class_<Engine>(m, "Engine")
      .def(py::init(
          [](std::string_view title, int w, int h,
             int x = SDL_WINDOWPOS_CENTERED, int y = SDL_WINDOWPOS_CENTERED,
             Uint32 sdl_init_flags =
                 SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER,
             SDL_WindowFlags window_flags = static_cast<SDL_WindowFlags>(
                 SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI),
             SDL_RendererFlags renderer_flags = static_cast<SDL_RendererFlags>(
                 SDL_RENDERER_PRESENTVSYNC |
                 SDL_RENDERER_ACCELERATED)) -> std::unique_ptr<Engine> {
            return Engine::Create(title, w, h, x, y, sdl_init_flags,
                                  window_flags, renderer_flags);
          }))
      .def(py::init(
          [](std::string_view title, int w, int h) -> std::unique_ptr<Engine> {
            return Engine::Create(
                title, w, h, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER,
                static_cast<SDL_WindowFlags>(SDL_WINDOW_RESIZABLE |
                                             SDL_WINDOW_ALLOW_HIGHDPI),
                static_cast<SDL_RendererFlags>(SDL_RENDERER_PRESENTVSYNC |
                                               SDL_RENDERER_ACCELERATED));
          }))
      .def_static("Create", &Engine::Create)
      .def(
          "AddObject",
          [](Engine &self, const std::string &name, GameObject *object)
              -> bool { return self.AddObject(name, object); },
          py::arg("name"), py::arg("object"), py::keep_alive<1, 2>())
      .def("GetObject", &Engine::GetObject)
      .def("RemoveObject", &Engine::RemoveObject, "Remove Object from Engine")
      .def("Terminate", &Engine::SetGameOver, py::arg("gameover") = true)
      .def("IsGameOver", &Engine::IsGameOver)
      .def("SwitchScene", &Engine::SwitchScene)
      .def("IsKeyPressed", &Engine::IsKeyPressed)
      .def("Run", &Engine::Run);
}

}  // namespace CSPill::EngineCore
#endif  // CSPILLENGINE_ENGINE_INCLUDE_PYBIND_PB_ENGINE_H_
