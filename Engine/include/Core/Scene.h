//
// Created by Jeromy Zhang on 3/28/23.
//

#ifndef CSPILLENGINE_ENGINE_INCLUDE_CORE_SCENE_H_
#define CSPILLENGINE_ENGINE_INCLUDE_CORE_SCENE_H_

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "SDL.h"

using json = nlohmann::json;

namespace CSPill::EngineCore {

/**
 * \brief Layer is a basic unit of Scene.
 *
 * Each layer has a unique name in a Scene, and it should use one tileset.
 * The data is a vector. -1 represents empty.
 * XY represents Row X and Column Y, 0<=X,Y<=9999.
 * XY / 1000 -> X, XY % 1000 -> Y
 */
class Layer {
 public:
  Layer(std::string name, std::string tileset, const std::vector<int> &data);
  Layer(std::string name, const std::vector<int> &data);
  [[nodiscard]] std::string_view GetName() const;
  void SetName(const std::string &name);
  [[nodiscard]] std::string_view GetTileset() const;
  void SetTileset(const std::string &tileset);
  [[nodiscard]] std::vector<int> &Data();
  const std::vector<int> &GetData() const;
  void SetData(const std::vector<int> &data);
  bool operator==(const Layer& rhs) const;

 private:
  std::string name_;
  std::string tileset_;
  std::vector<int> data_;
};

/**
 * \brief Tileset describes the tileset source and name,
 * which can be retrieved from ResourceManager.
 *
 * By retrieving ResourceManager with a name of tileset,
 * we can get a Tileset object.
 */
class Tileset {
 public:
  Tileset(std::string name, int image_width, int image_height, int tile_width,
          int tile_height);
  [[nodiscard]] std::string_view GetName() const;
  void SetName(const std::string &name);
  [[nodiscard]] int GetImageWidth() const;
  void SetImageWidth(int image_width);
  [[nodiscard]] int GetImageHeight() const;
  void SetImageHeight(int image_height);
  [[nodiscard]] int GetTileWidth() const;
  int &TileWidth();
  void SetTileWidth(int tile_width);
  [[nodiscard]] int GetTileHeight() const;
  int &TileHeight();
  void SetTileHeight(int tile_height);

 private:
  std::string name_;
  int image_width_;
  int image_height_;
  int tile_width_;
  int tile_height_;
};

class Scene {
 public:
  Scene(int canvas_width, int canvas_height);
  Scene(const std::vector<Layer> &layers, const std::vector<Tileset> &tile_sets,
        int canvas_width, int canvas_height);
  ~Scene();
  [[nodiscard]] std::vector<Layer> &Layers();
  [[nodiscard]] const std::vector<Layer> &GetLayers() const;
  void SetLayers(const std::vector<Layer> &layers);
  [[nodiscard]] const std::vector<Tileset> &GetTileSets() const;
  [[nodiscard]] std::vector<Tileset> &TileSets();
  void SetTileSets(const std::vector<Tileset> &tile_sets);
  void AddTileSet(Tileset &&t);
  [[nodiscard]] int GetCanvasWidth() const;
  void SetCanvasWidth(int canvas_width);
  [[nodiscard]] int GetCanvasHeight() const;
  void SetCanvasHeight(int canvas_height);
  // Add layer to scene
  void AddLayer(Layer &&l);
  // Remove layer from scene
  void RemoveLayer(const std::string &name);

  SDL_Texture *Render(SDL_Renderer *renderer, Layer *layer, Tileset *tileset,
                      bool accumulate = false);
  void Render(SDL_Renderer *renderer);

 private:
  std::vector<Layer> layers_;
  std::vector<Tileset> tile_sets_;
  int canvas_width_;
  int canvas_height_;

  SDL_Texture *scene_texture_ = nullptr;
};

}  // namespace CSPill::EngineCore

namespace nlohmann {
template <>
struct adl_serializer<CSPill::EngineCore::Layer> {
  static CSPill::EngineCore::Layer from_json(const json &j) {
    if (!j.contains("tileset")) {
      return {j.at("name"), j.at("data")};
    }
    return {j.at("name"), j.at("tileset"), j.at("data")};
  }

  static void to_json(json &j, const CSPill::EngineCore::Layer &l) {
    j["name"] = l.GetName();
    j["tileset"] = l.GetTileset();
    j["data"] = l.GetData();
  }
};

template <>
struct adl_serializer<CSPill::EngineCore::Tileset> {
  static CSPill::EngineCore::Tileset from_json(const json &j) {
    return {j.at("name"), j.at("imagewidth"), j.at("imageheight"),
            j.at("tilewidth"), j.at("tileheight")};
  }

  static void to_json(json &j, const CSPill::EngineCore::Tileset &t) {
    j["name"] = t.GetName();
    j["imagewidth"] = t.GetImageWidth();
    j["imageheight"] = t.GetImageHeight();
    j["tilewidth"] = t.GetTileWidth();
    j["tileheight"] = t.GetTileHeight();
  }
};

template <>
struct adl_serializer<CSPill::EngineCore::Scene> {
  static CSPill::EngineCore::Scene from_json(const json &j) {
    return {j.at("layers"), j.at("tilesets"), j.at("canvas").at("width"),
            j.at("canvas").at("height")};
  }

  static void to_json(json &j, const CSPill::EngineCore::Scene &s) {
    j["layers"] = s.GetLayers();
    j["tilesets"] = s.GetTileSets();
    j["canvas"]["width"] = s.GetCanvasWidth();
    j["canvas"]["height"] = s.GetCanvasHeight();
  }
};

}  // namespace nlohmann

#endif  // CSPILLENGINE_ENGINE_INCLUDE_CORE_SCENE_H_
