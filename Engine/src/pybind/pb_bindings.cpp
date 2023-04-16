#include <pybind11/pybind11.h>

#include "pb_component.hpp"
#include "pb_gameobject.hpp"
#include "pb_resource_manager.hpp"
#include "pb_scene.hpp"

namespace py = pybind11;
using namespace CSPill::EngineCore;

// Declare the functions for registering the bindings
// void init_component_bindings(py::module& m);
// void init_gameobject_bindings(py::module& m);

PYBIND11_MODULE(PyCSPillEngine, m) {
  m.doc() = "CSPill Engine Python Bindings";
  PB_GameObject(m);
  PB_Component(m);
  PB_Scene(m);
  PB_ResourceManager(m);
}