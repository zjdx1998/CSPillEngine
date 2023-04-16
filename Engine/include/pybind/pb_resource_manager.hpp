#include <pybind11/pybind11.h>

#include "ResourceManager.h"

namespace py = pybind11;

namespace CSPill::EngineCore {

void PB_ResourceManager(py::module &m) {
  py::class_<ResourceManager>(m, "ResourceManager", "Resource Manager for both engine and game.")
      .def_static("GetInstance", &ResourceManager::GetInstance,
                  py::return_value_policy::reference, "GetInstance will return the reference to ResourceManager object if it is created."
                                                      "Otherwise, create the object and return.")
      .def("LoadResources", &ResourceManager::LoadResources, "LoadResources takes a folder path as an argument and load all resources with certain "
                                                             "formats to ResourceManager.")
      .def("ReleaseAll", &ResourceManager::ReleaseAll, "Destroy all loaded resources.")
      .def("LoadAudio",
           [](ResourceManager &self, const std::string &audio_name) -> void * {
             return static_cast<void *>(self.LoadAudio(audio_name));
           }, "Search for the audio resource by audio_name.")
      .def("LoadFont",
           [](ResourceManager &self, const std::string &font_name) -> void * {
             return static_cast<void *>(self.LoadFont(font_name));
           }, "Search for the font resource by font_name.")
      .def("LoadImage",
           [](ResourceManager &self, const std::string &image_name) -> void * {
             return static_cast<void *>(self.LoadImage(image_name));
           }, "Search for the image resource by image_name.")
      .def("LoadScene", &ResourceManager::LoadScene,
           py::return_value_policy::reference, "Search for the tile map resource by scene_name.")
      .def("GetAudioResourceNames", &ResourceManager::GetAudioResourceNames, "Get a list of audio names that loaded to ResourceManager.")
      .def("GetFontResourceNames", &ResourceManager::GetFontResourceNames, "Get a list of font names that loaded to ResourceManager.")
      .def("GetSceneNames", &ResourceManager::GetSceneNames, "Get a list of scene names that loaded to ResourceManager.")
      .def("GetImageNames", &ResourceManager::GetImageNames, "Get a list of image names that loaded to ResourceManager.")
      .def("GetRenderer",
           [](ResourceManager &self) -> void * {
             return static_cast<void *>(self.GetRenderer());
           }, "Getter function for SDL_Renderer.")
      .def("SetRenderer", [](ResourceManager &self, void *renderer) {
        self.SetRenderer(static_cast<SDL_Renderer *>(renderer));
      }, "Setter function for SDL_Renderer.");
}

}  // namespace CSPill::EngineCore