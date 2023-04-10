//
// Created by Jeromy Zhang on 4/7/23.
//

#include "EditorScene.h"

#include <SDL_image.h>

#include <iostream>
#include <utility>

#include "EditorScene.h"
#include "ImGuiFileDialog.h"
#include "ResourceManager.h"
#include "Utils.h"
#include "imgui.h"

namespace CSPill::Editor {

using EngineCore::ResourceManager;
using EngineCore::Tileset;

// Scene
int SceneUI::SCENE_NUM_ROWS_ = 20;
int SceneUI::SCENE_NUM_COLS_ = 50;
int SceneUI::SCENE_BLOCK_SIZE_ = 50;

// Resources
int TileSetEditorUI::selected_resource_index_ = -1;
int TileSetEditorUI::RESOURCES_NUM_COLS_ = 3;
std::vector<SDL_Texture *> TileSetEditorUI::resource_textures_;
std::vector<CSPill::EngineCore::Tileset> TileSetEditorUI::resource_tilesets_;

SceneUI::SceneUI(std::string title, int width, int height)
    : CSPill::EngineCore::UI(std::move(title), width, height) {
  // allocate memory for the textures
  scene_textures_ =
      std::make_unique<SDL_Texture *[]>(SCENE_NUM_ROWS_ * SCENE_NUM_COLS_);

  // initialize each texture to nullptr
  for (int i = 0; i < SCENE_NUM_ROWS_ * SCENE_NUM_COLS_; ++i) {
    scene_textures_[i] = nullptr;
  }
}

void SceneUI::Render(SDL_Renderer *renderer) {
  ImGui::Begin(this->GetTitle().c_str());
  // Render scene component to SDL texture
  std::string image_filename =
      "../demo/resources/sprites/adventurer-v1.5-Sheet.bmp";
  SDL_Surface *mSurface = SDL_LoadBMP(image_filename.c_str());
  SDL_Texture *mTexture = SDL_CreateTextureFromSurface(renderer, mSurface);

  // Calculate the width and height of each frame in the BMP file
  int frame_width = mSurface->w / 7;
  int frame_height = mSurface->h / 16;

  // Test rendering blocks individually
  for (int row = 0; row < SCENE_NUM_ROWS_; row++) {
    for (int col = 0; col < SCENE_NUM_COLS_; col++) {
      int block_index = row * SCENE_NUM_COLS_ + col;
      if (scene_textures_[block_index] != nullptr) {
        continue;
      }
      // Calculate the position and color of the block
      SDL_Rect block_rect = {col * SCENE_BLOCK_SIZE_, row * SCENE_BLOCK_SIZE_,
                             SCENE_BLOCK_SIZE_, SCENE_BLOCK_SIZE_};
      SDL_Texture *block_texture = SDL_CreateTexture(
          renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
          SCENE_BLOCK_SIZE_, SCENE_BLOCK_SIZE_);

      // Render the BMP texture to the block texture using SDL
      SDL_SetRenderTarget(renderer, block_texture);
      SDL_Rect frame_rect = {0, 0, frame_width, frame_height};
      SDL_Rect dest_rect = {0, 0, SCENE_BLOCK_SIZE_, SCENE_BLOCK_SIZE_};
      SDL_RenderCopy(renderer, mTexture, &frame_rect, nullptr);
      SDL_SetRenderTarget(renderer, nullptr);

      // Store the block texture in the array
      scene_textures_[block_index] = block_texture;
    }
  }

  SDL_FreeSurface(mSurface);
  ImVec2 block_offset(0, 20);

  // Display the block textures in the ImGUI window
  for (int row = 0; row < SCENE_NUM_ROWS_; row++) {
    for (int col = 0; col < SCENE_NUM_COLS_; col++) {
      // Calculate the position and size of the block
      SDL_Rect block_rect = {col * SCENE_BLOCK_SIZE_, row * SCENE_BLOCK_SIZE_,
                             SCENE_BLOCK_SIZE_, SCENE_BLOCK_SIZE_};

      ImGui::SetCursorPos(ImVec2(col * SCENE_BLOCK_SIZE_ + block_offset.x,
                                 row * SCENE_BLOCK_SIZE_ + block_offset.y));
      // Display the block texture in the ImGUI window
      ImGui::Image(
          (void *)(intptr_t)scene_textures_[row * SCENE_NUM_COLS_ + col],
          ImVec2(SCENE_BLOCK_SIZE_, SCENE_BLOCK_SIZE_));
      // Check if a block was clicked
      if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
        std::cout << "clicked: " << row * SCENE_NUM_COLS_ + col << std::endl;
        if (TileSetEditorUI::GetSelectedResourceIndex() != -1) {
          std::cout << "replace with resource: "
                    << TileSetEditorUI::GetSelectedResourceIndex() << std::endl;
          SDL_Texture *current_resource = TileSetEditorUI::GetResourceTexture(
              TileSetEditorUI::GetSelectedResourceIndex());
          scene_textures_[row * SCENE_NUM_COLS_ + col] = current_resource;
          ImGui::SetCursorPos(ImVec2(col * SCENE_BLOCK_SIZE_ + block_offset.x,
                                     row * SCENE_BLOCK_SIZE_ + block_offset.y));
          ImGui::Image((void *)(intptr_t)current_resource,
                       ImVec2(SCENE_BLOCK_SIZE_, SCENE_BLOCK_SIZE_));
        }
      }
    }
  }
  ImGui::End();
}

