#include <pybind11/pybind11.h>

#include "Component.h"
#include "GameObject.h"
#include "SDL.h"
#include "TransformComponent.h"

namespace py = pybind11;

namespace CSPill::EngineCore {

void PB_TransformComponent(py::module &m) {
  py::class_<TransformComponent, Component>(m, "TransformComponent", "Transform Component takes care of all objects' displacement.")
      .def(py::init<>())
      .def("Update", &TransformComponent::Update, "Update TransformComponent")
      .def("position", &TransformComponent::position, "Get position",
           py::return_value_policy::reference)
      .def("velocity", &TransformComponent::velocity, "Get velocity",
           py::return_value_policy::reference)
      .def_property("scale", &TransformComponent::GetScale,
                    &TransformComponent::SetScale, "Set Scale");
}

}  // namespace CSPill::EngineCore