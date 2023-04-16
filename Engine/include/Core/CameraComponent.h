//
// Created by Jeromy Zhang on 4/16/23.
//

#ifndef CSPILLENGINE_ENGINE_INCLUDE_CORE_CAMERA_COMPONENT_H_
#define CSPILLENGINE_ENGINE_INCLUDE_CORE_CAMERA_COMPONENT_H_

#include "GameObject.h"

namespace CSPill::EngineCore {

class CameraComponent : public Component {
 public:
  void Update(GameObject *object, double dt) override;
  void Bind(GameObject *object, const std::pair<int, int> &offset = {0, 0});

 private:
  SDL_Rect viewport_;
  std::pair<int, int> offset_;
  GameObject *bind_object_;
};

}

#endif //CSPILLENGINE_ENGINE_INCLUDE_CORE_CAMERA_COMPONENT_H_
