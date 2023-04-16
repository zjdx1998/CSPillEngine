#include <pybind11/pybind11.h>

#include "Scene.h"

namespace py = pybind11;

namespace CSPill::EngineCore {

void PB_Scene(py::module &m) {
  py::class_<Scene>(m, "Scene", "Tile map of the game")
      .def(py::init<int, int>())
      .def(py::init<const std::vector<Layer> &, const std::vector<Tileset> &,
                    int, int>())
      .def("Layers", &Scene::Layers,
           py::return_value_policy::reference_internal, "Get the layers contained by Scene.")
      .def("GetLayers", &Scene::GetLayers,
           py::return_value_policy::reference_internal, "Get the layers contained by Scene.")
      .def("SetLayers", &Scene::SetLayers, "Set the layers contained by Scene.")
      .def("GetTileSets", &Scene::GetTileSets,
           py::return_value_policy::reference_internal, "Get the Tileset contained by Scene.")
      .def("TileSets", &Scene::TileSets,
           py::return_value_policy::reference_internal, "Get the Tileset contained by Scene.")
      .def("SetTileSets", &Scene::SetTileSets, "Set the Tilesets contained by Scene.")
      .def(
          "AddTileSet",
          [](Scene &scene, Tileset &tileset) {
            scene.AddTileSet(std::move(tileset));
          },
          py::arg("tileset"), "Add a Tileset to current Scene.")
      .def("GetCanvasWidth", &Scene::GetCanvasWidth, "Get canvas width.")
      .def("SetCanvasWidth", &Scene::SetCanvasWidth, "Set canvas width to canvas_width.")
      .def("GetCanvasHeight", &Scene::GetCanvasHeight, "Get canvas height.")
      .def("SetCanvasHeight", &Scene::SetCanvasHeight, "Set canvas height to canvas_height.")
      .def(
          "AddLayer",
          [](Scene &scene, Layer &layer) { scene.AddLayer(std::move(layer)); },
          py::arg("layer"), "Add a Layer to Scene.")
      .def("RemoveLayer", &Scene::RemoveLayer, "Remove a layer by name.")
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
          py::arg("accumulate") = false, "Render a specific Layer or Tileset.")
      .def("Render", [](Scene &self, void *temp) -> void * {
        SDL_Texture *texture = self.Render(static_cast<SDL_Renderer *>(temp));
        return static_cast<void *>(texture);
      }, "Render all layers and Tilesets in Scene.");
}

}  // namespace CSPill::EngineCore