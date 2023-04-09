//
// Created by Jeromy Zhang on 4/7/23.
//

#ifndef CSPILLENGINE_ENGINE_INCLUDE_EDITOR_EDITORSCENE_H_
#define CSPILLENGINE_ENGINE_INCLUDE_EDITOR_EDITORSCENE_H_

#include <SDL.h>

#include "Scene.h"
#include "UI.h"

namespace CSPill::Editor {

class SceneUI : public EngineCore::UI {
 public:
  SceneUI(std::string title, int width, int height);
  void LoadScene(std::string_view scene_name);
  void Render(SDL_Renderer *renderer) override;

 private:
  std::unique_ptr<EngineCore::Scene> scene_;
  static int SCENE_NUM_ROWS_;
  static int SCENE_NUM_COLS_;
  static int SCENE_BLOCK_SIZE_;
  std::unique_ptr<SDL_Texture *[]> scene_textures_;
};

class ResourcesUI : public EngineCore::UI {
 public:
  ResourcesUI(std::string title, int width, int height);
  void Render(SDL_Renderer *renderer) override;
  static int GetSelectedResourceIndex();
  static SDL_Texture *GetResourceTexture(int index);

 private:
  static int selected_resource_index_;
  static int RESOURCES_NUM_COLS_;
  static std::vector<SDL_Texture *> resource_textures_;
  static std::vector<CSPill::EngineCore::Tileset> resource_tilesets_;
};

class ResourceManagerUI : public EngineCore::UI {
 public:
  ResourceManagerUI(std::string title, int width, int height);
  void Render(SDL_Renderer *renderer) override;
};

}  // namespace CSPill::Editor

#endif  // CSPILLENGINE_ENGINE_INCLUDE_EDITOR_EDITORSCENE_H_
