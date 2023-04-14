//
// Created by Jeromy Zhang on 4/6/23.
//

#ifndef CSPILLENGINE_ENGINE_INCLUDE_CORE_RESOURCEMANAGER_H_
#define CSPILLENGINE_ENGINE_INCLUDE_CORE_RESOURCEMANAGER_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "Scene.h"

namespace CSPill::EngineCore {

/**
 * \brief Resource Manager for both engine and game.
 */
class ResourceManager {
 public:
  static ResourceManager &GetInstance();
  ResourceManager(ResourceManager const &) = delete;
  void operator=(ResourceManager const &) = delete;
  void LoadResources(std::string_view folder_path);
  void ReleaseAll();
  ~ResourceManager();
  Mix_Chunk *LoadAudio(const std::string &audio_name);
  TTF_Font *LoadFont(const std::string &font_name);
  SDL_Texture *LoadImage(const std::string &image_name);
  Scene *LoadScene(const std::string &scene_name);

  std::vector<std::string> GetAudioResourceNames();
  std::vector<std::string> GetFontResourceNames();
  std::vector<std::string> GetSceneNames();
  std::vector<std::string> GetImageNames();

  SDL_Renderer *GetRenderer();
  void SetRenderer(SDL_Renderer *renderer);

  // Editor Only Below

  Scene *ActiveScene();
  [[nodiscard]] std::string GetActiveSceneName() const;
  bool SetActiveScene(const std::string &scene_name);

  Layer *ActiveLayer();

  Tileset *ActiveTileset();
  void SetActiveTileset(const std::string &active_name);
  [[nodiscard]] std::string GetActiveTilesetName() const;

  void AddTile(
      std::string_view name,
      std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> tile);
  SDL_Texture *QueryTexture(std::string_view name);
  void ClearTiles();

 private:
  SDL_Renderer *renderer_;
  ResourceManager();
  std::unordered_map<std::string, TTF_Font *> fonts_;
  std::unordered_map<std::string, Mix_Chunk *> audios_;
  std::unordered_map<std::string, std::pair<std::string, std::unique_ptr<Scene>>> scenes_;
  std::unordered_map<std::string, SDL_Texture *> images_;

  // Editor Only
  std::unordered_map<
      std::string, std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>>
      tiles_;

  // Global Editor Scene Helper Variables
  std::string active_scene_ = "default.scene";
  std::string active_tileset_;
};

}  // namespace CSPill::EngineCore

#endif  // CSPILLENGINE_ENGINE_INCLUDE_CORE_RESOURCEMANAGER_H_
