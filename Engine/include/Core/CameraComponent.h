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
  explicit CameraComponent();
  ~CameraComponent() override;
  void Update(GameObject *object, float dt) override;
  void Bind(GameObject *object, const Math::Vec2D &view_range = {0, 0});
  const SDL_FRect &GetViewport();
  void SetViewport(float w, float h);
  [[nodiscard]] const Math::Vec2D &GetViewRange() const;
  void SetViewRange(const Math::Vec2D &view_range);
  float GetTransit() const;
  void SetTransit(float transit);

 private:
  SDL_FRect viewport_{};
  Math::Vec2D offset_;
  Math::Vec2D view_range_;
  GameObject *bind_object_{};
  float transit_ = 0.5;
};

}  // namespace CSPill::EngineCore

#endif  // CSPILLENGINE_ENGINE_INCLUDE_CORE_CAMERA_COMPONENT_H_
