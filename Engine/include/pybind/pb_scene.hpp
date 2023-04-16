#include <pybind11/pybind11.h>

#include "Scene.h"

namespace py = pybind11;

namespace CSPill::EngineCore {

void PB_Scene(py::module &m) {
  py::class_<Scene>(m, "Scene")
      .def(py::init<int, int>())
      .def(py::init<const std::vector<Layer> &, const std::vector<Tileset> &,
                    int, int>())
      .def("Layers", &Scene::Layers,
           py::return_value_policy::reference_internal)
      .def("GetLayers", &Scene::GetLayers,
           py::return_value_policy::reference_internal)
      .def("SetLayers", &Scene::SetLayers)
      .def("GetTileSets", &Scene::GetTileSets,
           py::return_value_policy::reference_internal)
      .def("TileSets", &Scene::TileSets,
           py::return_value_policy::reference_internal)
      .def("SetTileSets", &Scene::SetTileSets)
      .def(
          "AddTileSet",
          [](Scene &scene, Tileset &tileset) {
            scene.AddTileSet(std::move(tileset));
          },
          py::arg("tileset"))
      .def("GetCanvasWidth", &Scene::GetCanvasWidth)
      .def("SetCanvasWidth", &Scene::SetCanvasWidth)
      .def("GetCanvasHeight", &Scene::GetCanvasHeight)
      .def("SetCanvasHeight", &Scene::SetCanvasHeight)
      .def(
          "AddLayer",
          [](Scene &scene, Layer &layer) { scene.AddLayer(std::move(layer)); },
          py::arg("layer"))
      .def("RemoveLayer", &Scene::RemoveLayer)
      .def(
          "Render",
          [](Scene &self, void *renderer, Layer *layer, Tileset *tileset,
             bool accumulate) -> void * {
            SDL_Texture *texture =
                self.Render(static_cast<SDL_Renderer *>(renderer), layer,
                            tileset, accumulate);
            return static_cast<void *>(texture);
          },
          py::arg("renderer"), py::arg("layer"), py::arg("tileset"),
          py::arg("accumulate") = false)
      .def("Render", [](Scene &self, void *temp) -> void * {
        SDL_Texture *texture = self.Render(static_cast<SDL_Renderer *>(temp));
        return static_cast<void *>(texture);
      });
}

}  // namespace CSPill::EngineCore