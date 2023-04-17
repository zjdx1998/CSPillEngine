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
  /**
   * Constructor of Layer.
   * @param name a string of name of the layer
   * @param tileset a string of name of the tileset
   * @param data a list of int to represent the tile map
   * @param bkg_color SDL_Color of the layer, the default value is 255, 255,
   * 255, 255
   */
  Layer(std::string name, std::string tileset, const std::vector<int> &data);

  /**
   * Constructor of Layer.
   * @param name a string of name of the layer
   * @param data a list of int to represent the tile map
   * @param bkg_color SDL_Color of the layer, the default value is 255, 255,
   * 255, 255
   */
  Layer(std::string name, const std::vector<int> &data);

  /**
   * Get the name of the layer.
   * @return a string of name of the layer
   */
  [[nodiscard]] std::string_view GetName() const;

  /**
   * Set the name of the layer.
   * @param name a string of the target name of the layer
   */
  void SetName(const std::string &name);

  /**
   * Get the name of the tileset.
   * @return a string of name of the tileset
   */
  [[nodiscard]] std::string_view GetTileset() const;

  /**
   * Set the name of the tileset.
   * @param tileset a string of the target name of the tileset
   */
  void SetTileset(const std::string &tileset);

  /**
   * Get the data of the layer.
   * @return a list of int, which represents the tile map
   */
  [[nodiscard]] std::vector<int> &Data();

  /**
   * Get the data of the layer.
   * @return a list of int, which represents the tile map
   */
  [[nodiscard]] const std::vector<int> &GetData() const;

  /**
   * Set the member variable data to data.
   * @param data a list of int, which represents the tile map
   */
  void SetData(const std::vector<int> &data);

  /**
   * Override == operator.
   * @param rhs Layer object reference
   * @return true if name and tileset are equal, otherwise false
   */
  bool operator==(const Layer &rhs) const;

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
  /**
   * Constructor of Tileset.
   * @param name a string of name of the Tileset
   * @param image_width int, the width of the Tileset
   * @param image_height int, the height of the Tileset
   * @param tile_width int, the width of each tile
   * @param tile_height int, the height of each tile
   */
  Tileset(std::string name, int image_width, int image_height, int tile_width,
          int tile_height);

  /**
   * Get the name of the Tileset.
   * @return a string of the name of Tileset
   */
  [[nodiscard]] std::string_view GetName() const;

  /**
   * Set the name of the Tileset to name.
   * @param name a string of the target name of the Tileset
   */
  void SetName(const std::string &name);

  /**
   * Get the width of the Tileset.
   * @return int, width of the Tileset
   */
  [[nodiscard]] int GetImageWidth() const;

  /**
   * Set the width of the Tileset to image_width.
   * @param image_width int, the target width of the Tileset
   */
  void SetImageWidth(int image_width);

  /**
   * Get the height of the Tileset.
   * @return int, height of the Tileset
   */
  [[nodiscard]] int GetImageHeight() const;

  /**
   * Set the height of the Tileset to image_height.
   * @param image_width int, the target height of the Tileset
   */
  void SetImageHeight(int image_height);

  /**
   * Get the tile width of the Tileset.
   * @return int, tile width of the Tileset
   */
  [[nodiscard]] int GetTileWidth() const;

  /**
   * Get the tile width of the Tileset.
   * @return int, tile width of the Tileset
   */
  int &TileWidth();

  /**
   * Set the tile width of the Tileset to tile_width.
   * @param image_width int, the target tile width of the Tileset
   */
  void SetTileWidth(int tile_width);

  /**
   * Get the tile height of the Tileset.
   * @return int, tile height of the Tileset
   */
  [[nodiscard]] int GetTileHeight() const;

  /**
   * Get the tile height of the Tileset.
   * @return int, tile height of the Tileset
   */
  int &TileHeight();

  /**
   * Set the tile height of the Tileset to tile_height.
   * @param image_width int, the target tile height of the Tileset
   */
  void SetTileHeight(int tile_height);

 private:
  std::string name_;
  int image_width_;
  int image_height_;
  int tile_width_;
  int tile_height_;
};

