#include <pybind11/pybind11.h>

#include "ResourceManager.h"

namespace py = pybind11;

namespace CSPill::EngineCore {

void PB_ResourceManager(py::module &m) {
  py::class_<ResourceManager>(m, "ResourceManager")
    .def_static("GetInstance", &ResourceManager::GetInstance, py::return_value_policy::reference)
    .def("LoadResources", &ResourceManager::LoadResources)
    .def("ReleaseAll", &ResourceManager::ReleaseAll)
    .def("LoadAudio", [](ResourceManager &self, const std::string &audio_name) -> void* {
        return static_cast<void *>(self.LoadAudio(audio_name));
    })
    .def("LoadFont", [](ResourceManager &self, const std::string &font_name) -> void* {
        return static_cast<void *>(self.LoadFont(font_name));
    })
    .def("LoadImage", [](ResourceManager &self, const std::string &image_name) -> void* {
        return static_cast<void *>(self.LoadImage(image_name));
    })
    .def("LoadScene", &ResourceManager::LoadScene, py::return_value_policy::reference)
    .def("GetAudioResourceNames", &ResourceManager::GetAudioResourceNames)
    .def("GetFontResourceNames", &ResourceManager::GetFontResourceNames)
    .def("GetSceneNames", &ResourceManager::GetSceneNames)
    .def("GetImageNames", &ResourceManager::GetImageNames)
    .def("GetRenderer", [](ResourceManager &self) -> void* {
        return static_cast<void *>(self.GetRenderer());
    })
    .def("SetRenderer", [](ResourceManager &self, void *renderer) {
        self.SetRenderer(static_cast<SDL_Renderer *>(renderer));
    });
}

}  // namespace CSPill::EngineCore