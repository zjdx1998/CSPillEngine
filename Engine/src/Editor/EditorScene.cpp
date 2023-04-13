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

namespace {

constexpr float FILE_BROWSER_WIDTH = 400;
constexpr float FILE_BROWSER_HEIGHT = 300;
static int selected_tileset_row = 0;
static int selected_tileset_col = 0;

using EngineCore::ResourceManager;
using EngineCore::Tileset;

void UICenterRadioButton(std::string_view label, bool active) {
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
                      ImVec2(0, ImGui::GetStyle().ItemSpacing.y));
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
  ImGui::SameLine(ImGui::GetWindowWidth() -
                  ImGui::GetTextLineHeightWithSpacing() -
                  ImGui::GetStyle().ItemInnerSpacing.x);
  ImGui::RadioButton(label.data(), active);
  ImGui::PopStyleVar(2);
}

}  // namespace

SceneUI::SceneUI(std::string title, int width, int height)
    : CSPill::EngineCore::UI(std::move(title), width, height),
      active_scene_texture_(nullptr) {}

SceneUI::~SceneUI() { SDL_DestroyTexture(active_scene_texture_); }

void SceneUI::Render(SDL_Renderer *renderer) {
  ImGui::SetNextWindowSize(ImVec2(static_cast<float>(this->GetWidth()),
                                  static_cast<float>(this->GetHeight())));
  ImGui::Begin(this->GetTitle().c_str());
  auto scene = ResourceManager::GetInstance().ActiveScene();
  auto layer = ResourceManager::GetInstance().ActiveLayer();
  auto tileset = ResourceManager::GetInstance().ActiveTileset();
  if (scene and layer and tileset) {
    // Do not Destroy
    auto texture = scene->Render(renderer, layer, tileset);
    if (texture) {
      SDL_SetRenderTarget(renderer, texture);
      SDL_SetRenderDrawColor(renderer, 128, 128, 128, 128);
      for (int x = 0; x < scene->GetCanvasWidth();
           x += tileset->GetTileWidth()) {
        SDL_RenderDrawLine(renderer, x, 0, x, scene->GetCanvasHeight());
      }

      for (int y = 0; y < scene->GetCanvasHeight();
           y += tileset->GetTileHeight()) {
        SDL_RenderDrawLine(renderer, 0, y, scene->GetCanvasWidth(), y);
      }
      SDL_SetRenderTarget(renderer, nullptr);
      ImGui::Image(texture,
                   ImVec2(scene->GetCanvasWidth(), scene->GetCanvasHeight()));
      if (ImGui::IsItemHovered()) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (ImGui::IsMouseClicked(0)) {
          // get global mouse position
          ImVec2 mouse_pos_global = ImGui::GetMousePos();

          // get window pos and size
          ImVec2 item_pos = ImGui::GetItemRectMin();
          ImVec2 item_size = ImGui::GetItemRectSize();

          ImVec2 mouse_pos =
              ImVec2(fmodf(mouse_pos_global.x - item_pos.x, item_size.x),
                     fmodf(mouse_pos_global.y - item_pos.y, item_size.y));
          int col = mouse_pos.x / tileset->GetTileWidth(),
              row = mouse_pos.y / tileset->GetTileHeight();
          std::pair<int, int> row_and_col(selected_tileset_row,
                                          selected_tileset_col);
          layer
              ->Data()[row * scene->GetCanvasWidth() / tileset->GetTileWidth() +
                       col] =
              ::EngineCore::Utils::GetDataFromRowAndCol(row_and_col);
          //          std::cout << row << " , " << col << " , " <<
          //          ::EngineCore::Utils::GetDataFromRowAndCol(row_and_col)
          //                    << " , data index: " << row *
          //                    scene->GetCanvasWidth() /
          //                    tileset->GetTileWidth() + col
          //                    << std::endl;
        }
      }
    }
  }
  ImGui::End();
}

TileSetEditorUI::TileSetEditorUI(std::string title, int width, int height)
    : CSPill::EngineCore::UI(std::move(title), width, height) {}

