//
// Created by lucas on 3/30/2023.
//

#include <pybind11/pybind11.h>

#include "Component.h"
#include "GameObject.h"

namespace py = pybind11;

namespace CSPill::EngineCore {

void PB_GameObject(py::module &m) {
  py::class_<GameObject>(m, "GameObject")
      .def(py::init(
          []() -> std::unique_ptr<GameObject> { return GameObject::Create(); }))
      .def("Update", &GameObject::Update)
      .def("Render",
           [](GameObject &self, void *temp) {
             self.Render(static_cast<SDL_Renderer *>(temp));
           })
      .def(
          "AddComponent",
          [](GameObject &obj, Component &comp) {
            return obj.AddComponent(std::move(comp));
          },
          py::arg("component"))
      .def("RemoveComponent", &GameObject::RemoveComponent,
           py::arg("component_name"))
      .def("GetComponent", &GameObject::GetComponent, py::arg("component_name"),
           py::return_value_policy::reference)
      .def_static("Create", &GameObject::Create);
}
}  // namespace CSPill::EngineCore