#include <pybind11/pybind11.h>

#include "Component.h"
#include "GameObject.h"
#include "SDL.h"
#include "TransformComponent.h"

namespace py = pybind11;

namespace CSPill::EngineCore {

void PB_TransformComponent(py::module& m) {
  py::class_<TransformComponent>(m, "TransformComponent")
      .def(py::init<const std::string_view&>())
      .def("Update", &TransformComponent::Update, "Update TransformComponent")
      .def("position", &TransformComponent::position, "Get position")
      .def("velocity", &TransformComponent::velocity, "Get velocity");
}

}  // namespace CSPill::EngineCore