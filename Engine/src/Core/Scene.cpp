//
// Created by Jeromy Zhang on 3/28/23.
//
#include "Scene.h"

#include <utility>

#include "ResourceManager.h"
#include "Utils.h"

namespace CSPill::EngineCore {

Layer::Layer(std::string name, std::string tileset,
             const std::vector<int> &data)
    : name_(std::move(name)), tileset_(std::move(tileset)), data_(data) {}

Layer::Layer(std::string name, const std::vector<int> &data)
    : name_(std::move(name)), data_(data) {}

std::string_view Layer::GetName() const { return name_; }

void Layer::SetName(const std::string &name) { name_ = name; }

std::string_view Layer::GetTileset() const { return tileset_; }

void Layer::SetTileset(const std::string &tileset) { tileset_ = tileset; }

std::vector<int> &Layer::Data() { return data_; }
const std::vector<int> &Layer::GetData() const { return data_; }

void Layer::SetData(const std::vector<int> &data) { data_ = data; }

bool Layer::operator==(const Layer &rhs) const {
  return this->name_ == rhs.name_ && this->tileset_ == rhs.tileset_;
}

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
int &Tileset::TileWidth() { return tile_width_; }
int &Tileset::TileHeight() { return tile_height_; }

Scene::Scene(std::string name, int canvas_width, int canvas_height,
             const SDL_Color &bkg_color)
    : name_(std::move(name)),
      canvas_width_(canvas_width),
      canvas_height_(canvas_height),
      background_color_(bkg_color) {}
Scene::Scene(std::string name, const std::vector<Layer> &layers,
             const std::vector<Tileset> &tile_sets, int canvas_width,
             int canvas_height, const SDL_Color &bkg_color)
    : name_(std::move(name)),
      layers_(layers),
      tile_sets_(tile_sets),
      canvas_width_(canvas_width),
      canvas_height_(canvas_height),
      background_color_(bkg_color) {}
std::vector<Layer> &Scene::Layers() { return layers_; }
const std::vector<Layer> &Scene::GetLayers() const { return layers_; }
void Scene::SetLayers(const std::vector<Layer> &layers) { layers_ = layers; }
std::vector<Tileset> &Scene::TileSets() { return tile_sets_; }
[[nodiscard]] const std::vector<Tileset> &Scene::GetTileSets() const {
  return tile_sets_;
}
void Scene::SetTileSets(const std::vector<Tileset> &tile_sets) {
  tile_sets_ = tile_sets;
}
void Scene::AddTileSet(Tileset &&t) { tile_sets_.push_back(std::move(t)); }
int Scene::GetCanvasWidth() const { return canvas_width_; }
void Scene::SetCanvasWidth(int canvas_width) { canvas_width_ = canvas_width; }
int Scene::GetCanvasHeight() const { return canvas_height_; }
void Scene::SetCanvasHeight(int canvas_height) {
  canvas_height_ = canvas_height;
}
const SDL_Color &Scene::GetBackgroundColor() const { return background_color_; }
SDL_Color &Scene::BackgroundColor() { return background_color_; }
void Scene::SetBackgroundColor(const SDL_Color &background_color) {
  this->background_color_ = background_color;
}

Scene::~Scene() { SDL_DestroyTexture(scene_texture_); }

SDL_Texture *Scene::Render(SDL_Renderer *renderer, Layer *layer,
                           Tileset *tileset, bool accumulate) {
  int canvas_width = this->canvas_width_, canvas_height = this->canvas_height_;
  int tile_width = tileset->GetTileWidth(),
      tile_height = tileset->GetTileHeight();
  if (this->scene_texture_ == nullptr) {
    this->scene_texture_ = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                             SDL_TEXTUREACCESS_TARGET,
                                             canvas_width, canvas_height);
    accumulate = false;
  }

  SDL_SetRenderTarget(renderer, this->scene_texture_);
  if (!accumulate) {
    SDL_Surface *surface =
        SDL_CreateRGBSurface(0, canvas_width, canvas_height, 32, 0, 0, 0, 0);
    SDL_FillRect(
        surface, nullptr,
        SDL_MapRGB(surface->format, this->GetBackgroundColor().r,
                   this->GetBackgroundColor().g, this->GetBackgroundColor().b));
    auto white_bkg = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, white_bkg, nullptr, nullptr);
    SDL_DestroyTexture(white_bkg);
  }
  if (layer) {
    if (layer->Data().size() !=
        canvas_width * canvas_height / tile_width / tile_height) {
      layer->Data().resize(
          canvas_width * canvas_height / tile_width / tile_height, -1);
    }
    for (int i = 0; i < layer->Data().size(); i++) {
      if (layer->Data()[i] == -1) continue;
      int row = i / (canvas_width / tile_width),
          col = i % (canvas_width / tile_width);
      std::pair<int, int> row_and_col =
          ::EngineCore::Utils::GetRowAndCol(layer->Data()[i]);
      auto current_brush =
          std::string(tileset->GetName()) +
          std::string(::EngineCore::Utils::IMAGE_SPLIT) +
          std::to_string(
              ::EngineCore::Utils::GetDataFromRowAndCol(row_and_col));
      SDL_Rect dst_rect = {col * tileset->GetTileWidth(),
                           row * tileset->GetTileHeight(),
                           tileset->GetTileWidth(), tileset->GetTileHeight()};
      if (auto active_layer = ResourceManager::GetInstance().LoadImage(
              std::string(tileset->GetName()))) {
        SDL_Rect src_rect = {row_and_col.second * tileset->GetTileWidth(),
                             row_and_col.first * tileset->GetTileHeight(),
                             tileset->GetTileWidth(), tileset->GetTileHeight()};
        SDL_Texture *cropped_texture =
            ResourceManager::GetInstance().QueryTexture(current_brush);
        if (cropped_texture) {
          SDL_RenderCopy(renderer, cropped_texture, nullptr, &dst_rect);
        }
      }
    }
  }
  SDL_SetRenderTarget(renderer, nullptr);
  return this->scene_texture_;
}

SDL_Texture *Scene::Render(SDL_Renderer *renderer) {
  for (auto &layer : this->layers_) {
    for (auto &tileset : this->tile_sets_)
      if (tileset.GetName() == layer.GetTileset()) {
        Render(renderer, &layer, &tileset, true);
      }
  }
  return this->scene_texture_;
}

void Scene::AddLayer(Layer &&layer) { layers_.push_back(std::move(layer)); }

void Scene::RemoveLayer(const std::string &name) {
  for (auto it = layers_.begin(); it != layers_.end(); it++) {
    if (it->GetName() == name) {
      layers_.erase(it);
      break;
    }
  }
}
const std::string &Scene::GetName() const { return name_; }
void Scene::SetName(const std::string &name) { name_ = name; }

}  // namespace CSPill::EngineCore