/**
 * \brief Tile map of the game
 *
 * A scene object contains of list of layers and Tilesets. It is an object that
 * can be loaded to ResourceManager.
 */
class Scene {
 public:
  /**
   * Constructor of Scene.
   * @param canvas_width int, the width of the canvas
   * @param canvas_height int, the height of the canvas
   */
  Scene(int canvas_width, int canvas_height,
        const SDL_Color &bkg_color = {255, 255, 255, 255});

  /**
   * Constructor of Scene.
   * @param layers a vector, which represents a list of layer objects
   * @param tile_sets a vector, which represents a list of Tileset objects
   * @param canvas_width int, the width of the canvas
   * @param canvas_height int, the height of the canvas
   */
  Scene(const std::vector<Layer> &layers, const std::vector<Tileset> &tile_sets,
        int canvas_width, int canvas_height,
        const SDL_Color &bkg_color = {255, 255, 255, 255});

  /**
   * Destructor of Scene.
   */
  ~Scene();

  /**
   * Get the layers contained by Scene.
   * @return a vector, which represents a list of layer objects
   */
  [[nodiscard]] std::vector<Layer> &Layers();

  /**
   * Get the layers contained by Scene.
   * @return a vector, which represents a list of layer objects
   */
  [[nodiscard]] const std::vector<Layer> &GetLayers() const;

  /**
   * Set the layers contained by Scene.
   * @param layers a vector, which represents a target list of layer objects
   */
  void SetLayers(const std::vector<Layer> &layers);

  /**
   * Get the Tileset contained by Scene.
   * @return a vector, which represents a list of Tileset objects
   */
  [[nodiscard]] const std::vector<Tileset> &GetTileSets() const;

  /**
   * Get the Tileset contained by Scene.
   * @return a vector, which represents a list of Tileset objects
   */
  [[nodiscard]] std::vector<Tileset> &TileSets();

  /**
   * Set the Tilesets contained by Scene.
   * @param tile_sets a vector, which represents a target list of Tileset
   * objects
   */
  void SetTileSets(const std::vector<Tileset> &tile_sets);

  /**
   * Add a Tileset to current Scene.
   * @param t a Tileset that requires to add
   */
  void AddTileSet(Tileset &&t);

  /**
   * Get canvas width.
   * @return int, the width of the canvas
   */
  [[nodiscard]] int GetCanvasWidth() const;

  /**
   * Set canvas width to canvas_width.
   * @param canvas_width int, the target width of the canvas
   */
  void SetCanvasWidth(int canvas_width);

  /**
   * Get canvas height.
   * @return int, the height of the canvas
   */
  [[nodiscard]] int GetCanvasHeight() const;

  /**
   * Set canvas height to canvas_height.
   * @param canvas_height int, the target height of the canvas
   */
  void SetCanvasHeight(int canvas_height);

  /**
   * Add a Layer to Scene.
   * @param l a Layer that required to be added
   */
  void AddLayer(Layer &&l);

  /**
   * Remove a layer by name.
   * @param name a string of the name of the layer that requested to remove
   */
  void RemoveLayer(const std::string &name);

  /**
 * Get the background color of current layer.
 * @return SDL_Color, which represents the background color
 */
  [[nodiscard]] const SDL_Color &GetBackgroundColor() const;

  /**
   * Get the background color of current layer.
   * @return SDL_Color, which represents the background color
   */
  SDL_Color &BackgroundColor();

  /**
   * Set background color to background_color
   * @param background_color SDL_Color which represents the target background
   * color
   */
  void SetBackgroundColor(const SDL_Color &background_color);

  /**
   * Render a specific Layer or Tileset.
   * @param renderer SDL_Renderer that is going to be used
   * @param layer target rendering Layer
   * @param tileset target rendering Tileset
   * @param accumulate bool, determine if it accumulate render, default to be
   * false
   * @return SDL_Texture that is going to be rendered
   */
  SDL_Texture *Render(SDL_Renderer *renderer, Layer *layer, Tileset *tileset,
                      bool accumulate = false);

