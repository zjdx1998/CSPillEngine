//
// Created by lucas on 3/30/2023.
//

#include <pybind11/pybind11.h>

#include "Component.h"
#include "GameObject.h"

namespace py = pybind11;

namespace CSPill::EngineCore {

void PB_GameObject(py::module &m) {
  py::class_<GameObject>(m, "GameObject",
                         "Base class for all entities in CSPill Scenes.")
      .def(py::init(
          []() -> std::unique_ptr<GameObject> { return GameObject::Create(); }))
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
          [](GameObject &obj, Component &comp) {
            return obj.AddComponent(std::move(comp));
          },
          py::arg("component"), "Add a component to current GameObject.")
      .def("RemoveComponent", &GameObject::RemoveComponent,
           py::arg("component_name"),
           "Remove a component based on the component name.")
      .def("GetComponent", &GameObject::GetComponent, py::arg("component_name"),
           py::return_value_policy::reference,
           "Get a raw pointer to component based on name.")
      .def_static("Create", &GameObject::Create,
                  "Factory function to create an GameObject.");
}
}  // namespace CSPill::EngineCore