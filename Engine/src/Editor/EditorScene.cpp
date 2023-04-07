//
// Created by Jeromy Zhang on 4/7/23.
//

#include "EditorScene.h"
#include "imgui.h"

namespace CSPill::Editor {

void SceneUI::Render() {
  ImGui::Begin(this->GetTitle().c_str());

  ImGui::End();
}
void SceneUI::LoadScene(std::string_view scene_name) {

}

}  // namespace CSPill::Editor
