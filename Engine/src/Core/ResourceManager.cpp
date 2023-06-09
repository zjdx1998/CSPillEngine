//
// Created by Jeromy Zhang on 4/6/23.
//

#include "ResourceManager.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "GameObject.h"
#include "SDL_image.h"
#include "Utils.h"

namespace CSPill::EngineCore {

namespace fs = std::filesystem;

ResourceManager::ResourceManager() {
  if (TTF_Init() == -1) {
    std::cerr << "Could not initialize SDL2_ttf, error: " << TTF_GetError()
              << std::endl;
  }
  if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF) !=
      (IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF)) {
    std::cerr << "Failed to initialize SDL_image: " << IMG_GetError()
              << std::endl;
  }
  int Mix_flags = MIX_INIT_MP3 | MIX_INIT_FLAC;
  int Mix_initted = Mix_Init(Mix_flags);
  if ((Mix_initted & Mix_flags) != Mix_flags) {
    std::cerr << "Mix_Init: Failed to init required ogg and mod support!\n";
    std::cerr << "Mix_Init: %s\n" << Mix_GetError() << std::endl;
  }
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
    std::cerr << "Could not initialize SDL2_mixer, error: " << Mix_GetError()
              << std::endl;
  }
  renderer_ = nullptr;
}

ResourceManager &ResourceManager::GetInstance() {
  static ResourceManager instance;
  return instance;
}

SDL_Renderer *ResourceManager::GetRenderer() { return renderer_; }

void ResourceManager::SetRenderer(SDL_Renderer *renderer) {
  this->renderer_ = renderer;
}

