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
  std::vector<std::string> GetAudioResourceNames();
  std::vector<std::string> GetFontResourceNames();

 private:
  ResourceManager();
  std::unordered_map<std::string, TTF_Font *> fonts_;
  std::unordered_map<std::string, Mix_Chunk *> audios_;
};

}  // namespace CSPill::EngineCore

#endif  // CSPILLENGINE_ENGINE_INCLUDE_CORE_RESOURCEMANAGER_H_
