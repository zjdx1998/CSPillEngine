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
 *
 * The ResourceManager is a singleton class. It can load and record different
 * kinds of resources such as audio, image, font, and Scene(i.e tilemap) for the
 * game.
 */
class ResourceManager {
 public:
  ResourceManager(ResourceManager const &) = delete;

  void operator=(ResourceManager const &) = delete;

  /**
   * GetInstance will return the reference to ResourceManager object if it is
   * created. Otherwise, create the object and return.
   * @return the reference to ResourceManager object
   */
  static ResourceManager &GetInstance();

  /**
   * LoadResources takes a folder path as an argument and load all resources
   * with certain formats to ResourceManager. The file format that supports are
   * audio ("wav"), fonts("ttf"), image("bmp", "jpg", "png", "tif"), and
   * Scene("scene"). If the folder_path is not found, it will print an error of
   * "folder_path is not found".
   * @param folder_path a string, the ResourceManager will search and load
   * resources
   */
  void LoadResources(std::string_view folder_path);

  /**
   * Destroy all loaded resources.
   */
  void ReleaseAll();

  /**
   * Destructor of ResourceManager.
   */
  ~ResourceManager();

  /**
   * Search for the audio resource by audio_name. If audio_name is not loaded to
   * the ResourceManager yet, it will log "Not found audio_name" and return a
   * nullptr. Otherwise return a pointer to Mix_Chunk, which is an audio
   * resource that can be played.
   * @param audio_name a string, the name of the audio
   * @return a pointer to Mix_Chunk, if audio_name not found, nullptr is
   * returned instead
   */
  Mix_Chunk *LoadAudio(const std::string &audio_name);

  /**
   * Search for the font resource by font_name. If font_name is not loaded to
   * the ResourceManager yet, it will log "Not found font_name" and return a
   * nullptr. Otherwise return a pointer to TTF_Font, which is a font resource
   * that can be displayed.
   * @param font_name a string, the name of the font
   * @return a pointer to TTF_Font, if font_name not found, nullptr is returned
   * instead
   */
  TTF_Font *LoadFont(const std::string &font_name);

  /**
   * Search for the image resource by image_name. If image_name is not loaded to
   * the ResourceManager yet, it will log "Not found image_name" and return a
   * nullptr. Otherwise return a pointer to SDL_Texture, which is an image
   * resource that can be displayed.
   * @param image_name a string, the name of the image
   * @return a pointer to SDL_Texture, if image_name not found, nullptr is
   * returned instead
   */
  SDL_Texture *LoadImage(const std::string &image_name);

  /**
   * Search for the tile map resource by scene_name. If scene_name is not loaded
   * to the ResourceManager yet, it will log "Not found scene_name" and return a
   * nullptr. Otherwise, return a pointer to Scene, which is a tile map resource
   * that can be displayed.
   * @param scene_name a string, the name of the scene
   * @return a pointer to Scene, if scene_name not found, nullptr is returned
   * instead
   */
  Scene *LoadScene(const std::string &scene_name);

  /**
   * Get a list of audio names that loaded to ResourceManager.
   * @return a vector that contains all the audio names
   */
  std::vector<std::string> GetAudioResourceNames();

  /**
   * Get a list of font names that loaded to ResourceManager.
   * @return a vector that contains all the font names
   */
  std::vector<std::string> GetFontResourceNames();

  /**
   * Get a list of scene names that loaded to ResourceManager.
   * @return a vector that contains all the scene names
   */
  std::vector<std::string> GetSceneNames();

  /**
   * Get a list of image names that loaded to ResourceManager.
   * @return a vector that contains all the image names
   */
  std::vector<std::string> GetImageNames();

  /**
   * Getter function for SDL_Renderer.
   * @return a pointer for SDL_Renderer object
   */
  SDL_Renderer *GetRenderer();

  /**
   * Setter function for SDL_Renderer.
   * @param renderer a pointer to SDL_renderer object
   */
  void SetRenderer(SDL_Renderer *renderer);

  // Editor Only Below

  /**
   * Load active scene. The default active scene is "default.scene".
   * @return a pointer to Scene object
   */
  Scene *ActiveScene();

  /**
   * Get the name of the active scene. If the active scene name is not found
   * in the ResourceManager, it will return an empty string. Otherwise, it
   * returns the string of the active scene.
   * @return a string of name of the active scene, if the name is not found,
   *          return an empty string instead
   */
  [[nodiscard]] std::string GetActiveSceneName() const;

  /**
   * Get the file path to the active scene. If the active scene name is not
   * found in the ResourceManager, it will return a nullptr. Otherwise, it
   * returns the string of the active scene.
   * @return a string of the file path to the active scene, if the name is not
   * found, return an empty string instead
   */
  [[nodiscard]] std::string GetActiveScenePath() const;

  /**
   * Set active scene to scene_name. If the scene_name is not found
   * in the ResourceManager, it will return false. Otherwise, it sets active
   * scene to scene_name and returns true.
   * @param scene_name a string of scene name that is going to be set as active
   * scene
   * @return true if scene_name is loaded already, otherwise false
   */
  bool SetActiveScene(const std::string &scene_name);

  /**
   * Get the active layer of a Scene.
   * @param tileset_name name of the tileset of the layer, if empty then default is the current active tileset.
   * @return a pointer of layer if the layer is found, otherwise nullptr
   */
  Layer *ActiveLayer(std::string_view tileset_name = "");

  /**
   * Get the active Tileset of a Scene.
   * @param tileset_name name of the tileset, if empty then default is the current active tileset.
   * @return a pointer of Tileset if the layer is found, otherwise nullptr
   */
  Tileset *ActiveTileset(std::string_view tileset_name = "");

  /**
   * Set the active tileset to active_name.
   * @param active_name a string of the name of the tileset that is going
   *        to be set active
   */
  void SetActiveTileset(const std::string &active_name);

  /**
   * Get the name of the active tileset.
   * @return a string of the active tileset
   */
  [[nodiscard]] std::string GetActiveTilesetName() const;

  /**
   * Add a texture by name.
   * @param name a string of the name of the texture
   * @param tile unique_ptr to the texture
   */
  void AddTile(
      std::string_view name,
      std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> tile);

  /**
   * Get the texture by name.
   * @param name a string of the name of the texture
   * @return a pointer to the texture if the name exists, otherwise nullptr
   */
  SDL_Texture *QueryTexture(std::string_view name);

  /**
   * Destroy the SDL_texture loaded.
   */
  void ClearTiles();

 private:
  SDL_Renderer *renderer_;

  ResourceManager();

  std::unordered_map<std::string, TTF_Font *> fonts_;
  std::unordered_map<std::string, Mix_Chunk *> audios_;
  std::unordered_map<std::string,
                     std::pair<std::string, std::unique_ptr<Scene>>>
      scenes_;
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
