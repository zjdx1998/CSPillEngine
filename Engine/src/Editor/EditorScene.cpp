//
// Created by Jeromy Zhang on 4/7/23.
//

#include "EditorScene.h"

#include "imgui.h"
#include <iostream>
#include <SDL_image.h>

namespace CSPill::Editor {

// Scene
int SceneUI::SCENE_NUM_ROWS_ = 10;
int SceneUI::SCENE_NUM_COLS_ = 10;
int SceneUI::SCENE_BLOCK_SIZE_ = 50;

// Resources
int ResourcesUI::selected_resource_index_ = -1;
int ResourcesUI::RESOURCES_NUM_COLS_ = 3;
std::vector<SDL_Texture*> ResourcesUI::resource_textures_;
std::vector<CSPill::EngineCore::Tileset> ResourcesUI::resource_tilesets_;

SceneUI::SceneUI(std::string title, int width, int height)
    : CSPill::EngineCore::UI(title, width, height) {
    // allocate memory for the textures
    scene_textures_ = std::make_unique<SDL_Texture*[]>(SCENE_NUM_ROWS_ * SCENE_NUM_COLS_);

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
  int frame_width = mSurface-> w / 7;
  int frame_height = mSurface-> h / 16;

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
        if (ResourcesUI::GetSelectedResourceIndex() != -1) {
          std::cout << "replace with resource: " << ResourcesUI::GetSelectedResourceIndex()
                    << std::endl;
          SDL_Texture* current_resource = ResourcesUI::GetResourceTexture(ResourcesUI::GetSelectedResourceIndex());
          scene_textures_[row * SCENE_NUM_COLS_ + col] = current_resource;
          ImGui::SetCursorPos(
              ImVec2(col * SCENE_BLOCK_SIZE_ + block_offset.x,
                      row * SCENE_BLOCK_SIZE_ + block_offset.y));
          ImGui::Image(
              (void *)(intptr_t)current_resource,
              ImVec2(SCENE_BLOCK_SIZE_, SCENE_BLOCK_SIZE_));
        }
      }
    }
  }
  ImGui::End();
}
void SceneUI::LoadScene(std::string_view scene_name) {}

ResourcesUI::ResourcesUI(std::string title, int width, int height)
    : CSPill::EngineCore::UI(title, width, height) {}

int ResourcesUI::GetSelectedResourceIndex() {
  return selected_resource_index_;
}

SDL_Texture* ResourcesUI::GetResourceTexture(int index) {
  return resource_textures_.at(index);
}

void ResourcesUI::Render(SDL_Renderer *renderer) {
  ImGui::Begin(this->GetTitle().c_str());

  // Add Tileset button
  if (ImGui::Button("Add Tileset"))
  {
    ImGui::OpenPopup("Add Tileset Popup");
  }

  // Add Tileset popup
  if (ImGui::BeginPopup("Add Tileset Popup"))
  {
    static char file_path[256] = {0};
    static int tile_width = 0;
    static int tile_height = 0;

    ImGui::InputText("File Path", file_path, sizeof(file_path));
    // std::string file_path_str(file_path);
    ImGui::SameLine();
    if (ImGui::Button("Browse")) {
        // TODO: File explorer
    }

    // Tile width input
    ImGui::InputInt("Tile Width", &tile_width);

    // Tile height input
    ImGui::InputInt("Tile Height", &tile_height);

    // Confirm button
    if (ImGui::Button("Confirm"))
    {
      // Load texture
      SDL_Surface* surface = IMG_Load(file_path);
      SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
      SDL_FreeSurface(surface);

      // Get the size of the texture
      int texture_width, texture_height;
      SDL_QueryTexture(texture, NULL, NULL, &texture_width, &texture_height);

      // Add texture to resource textures array
      resource_textures_.push_back(texture);

      // Add tileset to tilesets array
      resource_tilesets_.push_back(CSPill::EngineCore::Tileset(file_path, texture_width, texture_height, tile_width, tile_height));

      ImGui::CloseCurrentPopup();
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
      std::cout << "clicked resource: " << i
                << std::endl;
    }
    // Add a same-line separator to align images horizontally
    if (i % RESOURCES_NUM_COLS_ != 0) {
      ImGui::SameLine();
    }
  }

  ImGui::End();
}



}  // namespace CSPill::Editor