void ResourceManager::LoadResources(std::string_view folder_path) {
  if (!fs::exists(folder_path)) {
    std::cerr << "Path: " << folder_path << " is not valid!" << std::endl;
    return;
  }
  for (const auto &directory : fs::recursive_directory_iterator(folder_path)) {
    if (fs::is_regular_file(directory)) {
      const auto &extension = directory.path().extension();
      const auto &filename = directory.path().filename().string();
      if (extension == ".ttf") {
        if (fonts_.find(filename) != fonts_.end()) continue;
        fonts_[filename] = TTF_OpenFont(directory.path().string().c_str(), 20);
      }
      if (extension == ".wav") {
        if (audios_.find(filename) != audios_.end()) continue;
        audios_[filename] = Mix_LoadWAV(directory.path().string().c_str());
      }
      if (extension == ".scene") {
        if (scenes_.find(filename) != scenes_.end()) continue;
        std::ifstream scene_in(directory.path().string().c_str());
        json scene_json;
        scene_in >> scene_json;
        scenes_[filename] = {directory.path().string(),
                             std::make_unique<Scene>(scene_json.get<Scene>())};
        scene_in.close();
      }
      if (extension == ".bmp") {
        if (images_.find(filename) != images_.end()) continue;
        if (SDL_Surface *bmp_surface =
                SDL_LoadBMP(directory.path().string().c_str())) {
          SDL_SetColorKey(bmp_surface, SDL_TRUE,
                          SDL_MapRGB(bmp_surface->format, 0, 0, 0));
          images_[filename] =
              SDL_CreateTextureFromSurface(renderer_, bmp_surface);
          SDL_FreeSurface(bmp_surface);
        }
      }
      if (::EngineCore::Utils::isIn(extension, ".png", ".jpg", ".tif")) {
        if (images_.find(filename) != images_.end()) continue;
        if (SDL_Surface *surface =
                IMG_Load(directory.path().string().c_str())) {
          SDL_SetColorKey(surface, SDL_TRUE,
                          SDL_MapRGB(surface->format, 0, 0, 0));
          images_[filename] = SDL_CreateTextureFromSurface(renderer_, surface);
          SDL_FreeSurface(surface);
        }
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

std::vector<std::string> ResourceManager::GetSceneNames() {
  std::vector<std::string> scene_names;
  for (const auto &[name, scene] : scenes_) {
    scene_names.push_back(name);
  }
  return std::move(scene_names);
}

std::vector<std::string> ResourceManager::GetImageNames() {
  std::vector<std::string> image_names;
  for (const auto &[name, scene] : images_) {
    image_names.push_back(name);
  }
  return std::move(image_names);
}

Scene *ResourceManager::LoadScene(const std::string &scene_name) {
  if (scenes_.find(scene_name) == scenes_.end()) {
    std::cerr << "Scene: " << scene_name << " Not Found!" << std::endl;
    return nullptr;
  }
  return scenes_[scene_name].second.get();
}

SDL_Texture *ResourceManager::LoadImage(const std::string &image_name) {
  if (images_.find(image_name) == images_.end()) {
    std::cerr << "Image: " << image_name << " Not Found!" << std::endl;
    return nullptr;
  }
  return images_[image_name];
}

Scene *ResourceManager::ActiveScene() { return LoadScene(active_scene_); }

bool ResourceManager::SetActiveScene(const std::string &scene_name) {
  if (scenes_.find(scene_name) == scenes_.end()) {
    std::cerr << "Scene " << scene_name << "Not Found!" << std::endl;
    return false;
  }
  active_scene_ = scene_name;
  return true;
}

std::string ResourceManager::GetActiveSceneName() const {
  return active_scene_;
}

std::string ResourceManager::GetActiveScenePath() const {
  if (scenes_.find(active_scene_) == scenes_.end()) {
    std::cerr << "Scene " << active_scene_ << "Not Found!" << std::endl;
    return "";
  }
  return scenes_.at(active_scene_).first;
}

void ResourceManager::SetActiveTileset(const std::string &active_name) {
  active_tileset_ = active_name;
}

Tileset *ResourceManager::ActiveTileset(std::string_view tileset_name) {
  if (tileset_name.empty()) tileset_name = active_tileset_;
  if (tileset_name.empty()) return nullptr;
  for (auto &i : ActiveScene()->TileSets()) {
    if (i.GetName() == tileset_name) return &i;
  }
  return nullptr;
}

std::string ResourceManager::GetActiveTilesetName() const {
  return active_tileset_;
}

Layer *ResourceManager::ActiveLayer(std::string_view tileset_name) {
  if (tileset_name.empty()) tileset_name = active_tileset_;
  if (tileset_name.empty()) return nullptr;
  for (auto &i : ActiveScene()->Layers()) {
    if (i.GetTileset() == tileset_name) return &i;
  }
  return nullptr;
}

void ResourceManager::AddTile(
    std::string_view name,
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> tile) {
  tiles_.insert({name.data(), std::move(tile)});
}

SDL_Texture *ResourceManager::QueryTexture(std::string_view name) {
  if (tiles_.find(name.data()) != tiles_.end())
    return tiles_.at(name.data()).get();
  auto split_pos = name.find(::EngineCore::Utils::IMAGE_SPLIT);
  auto tileset_name = name.substr(0, split_pos);
  auto row_and_col_data = std::stoi(std::string(name.substr(
      split_pos + std::string(::EngineCore::Utils::IMAGE_SPLIT).size())));
  auto row_and_col = ::EngineCore::Utils::GetRowAndCol(row_and_col_data);
  if (auto active_layer =
          ResourceManager::GetInstance().LoadImage(std::string(tileset_name))) {
    SDL_Texture *cropped_texture = nullptr;
    if (auto tileset = ActiveTileset(tileset_name)) {
      SDL_Rect src_rect = {row_and_col.second * tileset->GetTileWidth(),
                           row_and_col.first * tileset->GetTileHeight(),
                           tileset->GetTileWidth(), tileset->GetTileHeight()};
      auto cropped =
          ::EngineCore::Utils::CropTexture(renderer_, active_layer, src_rect);
      cropped_texture = cropped.get();
      ResourceManager::GetInstance().AddTile(name, std::move(cropped));
    }
    return cropped_texture;
  }
  return nullptr;
}

void ResourceManager::ClearTiles() {
  ;
  tiles_.clear();
}

void ResourceManager::ReleaseAll() {
  ClearTiles();
  for (const auto &font : fonts_) TTF_CloseFont(font.second);
  for (const auto &audio : audios_) Mix_FreeChunk(audio.second);
  for (const auto &image : images_) SDL_DestroyTexture(image.second);
  fonts_.clear();
  audios_.clear();
  images_.clear();
}

ResourceManager::~ResourceManager() {
  ReleaseAll();
  TTF_Quit();
  Mix_CloseAudio();
  Mix_Quit();
  IMG_Quit();
}
GameObject *ResourceManager::GetActiveCamera() const { return active_camera_; }
void ResourceManager::SetActiveCamera(GameObject *active_camera) {
  active_camera_ = active_camera;
}

}  // namespace CSPill::EngineCore