void TileSetEditorUI::Render(SDL_Renderer *renderer) {
  ImGui::SetNextWindowSize(ImVec2(this->GetWidth(), this->GetHeight()));
  ImGui::Begin(this->GetTitle().c_str(), nullptr,
               ImGuiWindowFlags_HorizontalScrollbar);

  if (!ResourceManager::GetInstance().GetActiveTilesetName().empty()) {
    if (auto active_layer = ResourceManager::GetInstance().LoadImage(
            ResourceManager::GetInstance().GetActiveTilesetName())) {
      SDL_Point active_layer_size;
      SDL_QueryTexture(active_layer, nullptr, nullptr, &active_layer_size.x,
                       &active_layer_size.y);
      SDL_Texture *lined_texture = SDL_CreateTexture(
          renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
          active_layer_size.x, active_layer_size.y);
      SDL_SetRenderTarget(renderer, lined_texture);
      SDL_RenderCopy(renderer, active_layer, nullptr, nullptr);
      SDL_SetRenderDrawColor(renderer, 128, 128, 128, 128);

      EngineCore::Tileset *tileset =
          ResourceManager::GetInstance().ActiveTileset();
      if (tileset->GetImageWidth() != active_layer_size.x or
          tileset->GetImageHeight() != active_layer_size.y) {
        tileset->SetImageWidth(active_layer_size.x);
        tileset->SetImageHeight(active_layer_size.y);
      }
      for (int x = 0; x < tileset->GetImageWidth();
           x += tileset->GetTileWidth()) {
        SDL_RenderDrawLine(renderer, x, 0, x, tileset->GetImageHeight());
      }

      for (int y = 0; y < tileset->GetImageHeight();
           y += tileset->GetTileHeight()) {
        SDL_RenderDrawLine(renderer, 0, y, tileset->GetImageWidth(), y);
      }
      SDL_SetRenderTarget(renderer, nullptr);
      ImGui::Image(lined_texture,
                   ImVec2(active_layer_size.x, active_layer_size.y));
      if (ImGui::IsItemHovered()) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (ImGui::IsMouseClicked(0)) {
          // get global mouse position
          ImVec2 mouse_pos_global = ImGui::GetMousePos();

          // get window pos and size
          ImVec2 item_pos = ImGui::GetItemRectMin();
          ImVec2 item_size = ImGui::GetItemRectSize();

          ImVec2 mouse_pos =
              ImVec2(fmodf(mouse_pos_global.x - item_pos.x, item_size.x),
                     fmodf(mouse_pos_global.y - item_pos.y, item_size.y));
          selected_tileset_col = mouse_pos.x / tileset->GetTileWidth();
          selected_tileset_row = mouse_pos.y / tileset->GetTileHeight();
          src_rect_ = {selected_tileset_col * tileset->GetTileWidth(),
                       selected_tileset_row * tileset->GetTileHeight(),
                       tileset->GetTileWidth(), tileset->GetTileHeight()};
          ImGui::OpenPopup("Tile");
        }
      }
      if (ImGui::BeginPopup("Tile", ImGuiWindowFlags_AlwaysAutoResize)) {
        SDL_Texture *cropped_texture = nullptr;
        std::pair<int, int> row_and_col =
            std::pair<int, int>(selected_tileset_row, selected_tileset_col);
        auto current_brush =
            std::string(tileset->GetName()) + "-cropped-" +
            std::to_string(
                ::EngineCore::Utils::GetDataFromRowAndCol(row_and_col));
        if (auto query_texture =
                ResourceManager::GetInstance().QueryTexture(current_brush)) {
          cropped_texture = query_texture;
        } else {
          auto cropped = ::EngineCore::Utils::CropTexture(
              renderer, active_layer, src_rect_);
          ResourceManager::GetInstance().AddTile(current_brush,
                                                 std::move(cropped));
          cropped_texture = cropped.get();
        }
        if (cropped_texture) {
          ImGui::Image(cropped_texture, ImVec2(src_rect_.w, src_rect_.h));
        }
        ImGui::EndPopup();
      }
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

        bool is_scene_selected =
            scene_name == resource_manager.GetActiveSceneName();
        UICenterRadioButton("", is_scene_selected);
        if (scene_name == resource_manager.GetActiveSceneName()) {
          if (auto scene = ResourceManager::GetInstance().ActiveScene()) {
            std::unordered_map<std::string, Tileset *> tilesets;
            for (auto &tileset : scene->GetTileSets()) {
              tilesets[tileset.GetName().data()] = &tileset;
            }
            if (ImGui::TreeNode("Layers")) {
              for (const auto &layer : scene->Layers()) {
                if (ImGui::TreeNode(layer.GetName().data())) {
                  bool is_layer_selected =
                      layer.GetTileset() ==
                      resource_manager.GetActiveTilesetName();
                  UICenterRadioButton("", is_layer_selected);
                  if (ImGui::TreeNode(layer.GetTileset().empty()
                                          ? "N/A"
                                          : layer.GetTileset().data())) {
                    if (!layer.GetTileset().empty()) {
                      ImGui::Text("File: %s",
                                  tilesets[layer.GetTileset().data()]
                                      ->GetName()
                                      .data());
                      ImGui::SliderInt(
                          "Width: %d",
                          &tilesets[layer.GetTileset().data()]->TileWidth(), 2,
                          tilesets[layer.GetTileset().data()]->GetImageWidth());
                      ImGui::SliderInt(
                          "Height: %d",
                          &tilesets[layer.GetTileset().data()]->TileHeight(), 2,
                          tilesets[layer.GetTileset().data()]
                              ->GetImageHeight());
                      ImGui::Text(
                          "Width: %d",
                          tilesets[layer.GetTileset().data()]->GetImageWidth());
                      ImGui::Text("Height: %d",
                                  tilesets[layer.GetTileset().data()]
                                      ->GetImageHeight());
                    }
                    ImGui::TreePop();
                  }
                  if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
                    resource_manager.SetActiveTileset(
                        layer.GetTileset().data());
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
  ImGui::SetNextWindowSize(ImVec2(this->GetWidth(), this->GetHeight()));
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
      ImGui::SetNextWindowSize(ImVec2(FILE_BROWSER_WIDTH, FILE_BROWSER_HEIGHT));
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
        ImGui::EndPopup();
      }
      ImGui::TreePop();
    }
  }

  ImGui::End();
}

}  // namespace CSPill::Editor
