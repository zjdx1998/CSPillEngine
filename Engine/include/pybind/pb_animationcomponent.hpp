#include <pybind11/functional.h>
#include <pybind11/pybind11.h>

#include "AnimationComponent.h"
#include "GameObject.h"

namespace py = pybind11;

#ifndef CSPILLENGINE_ENGINE_INCLUDE_PYBIND_PB_ANIMATION_COMPONENT_HPP_
#define CSPILLENGINE_ENGINE_INCLUDE_PYBIND_PB_ANIMATION_COMPONENT_HPP_

namespace CSPill::EngineCore {

void PB_AnimationComponent(py::module &m) {
  py::class_<AnimationComponent, Component>(
      m, "AnimationComponent",
      "Animation Component takes care of all the animation made.")
      .def(py::init<>())
      .def("Update", &AnimationComponent::Update, "Update Animation Component")
      .def(
          "Render",
          [](GameObject &self, void *temp) {
            self.Render(static_cast<SDL_Renderer *>(temp));
          },
          "Render the animation.")
      .def("GetAnimations", &AnimationComponent::GetAnimations,
           "Get all Animations")
      .def("AddAnimation", &AnimationComponent::AddAnimation, py::arg("name"),
           py::arg("animation"), "Add a Animation")
      .def("AddAnimations", &AnimationComponent::AddAnimations, py::arg("name"),
           py::arg("animations"), "Add animations")
      .def("RemoveAnimation", &AnimationComponent::RemoveAnimation,
           "Remove a Animation")
      .def("GetCurrentAnimation", &AnimationComponent::GetCurrentAnimation,
           "Get the component's current animation")
      .def("SetCurrentAnimation", &AnimationComponent::SetCurrentAnimation,
           "Set the component's current animation")
      .def("GetFrame", &AnimationComponent::GetFrame,
           "Get the current animation frame")
      .def("SetFrame", &AnimationComponent::SetFrame,
           "Set the current animation frame")
      .def_property("speed", &AnimationComponent::GetSpeed,
                    &AnimationComponent::SetSpeed,
                    "Get/Set speed of animation, default is 30 FPS");
}

}  // namespace CSPill::EngineCore

#endif  // CSPILLENGINE_ENGINE_INCLUDE_PYBIND_PB_ANIMATION_COMPONENT_HPP_
