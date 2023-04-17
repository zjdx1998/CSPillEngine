#include <pybind11/pybind11.h>

#include "CameraComponent.h"
#include "Component.h"
#include "GameObject.h"
#include "SDL.h"

namespace py = pybind11;

namespace CSPill::EngineCore {

void PB_CameraComponent(py::module &m) {
  py::class_<CameraComponent, Component>(m, "CameraComponent")
      .def(py::init<const std::string_view &>())
      .def("Update", &CameraComponent::Update, "Update CameraComponent")
      .def("Bind", &CameraComponent::Bind, py::arg("object"),
           py::arg("offset") = Math::Vec2D{0, 0}, "Bind CameraComponent")
      .def("GetViewport", &CameraComponent::GetViewport, "Get Viewport")
      .def("SetViewport", &CameraComponent::SetViewport, "Set Viewport");
}

}  // namespace CSPill::EngineCore