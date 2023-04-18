//
// Created by lucas on 3/30/2023.
//

#include <pybind11/pybind11.h>

#include "Component.h"
#include "GameObject.h"

namespace py = pybind11;

namespace CSPill::EngineCore {

void PB_GameObject(py::module &m) {
  py::class_<GameObject, std::unique_ptr<GameObject>>(
      m, "GameObject", "Base class for all entities in CSPill Scenes.")
      .def(py::init(
          []() -> std::unique_ptr<GameObject> { return GameObject::Create(); }))
      .def(py::init(
          [](const Math::Vec2D &position, const Math::Vec2D &scale) -> std::unique_ptr<GameObject> {
              return GameObject::Create(position, scale);
          }),
          py::arg("position"), py::arg("scale"))
      .def("Update", &GameObject::Update,
           "Update is called every frame, if the object is enabled.")
      .def(
          "Render",
          [](GameObject &self, void *temp) {
            self.Render(static_cast<SDL_Renderer *>(temp));
          },
          "Render is called every frame, if the object is enabled.")
      .def(
          "AddComponent",
          [](GameObject &self, Component *comp) {
            return self.AddComponent(comp);
          },
          py::keep_alive<1, 2>(), py::arg("component"),
          "Add a component to current GameObject.")
      .def("RemoveComponent", &GameObject::RemoveComponent,
           py::arg("component_name"),
           "Remove a component based on the component name.")
      .def("GetComponent", &GameObject::GetComponent, py::arg("component_name"),
           py::return_value_policy::reference,
           "Get a raw pointer to component based on name.")
      .def_static("Create", py::overload_cast<>(&GameObject::Create),
                  "Factory function to create an empty GameObject.")
      .def_static("Create", py::overload_cast<const Math::Vec2D &, const Math::Vec2D &>(&GameObject::Create),
                  py::arg("position"), py::arg("scale"),
                  "Factory function to create an GameObject with position and scale.");
}
}  // namespace CSPill::EngineCore