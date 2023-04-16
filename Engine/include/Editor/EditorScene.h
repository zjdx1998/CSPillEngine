//
// Created by Jeromy Zhang on 4/7/23.
//

#ifndef CSPILLENGINE_ENGINE_INCLUDE_EDITOR_EDITORSCENE_H_
#define CSPILLENGINE_ENGINE_INCLUDE_EDITOR_EDITORSCENE_H_

#include <SDL.h>

#include "Scene.h"
#include "UI.h"

namespace CSPill::Editor {

using EngineCore::Scene;

class SceneUI : public EngineCore::UI {
 public:
  SceneUI(std::string title, int width, int height);
  void Render(SDL_Renderer *renderer) override;
  ~SceneUI();
  bool IsPreview() const;
  void SetPreview(bool preview);
  SDL_Texture *GetActiveSceneTexture() const;
  void SetActiveSceneTexture(SDL_Texture *active_scene_texture);

 private:
  bool preview_ = false;
  SDL_Texture *active_scene_texture_;
};

class TileSetEditorUI : public EngineCore::UI {
 public:
  TileSetEditorUI(std::string title, int width, int height);
  void Render(SDL_Renderer *renderer) override;

 private:
  SDL_Rect src_rect_;
};

class ResourceManagerUI : public EngineCore::UI {
 public:
  ResourceManagerUI(std::string title, int width, int height);
  void Render(SDL_Renderer *renderer) override;

 private:
  void ResourceManagerRenderSceneLevels();
  std::string selected_image_;
};

}  // namespace CSPill::Editor

#endif  // CSPILLENGINE_ENGINE_INCLUDE_EDITOR_EDITORSCENE_H_
