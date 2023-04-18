//
// Created by Jeromy Zhang on 4/17/23.
//

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>

#include "CollisionComponent.h"
#include "GameObject.h"

namespace py = pybind11;

#ifndef CSPILLENGINE_ENGINE_INCLUDE_PYBIND_PB_COLLISION_COMPONENT_HPP_
#define CSPILLENGINE_ENGINE_INCLUDE_PYBIND_PB_COLLISION_COMPONENT_HPP_

namespace CSPill::EngineCore {

void PB_CollisionComponent(py::module &m) {
  py::class_<CollisionComponent, Component>(m, "CollisionComponent", "Collision Component handles all the physics of the game.")
      .def(py::init<const SDL_FRect &>(), py::arg("bounding_box"))
      .def_property("bouding_box", &CollisionComponent::GetBoundingBox,
                    &CollisionComponent::SetBoundingBox)
      .def_property("callback", &CollisionComponent::GetCallback,
                    &CollisionComponent::SetCallback,
                    "Get/Set Call Back Function.")
      .def("Register", &CollisionComponent::Register,
           "Register a GameObject into collision events listener.")
      .def("UnRegister", &CollisionComponent::UnRegister,
           "UnRegister a GameObject from collision event listeners.")
      .def_static("Collide", &CollisionComponent::Collide,
                  "Check if two FRect overlapped or not.");
}

}  // namespace CSPill::EngineCore

#endif  // CSPILLENGINE_ENGINE_INCLUDE_PYBIND_PB_COLLISION_COMPONENT_HPP_
