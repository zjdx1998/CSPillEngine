//
// Created by Jeromy Zhang on 4/16/23.
//

#ifndef CSPILLENGINE_ENGINE_INCLUDE_CORE_CAMERA_COMPONENT_H_
#define CSPILLENGINE_ENGINE_INCLUDE_CORE_CAMERA_COMPONENT_H_

#include "GameObject.h"
#include "TinyMath.hpp"

namespace CSPill::EngineCore {

class CameraComponent : public Component {
 public:
  explicit CameraComponent(const std::string_view &name = "Camera");
  ~CameraComponent() override;
  void Update(GameObject *object, float dt) override;
  void Bind(GameObject *object, const Math::Vec2D &offset = {0, 0});
  const SDL_FRect &GetViewport();
  void SetViewport(float w, float h);

 private:
  SDL_FRect viewport_{};
  Math::Vec2D offset_;
  GameObject *bind_object_{};
};

}  // namespace CSPill::EngineCore

#endif  // CSPILLENGINE_ENGINE_INCLUDE_CORE_CAMERA_COMPONENT_H_
