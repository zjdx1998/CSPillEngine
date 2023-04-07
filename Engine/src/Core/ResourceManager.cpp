//
// Created by Jeromy Zhang on 4/6/23.
//

#include "ResourceManager.h"

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