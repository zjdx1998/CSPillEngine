//
// Created by Jeromy Zhang on 4/6/23.
//

#include "ResourceManager.h"

#include <fstream>
#include <filesystem>
#include <iostream>

namespace CSPill::EngineCore {

namespace fs = std::filesystem;

ResourceManager::ResourceManager() {
  if (TTF_Init() == -1) {
    std::cerr << "Could not initialize SDL2_ttf, error: " << TTF_GetError()
              << std::endl;
    return;
  }
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    std::cerr << "Could not initialize SDL2_mixer, error: " << Mix_GetError()
              << std::endl;
    return;
  }
}

ResourceManager &ResourceManager::GetInstance() {
  static ResourceManager instance;
  return instance;
}

void ResourceManager::LoadResources(std::string_view folder_path) {
  for (const auto &directory : fs::recursive_directory_iterator(folder_path)) {
    if (fs::is_regular_file(directory)) {
      const auto &extension = directory.path().extension();
      const auto &filename = directory.path().filename();
      if (extension == ".ttf") {
        if (fonts_.find(filename) != fonts_.end()) continue;
        fonts_[filename] = TTF_OpenFont(directory.path().c_str(), 20);
      }
      if (extension == ".wav") {
        if (audios_.find(filename) != audios_.end()) continue;
        audios_[filename] = Mix_LoadWAV(directory.path().c_str());
      }
      if (extension == ".scene") {
        if (scenes_.find(filename) != scenes_.end()) continue;
        std::ifstream scene_in(directory.path().c_str());
        json scene_json;
        scene_in >> scene_json;
        scenes_[filename] = std::make_unique<Scene>(scene_json.get<Scene>());
        scene_in.close();
      }
    }
  }
}

Mix_Chunk *ResourceManager::LoadAudio(const std::string &audio_name) {
  if (audios_.find(audio_name) == audios_.end()) {
    std::cerr << "Not found " << audio_name << "!" << std::endl;
    return nullptr;
  }
  std::cout << "Load " << audio_name << " successfully!\n";
  return audios_[audio_name];
}

TTF_Font *ResourceManager::LoadFont(const std::string &font_name) {
  if (fonts_.find(font_name) == fonts_.end()) {
    std::cerr << "Not found " << font_name << "!" << std::endl;
    return nullptr;
  }
  return fonts_[font_name];
}

std::vector<std::string> ResourceManager::GetAudioResourceNames() {
  std::vector<std::string> audio_names;
  for (const auto &[name, audio] : audios_) {
    audio_names.push_back(name);
  }
  return audio_names;
}

std::vector<std::string> ResourceManager::GetFontResourceNames() {
  std::vector<std::string> font_names;
  for (const auto &[name, font] : fonts_) {
    font_names.push_back(name);
  }
  return font_names;
}

Scene *ResourceManager::LoadScene(const std::string &scene_name) {
  if (scenes_.find(scene_name) == scenes_.end()) {
    std::cerr << "Not found " << scene_name << "!" << std::endl;
    return nullptr;
  }
  return scenes_[scene_name].get();
}

void ResourceManager::ReleaseAll() {
  for (const auto &font : fonts_) TTF_CloseFont(font.second);
  for (const auto &audio : audios_) Mix_FreeChunk(audio.second);
  fonts_.clear();
  audios_.clear();
}

ResourceManager::~ResourceManager() {
  ReleaseAll();
  TTF_Quit();
  Mix_CloseAudio();
  Mix_Quit();
}

}  // namespace CSPill::EngineCore