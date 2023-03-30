//
// Created by Jeromy Zhang on 3/28/23.
//

#ifndef CSPILLENGINE_ENGINE_INCLUDE_CORE_SCENE_H_
#define CSPILLENGINE_ENGINE_INCLUDE_CORE_SCENE_H_

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace CSPill::EngineCore {

/**
 * \brief Layer is a basic unit of Scene.
 *
 * Each layer has a unique name in a Scene, and it should use one tileset.
 * The data is a vector. -1 represents empty.
 * Y.X represents Column Y and Row X, if X is zero, then it's an integer Y.
 */
class Layer {
 public:
  Layer(std::string name, std::string tileset, const std::vector<double> &data);
  [[nodiscard]] std::string_view GetName() const;
  void SetName(const std::string &name);
  [[nodiscard]] std::string_view GetTileset() const;
  void SetTileset(const std::string &tileset);
  [[nodiscard]] const std::vector<double> &GetData() const;
  void SetData(const std::vector<double> &data);
  [[nodiscard]] bool IsVisible() const;
  void SetVisible(bool visible);

 private:
  std::string name_;
  std::string tileset_;
  std::vector<double> data_;
  bool visible_ = true;
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
  Tileset(std::string name, int image_width, int image_height, int tile_width, int tile_height);
  [[nodiscard]] std::string_view GetName() const;
  void SetName(const std::string &name);
  [[nodiscard]] int GetImageWidth() const;
  void SetImageWidth(int image_width);
  [[nodiscard]] int GetImageHeight() const;
  void SetImageHeight(int image_height);
  [[nodiscard]] int GetTileWidth() const;
  void SetTileWidth(int tile_width);
  [[nodiscard]] int GetTileHeight() const;
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
  Scene(const std::vector<Layer> &layers, const std::vector<Tileset> &tile_sets, int canvas_width, int canvas_height);
  [[nodiscard]] const std::vector<Layer> &GetLayers() const;
  void SetLayers(const std::vector<Layer> &layers);
  [[nodiscard]] const std::vector<Tileset> &GetTileSets() const;
  void SetTileSets(const std::vector<Tileset> &tile_sets);
  [[nodiscard]] int GetCanvasWidth() const;
  void SetCanvasWidth(int canvas_width);
  [[nodiscard]] int GetCanvasHeight() const;
  void SetCanvasHeight(int canvas_height);
 private:
  std::vector<Layer> layers_;
  std::vector<Tileset> tile_sets_;
  int canvas_width_;
  int canvas_height_;
};

}  // namespace CSPill::EngineCore

#endif //CSPILLENGINE_ENGINE_INCLUDE_CORE_SCENE_H_