TileSetEditorUI::TileSetEditorUI(std::string title, int width, int height)
    : CSPill::EngineCore::UI(std::move(title), width, height) {}

int TileSetEditorUI::GetSelectedResourceIndex() {
  return selected_resource_index_;
}

SDL_Texture *TileSetEditorUI::GetResourceTexture(int index) {
  return resource_textures_.at(index);
}

void TileSetEditorUI::Render(SDL_Renderer *renderer) {
  ImGui::Begin(this->GetTitle().c_str());
  // Add Tileset button
  if (ImGui::Button("Add Tileset")) {
    ImGui::OpenPopup("Add Tileset Popup");
  }

  // Add Tileset popup
  if (ImGui::BeginPopup("Add Tileset Popup")) {
    char *file_path = new char[255];
    int tile_width = 0;
    int tile_height = 0;
    int width = 0;
    int height = 0;
    char *tileset_name = new char[127];

    ImGui::InputText("File Path", file_path, sizeof(file_path));
    ImGui::SameLine();
    if (ImGui::Button("Browse")) {
      // TODO: File explorer
    }
    ImGui::InputText("Name", tileset_name, sizeof(tileset_name));
    // std::string file_path_str(file_path);

    // Tile width input
    ImGui::InputInt("Tile Width", &tile_width);

    // Tile height input
    ImGui::InputInt("Tile Height", &tile_height);

    // Tileset width input
    ImGui::InputInt("Tileset Width", &width);

    // Tileset height input
    ImGui::InputInt("Tileset Height", &height);

    // Confirm button
    if (ImGui::Button("Confirm")) {
      // Load texture
      SDL_Surface *surface = IMG_Load(file_path);
      SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
      SDL_FreeSurface(surface);

      // Get the size of the texture
      int texture_width, texture_height;
      SDL_QueryTexture(texture, nullptr, nullptr, &texture_width,
                       &texture_height);

      // Add texture to resource textures array
      resource_textures_.push_back(texture);

      // Add tileset to tilesets array
      resource_tilesets_.emplace_back(tileset_name, file_path, texture_width,
                                      texture_height, tile_width, tile_height);

      if (ResourceManager::GetInstance().ActiveScene()) {
        ResourceManager::GetInstance().ActiveScene()->AddTileSet(Tileset(
            tileset_name, file_path, width, height, tile_width, tile_height));
      }
      ImGui::CloseCurrentPopup();
    }

    if (ResourceManager::GetInstance().ActiveScene()) {
      int active_tileset = -1;
      ImGui::BeginChild("TileSets");
      for (const auto &tileset :
           ResourceManager::GetInstance().ActiveScene()->GetTileSets()) {
        ImGui::Text("%s", tileset.GetName().data());
        ImGui::RadioButton(tileset.GetName().data(), &active_tileset, 0);
      }
      ImGui::EndChild();
    }

    ImGui::EndPopup();
  }

  // Load and display the images in a grid
  for (int i = 0; i < resource_tilesets_.size(); i++) {
    // Display the image in the ImGUI window
    ImGui::PushID(i);
    ImGui::Image(resource_textures_.at(i), ImVec2(100, 100));
    ImGui::PopID();
    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
      selected_resource_index_ = i;
      std::cout << "clicked resource: " << i << std::endl;
    }
    // Add a same-line separator to align images horizontally
    if (i % RESOURCES_NUM_COLS_ != 0) {
      ImGui::SameLine();
    }
  }
  ImGui::End();
}

ResourceManagerUI::ResourceManagerUI(std::string title, int width, int height)
    : CSPill::EngineCore::UI(std::move(title), width, height) {}

