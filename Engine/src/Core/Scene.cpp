//
// Created by Jeromy Zhang on 3/28/23.
//

#include "Scene.h"

#include <utility>

namespace CSPill::EngineCore {

Layer::Layer(std::string name, std::string tileset,
             const std::vector<double> &data)
    : name_(std::move(name)), tileset_(std::move(tileset)), data_(data) {}

std::string_view Layer::GetName() const { return name_; }

void Layer::SetName(const std::string &name) { name_ = name; }

std::string_view Layer::GetTileset() const { return tileset_; }

void Layer::SetTileset(const std::string &tileset) { tileset_ = tileset; }

const std::vector<double> &Layer::GetData() const { return data_; }

void Layer::SetData(const std::vector<double> &data) { data_ = data; }
bool Layer::IsVisible() const { return visible_; }
void Layer::SetVisible(bool visible) { visible_ = visible; }

Tileset::Tileset(std::string name, int image_width, int image_height,
                 int tile_width, int tile_height)
    : name_(std::move(name)),
      image_width_(image_width),
      image_height_(image_height),
      tile_width_(tile_width),
      tile_height_(tile_height) {}
std::string_view Tileset::GetName() const { return name_; }
void Tileset::SetName(const std::string &name) { name_ = name; }
int Tileset::GetImageWidth() const { return image_width_; }
void Tileset::SetImageWidth(int image_width) { image_width_ = image_width; }
int Tileset::GetImageHeight() const { return image_height_; }
void Tileset::SetImageHeight(int image_height) { image_height_ = image_height; }
int Tileset::GetTileWidth() const { return tile_width_; }
void Tileset::SetTileWidth(int tile_width) { tile_width_ = tile_width; }
int Tileset::GetTileHeight() const { return tile_height_; }
void Tileset::SetTileHeight(int tile_height) { tile_height_ = tile_height; }

Scene::Scene(int canvas_width, int canvas_height)
    : canvas_width_(canvas_width), canvas_height_(canvas_height) {}
Scene::Scene(const std::vector<Layer> &layers,
             const std::vector<Tileset> &tile_sets, int canvas_width,
             int canvas_height)
    : layers_(layers),
      tile_sets_(tile_sets),
      canvas_width_(canvas_width),
      canvas_height_(canvas_height) {}
const std::vector<Layer> &Scene::GetLayers() const { return layers_; }
void Scene::SetLayers(const std::vector<Layer> &layers) { layers_ = layers; }
const std::vector<Tileset> &Scene::GetTileSets() const { return tile_sets_; }
void Scene::SetTileSets(const std::vector<Tileset> &tile_sets) {
  tile_sets_ = tile_sets;
}
void Scene::AddTileSet(Tileset &&t) {
  tile_sets_.push_back(std::move(t));
}
int Scene::GetCanvasWidth() const { return canvas_width_; }
void Scene::SetCanvasWidth(int canvas_width) { canvas_width_ = canvas_width; }
int Scene::GetCanvasHeight() const { return canvas_height_; }
void Scene::SetCanvasHeight(int canvas_height) {
  canvas_height_ = canvas_height;
}

}  // namespace CSPill::EngineCore