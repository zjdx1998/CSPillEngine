#include "Component.h"
#include "GameObject.h"
#include "SDL.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace CSPill::EngineCore {

class PyComponent : public Component {
 public:
  using Component::Component;
  void Update(GameObject *object) override {
    PYBIND11_OVERLOAD_PURE(void,       // Return type
                           Component,  // Parent class
                           Update,     // Name of function
                           object      // Argument(s)
    );
  }
  void Receive(std::string_view message) override {
    PYBIND11_OVERLOAD_PURE(void,       // Return type
                           Component,  // Parent class
                           Receive,    // Name of function
                           message     // Argument(s)
    );
  }
};

void PB_Component(py::module &m) {
  py::class_<Component, PyComponent>(m, "Component")
      .def(py::init([](std::string_view name) {
        return new CSPill::EngineCore::PyComponent(name);
      }))
      .def("Update", &Component::Update)
      .def("Render",
           [](GameObject &self, void *temp) {
             self.Render(static_cast<SDL_Renderer *>(temp));
           })
      .def("Name", &Component::Name)
      .def("Receive", &Component::Receive);
}

}  // namespace CSPill::EngineCore