#include <pybind11/pybind11.h>

#include "UIText.h"

namespace py = pybind11;

namespace CSPill::UI {

using EngineCore::GameObject;

void PB_UIText(py::module &m) {
  py::class_<UIText, GameObject, std::unique_ptr<UIText>>(m, "UIText")
      .def_static("Create", &UIText::Create, py::arg("font_name"),
                  py::arg("content"), py::arg("pos"), py::arg("size"),
                  py::arg("font_size"))
      .def("GetFontName", &UIText::GetFontName)
      .def("SetFontName", &UIText::SetFontName)
      .def("GetPos", &UIText::GetPos)
      .def("SetPos", &UIText::SetPos)
      .def("GetFontSize", &UIText::GetFontSize)
      .def("SetFontSize", &UIText::SetFontSize)
      .def("GetSize", &UIText::GetSize)
      .def("SetSize", &UIText::SetSize)
      .def("GetContent", &UIText::GetContent)
      .def("SetContent", &UIText::SetContent)
      .def("Render", [](GameObject &self, void *temp) {
        self.Render(static_cast<SDL_Renderer *>(temp));
      });
}
}  // namespace CSPill::EngineCore