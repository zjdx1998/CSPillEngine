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

/**
 * \brief UI for Scene
 */
class SceneUI : public UI::UI {
 public:
  /**
   * Constructor for SceneUI.
   * @param title a string of the title of the SceneUI.
   * @param width int, the width of the SceneUI
   * @param height int, the height of the SceneUI
   */
  SceneUI(std::string title, int width, int height);

  /**
   * Render the current SceneUI.
   * @param renderer SDL_Renderer that is going to be used
   */
  void Render(SDL_Renderer *renderer) override;

  /**
   * Destructor of SceneUI.
   */
  ~SceneUI();

  /**
   * Check if the SceneUI is previewed.
   * @return true the SceneUI is previewed, false otherwise
   */
  bool IsPreview() const;

  /**
   * Set the current SceneUI should or should not to be previewed.
   * @param preview true the SceneUI is going to be previewed, false otherwise
   */
  void SetPreview(bool preview);

  /**
   * Get active Scene texture.
   * @return SDL_Texture of the active Scene
   */
  SDL_Texture *GetActiveSceneTexture() const;

  /**
   * Set active Scene texture to active_scene_texture.
   * @param active_scene_texture the targeted SDL_Texture of the active Scene
   */
  void SetActiveSceneTexture(SDL_Texture *active_scene_texture);

 private:
  bool preview_ = false;
  SDL_Texture *active_scene_texture_;
};

/**
 * \brief UI for TileSetEditor
 */
class TileSetEditorUI : public UI::UI {
 public:
  /**
   * Constructor of TileSetEditorUI.
   * @param title a string of the title of TileSetEditorUI
   * @param width int, the width of TileSetEditorUI
   * @param height int, the height of TileSetEditorUI
   */
  TileSetEditorUI(std::string title, int width, int height);

  /**
   * Render the current TileSetEditorUI.
   * @param renderer SDL_Renderer that is going to be used
   */
  void Render(SDL_Renderer *renderer) override;

 private:
  SDL_Rect src_rect_;
};

/**
 * \brief UI for ResourceManager
 */
class ResourceManagerUI : public UI::UI {
 public:
  /**
   * Constructor of ResourceManagerUI.
   * @param title a string of the title of ResourceManagerUI
   * @param width int, the width of ResourceManagerUI
   * @param height int, the height of ResourceManagerUI
   */
  ResourceManagerUI(std::string title, int width, int height);

  /**
   * Render the current ResourceManagerUI.
   * @param renderer SDL_Renderer that is going to be used
   */
  void Render(SDL_Renderer *renderer) override;

 private:
  void ResourceManagerRenderSceneLevels();

  std::string selected_image_;
};

}  // namespace CSPill::Editor

#endif  // CSPILLENGINE_ENGINE_INCLUDE_EDITOR_EDITORSCENE_H_
