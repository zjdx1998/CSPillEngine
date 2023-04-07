//
// Created by Jeromy Zhang on 4/7/23.
//

#ifndef CSPILLENGINE_ENGINE_INCLUDE_EDITOR_EDITORSCENE_H_
#define CSPILLENGINE_ENGINE_INCLUDE_EDITOR_EDITORSCENE_H_

#include "Scene.h"
#include "UI.h"

namespace CSPill::Editor {

class SceneUI : public EngineCore::UI {
 public:
  void LoadScene(std::string_view scene_name);
  void Render() override;

 private:
  std::unique_ptr<EngineCore::Scene> scene_;
};

}  // namespace CSPill::Editor

#endif  // CSPILLENGINE_ENGINE_INCLUDE_EDITOR_EDITORSCENE_H_