  /**
   * Render all layers and Tilesets in Scene.
   * @param renderer SDL_Renderer that is going to be used
   * @return SDL_Texture that is going to be rendered
   */
  SDL_Texture *Render(SDL_Renderer *renderer);

 private:
  std::vector<Layer> layers_;
  std::vector<Tileset> tile_sets_;
  int canvas_width_;
  int canvas_height_;
  SDL_Color background_color_{};

  SDL_Texture *scene_texture_ = nullptr;
};

}  // namespace CSPill::EngineCore

namespace nlohmann {
/**
 * \brief JSON parsing helper struct for Layer
 */
template<>
struct adl_serializer<CSPill::EngineCore::Layer> {
  /**
   * Retrieve the information in JSON object j.
   * @param j JOSN object that is going to be parsed
   * @return Layer object that contained by JSON object
   */
  static CSPill::EngineCore::Layer from_json(const json &j) {
    if (!j.contains("tileset")) {
      return {j.at("name"), j.at("data")};
    }
    return {j.at("name"), j.at("tileset"), j.at("data")};
  }

  /**
   * Parse the Layer object to JSON object j.
   * @param j JOSN object that is going to be parsed
   * @param l Layer object requested to be parsed
   */
  static void to_json(json &j, const CSPill::EngineCore::Layer &l) {
    j["name"] = l.GetName();
    j["tileset"] = l.GetTileset();
    j["data"] = l.GetData();
  }
};

/**
 * \brief JSON parsing helper struct for Tileset
 */
template<>
struct adl_serializer<CSPill::EngineCore::Tileset> {
  /**
   * Retrieve the information in JSON object j.
   * @param j JOSN object that is going to be parsed
   * @return Tileset object that contained by JSON object
   */
  static CSPill::EngineCore::Tileset from_json(const json &j) {
    return {j.at("name"), j.at("imagewidth"), j.at("imageheight"),
            j.at("tilewidth"), j.at("tileheight")};
  }

  /**
   * Parse the Tileset object to JSON object j.
   * @param j JOSN object that is going to be parsed
   * @param t Tileset object requested to be parsed
   */
  static void to_json(json &j, const CSPill::EngineCore::Tileset &t) {
    j["name"] = t.GetName();
    j["imagewidth"] = t.GetImageWidth();
    j["imageheight"] = t.GetImageHeight();
    j["tilewidth"] = t.GetTileWidth();
    j["tileheight"] = t.GetTileHeight();
  }
};

/**
 * \brief JSON parsing helper struct for Scene
 */
template<>
struct adl_serializer<CSPill::EngineCore::Scene> {
  /**
   * Retrieve the information in JSON object j.
   * @param j JOSN object that is going to be parsed
   * @return Scene object that contained by JSON object
   */
  static CSPill::EngineCore::Scene from_json(const json &j) {
    SDL_Color color = {255, 255, 255, 255};
    if (j.contains("color")) {
      json json_color = j.at("color");
      color.r = json_color.at("r");
      color.g = json_color.at("g");
      color.b = json_color.at("b");
      color.a = json_color.at("a");
    }
    return {j.at("layers"), j.at("tilesets"), j.at("canvas").at("width"),
            j.at("canvas").at("height"), color};
  }

  /**
   * Parse the Scene object to JSON object j.
   * @param j JOSN object that is going to be parsed
   * @param t Scene object requested to be parsed
   */
  static void to_json(json &j, const CSPill::EngineCore::Scene &s) {
    j["layers"] = s.GetLayers();
    j["tilesets"] = s.GetTileSets();
    j["canvas"]["width"] = s.GetCanvasWidth();
    j["canvas"]["height"] = s.GetCanvasHeight();
    json json_color;
    json_color["r"] = s.GetBackgroundColor().r;
    json_color["g"] = s.GetBackgroundColor().g;
    json_color["b"] = s.GetBackgroundColor().b;
    json_color["a"] = s.GetBackgroundColor().a;
    j["color"] = json_color;
  }
};

}  // namespace nlohmann

#endif  // CSPILLENGINE_ENGINE_INCLUDE_CORE_SCENE_H_
