#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "pb_animationcomponent.hpp"
#include "pb_cameracomponent.hpp"
#include "pb_collision_component.hpp"
#include "pb_component.hpp"
#include "pb_engine.hpp"
#include "pb_gameobject.hpp"
#include "pb_media.hpp"
#include "pb_resource_manager.hpp"
#include "pb_scene.hpp"
#include "pb_tiny_math.hpp"
#include "pb_transformcomponent.hpp"
#include "pb_ui_text.hpp"

namespace py = pybind11;
using namespace CSPill::EngineCore;

// Declare the functions for registering the bindings
// void init_component_bindings(py::module& m);
// void init_gameobject_bindings(py::module& m);

PYBIND11_MODULE(PyCSPillEngine, m) {
  m.doc() = "CSPill Engine Python Bindings";
  py::module_ mCore = m.def_submodule("Core", "Core Library of CSPill Engine");
  py::module_ mUtils = m.def_submodule("Utils", "Utils part of CSPill Engine");
  py::module_ mUI = m.def_submodule("UI", "UI Part of CSPillEngine");
  py::module_ mPhysics = m.def_submodule("Physics", "UI Part of CSPillEngine");
  CSPill::Math::PB_TinyMath(mUtils);
  PB_Component(mCore);
  PB_GameObject(mCore);
  PB_Scene(mCore);
  PB_ResourceManager(mCore);
  PB_Engine(mCore);
  PB_CameraComponent(mCore);
  PB_TransformComponent(mCore);
  PB_CollisionComponent(mPhysics);
  CSPill::Media::PB_Media(mUtils);
  CSPill::UI::PB_UIText(mUI);
  PB_AnimationComponent(mCore);
}
