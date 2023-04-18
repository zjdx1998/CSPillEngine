#include <pybind11/pybind11.h>

#include "UIText.h"

namespace py = pybind11;

namespace CSPill::UI {

using EngineCore::GameObject;

void PB_UIText(py::module &m) {
  py::class_<UIText, GameObject, std::unique_ptr<UIText>>(
      m, "UIText", "UIText displays score and level of the game.")
      .def_static("Create", &UIText::Create, py::arg("font_name"),
                  py::arg("content"), py::arg("pos"), py::arg("size"),
                  py::arg("font_size"), "Factory function to create UIText.")
      .def("GetFontName", &UIText::GetFontName, "Get the name of the font.")
      .def("SetFontName", &UIText::SetFontName, "Set the font name.")
      .def("GetPos", &UIText::GetPos, "Get the position of the font.")
      .def("SetPos", &UIText::SetPos, "Set the position of the font.")
      .def("GetFontSize", &UIText::GetFontSize, "Get the font size.")
      .def("SetFontSize", &UIText::SetFontSize, "Set the font size.")
      .def("GetSize", &UIText::GetSize, "Get display size.")
      .def("SetSize", &UIText::SetSize, "Set the display size.")
      .def("GetContent", &UIText::GetContent, "Get the content of the font.")
      .def("SetContent", &UIText::SetContent, "Set the displayed contents.")
      .def(
          "Render",
          [](GameObject &self, void *temp) {
            self.Render(static_cast<SDL_Renderer *>(temp));
          },
          "Render the contents.");
}
}  // namespace CSPill::UI