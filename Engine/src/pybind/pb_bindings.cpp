#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "pb_component.hpp"
#include "pb_engine.hpp"
#include "pb_gameobject.hpp"
#include "pb_resource_manager.hpp"
#include "pb_scene.hpp"
#include "pb_tiny_math.hpp"
#include "pb_media.hpp"

namespace py = pybind11;
using namespace CSPill::EngineCore;

// Declare the functions for registering the bindings
// void init_component_bindings(py::module& m);
// void init_gameobject_bindings(py::module& m);

PYBIND11_MODULE(PyCSPillEngine, m) {
  m.doc() = "CSPill Engine Python Bindings";
  py::module_ mCore = m.def_submodule("Core", "Core Library of CSPill Engine");
  py::module_ mUtils = m.def_submodule("Utils", "Utils part of CSPill Engine");
  PB_GameObject(mCore);
  PB_Component(mCore);
  PB_Scene(mCore);
  PB_ResourceManager(mCore);
  PB_Engine(mCore);
  CSPill::Media::PB_Media(mUtils);
  CSPill::Math::PB_TinyMath(mUtils);
}