void ResourceManagerUI::ResourceManagerRenderSceneLevels() {
  CSPill::EngineCore::ResourceManager &resource_manager =
      CSPill::EngineCore::ResourceManager::GetInstance();
  std::vector<std::string> scene_names = resource_manager.GetSceneNames();
  if (!scene_names.empty()) {
    if (ImGui::TreeNode("Scenes")) {
      for (const std::string &scene_name : scene_names) {
        if (ImGui::Selectable(scene_name.c_str())) {
          resource_manager.SetActiveScene(scene_name);
        }

        bool is_selected = scene_name == resource_manager.GetActiveSceneName();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
                            ImVec2(0, ImGui::GetStyle().ItemSpacing.y));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::SameLine(ImGui::GetWindowWidth() -
                        ImGui::GetTextLineHeightWithSpacing() -
                        ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::RadioButton("", is_selected);
        ImGui::PopStyleVar(2);
        if (scene_name == resource_manager.GetActiveSceneName()) {
          if (auto scene = ResourceManager::GetInstance().ActiveScene()) {
            std::unordered_map<std::string, const Tileset *> tilesets;
            for (const auto &tileset : scene->GetTileSets()) {
              tilesets[tileset.GetName().data()] = &tileset;
            }
            if (ImGui::TreeNode("Layers")) {
              for (const auto &layer : scene->GetLayers()) {
                if (ImGui::TreeNode(layer.GetName().data())) {
                  if (ImGui::TreeNode(layer.GetTileset().empty()
                                          ? "N/A"
                                          : layer.GetTileset().data())) {
                    if (!layer.GetTileset().empty()) {
                      ImGui::Text("File: %s",
                                  tilesets[layer.GetTileset().data()]
                                      ->GetImage()
                                      .c_str());
                      ImGui::Text(
                          "Width: %d",
                          tilesets[layer.GetTileset().data()]->GetImageWidth());
                      ImGui::Text("Height: %d",
                                  tilesets[layer.GetTileset().data()]
                                      ->GetImageHeight());
                      ImGui::Text(
                          "Tile Width: %d",
                          tilesets[layer.GetTileset().data()]->GetTileWidth());
                      ImGui::Text(
                          "Tile Height: %d",
                          tilesets[layer.GetTileset().data()]->GetTileHeight());
                    }
                    ImGui::TreePop();
                  }
                  ImGui::TreePop();
                }
              }
              ImGui::TreePop();
            }
          }
        }
      }
      ImGui::TreePop();
    }
  }
}

void ResourceManagerUI::Render(SDL_Renderer *renderer) {
  ImGui::Begin(this->GetTitle().c_str());

  CSPill::EngineCore::ResourceManager &resource_manager =
      CSPill::EngineCore::ResourceManager::GetInstance();

  if (ImGui::Button("Add Asset")) {
    ImGui::OpenPopup("Add Asset");
  }
  // Add resource popup
  if (ImGui::BeginPopup("Add Asset")) {
    static std::string file_path;

    ImGui::InputText("File Path", file_path.data(), file_path.capacity());
    ImGui::SameLine();
    if (ImGui::Button("Browse")) {
      // open Dialog Simple
      ImGuiFileDialog::Instance()->OpenDialog("FileBrowser", "Choose Folder",
                                              nullptr, ".");
    }

    // Display file browser
    if (ImGuiFileDialog::Instance()->Display("FileBrowser")) {
      if (ImGuiFileDialog::Instance()->IsOk()) {
        file_path = ImGuiFileDialog::Instance()->GetFilePathName();
      }
      ImGuiFileDialog::Instance()->Close();
    }

    // Confirm button
    if (ImGui::Button("Confirm")) {
      resource_manager.LoadResources(file_path);
      ImGui::CloseCurrentPopup();
    }

    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }

  // Show font resources
  std::vector<std::string> font_names = resource_manager.GetFontResourceNames();
  if (!font_names.empty()) {
    if (ImGui::TreeNode("Fonts")) {
      for (const std::string &font : font_names) {
        if (ImGui::Selectable(font.c_str())) {
          // TODO: click event
        }
      }
      ImGui::TreePop();
    }
  }

  // Show audio resources
  std::vector<std::string> audio_names =
      resource_manager.GetAudioResourceNames();
  if (!audio_names.empty()) {
    if (ImGui::TreeNode("Audios")) {
      for (const std::string &audio : audio_names) {
        if (ImGui::Selectable(audio.c_str())) {
          Mix_PlayChannel(
              -1,
              CSPill::EngineCore::ResourceManager::GetInstance().LoadAudio(
                  audio),
              1);
        }
      }
      ImGui::TreePop();
    }
  }

  ResourceManagerRenderSceneLevels();

  std::vector<std::string> image_names = resource_manager.GetImageNames();
  if (!image_names.empty()) {
    if (ImGui::TreeNode("Images")) {
      for (const std::string &image : image_names) {
        if (ImGui::Selectable(image.c_str())) {
          ImGui::OpenPopup("Preview");
          selected_image_ = image;
        }
      }
      if (ImGui::BeginPopup("Preview", ImGuiWindowFlags_AlwaysAutoResize)) {
        SDL_Point texture_size;
        SDL_QueryTexture(
            ResourceManager::GetInstance().LoadImage(selected_image_), nullptr,
            nullptr, &texture_size.x, &texture_size.y);
        ImGui::Image(ResourceManager::GetInstance().LoadImage(selected_image_),
                     ImVec2(texture_size.x, texture_size.y));
        SDL_QueryTexture(resource_manager.LoadImage(selected_image_), nullptr,
                         nullptr, &texture_size.x, &texture_size.y);
        ImGui::Image(resource_manager.LoadImage(selected_image_),
                     ImVec2(texture_size.x, texture_size.y));
        ImGui::EndPopup();
      }
      ImGui::TreePop();
    }
  }

  ImGui::End();
}

}  // namespace CSPill::Editor
