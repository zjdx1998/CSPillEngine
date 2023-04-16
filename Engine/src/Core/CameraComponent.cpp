//
// Created by Jeromy Zhang on 4/16/23.
//

#include "CameraComponent.h"

#include "TransformComponent.h"

namespace CSPill::EngineCore {

void CameraComponent::Update(GameObject *object, float dt) {
  Component::Update(object, dt);
  auto transform_component =
      (TransformComponent *)(object->GetComponent("Transform"));
  if (this->bind_object_) {
    transform_component->position() =
        ((TransformComponent *)this->bind_object_->GetComponent("Transform"))
            ->position();
  }
  this->viewport_ = {
      transform_component->position().x + offset_.x - this->viewport_.w / 2,
      transform_component->position().y + offset_.y - this->viewport_.h / 2,
      this->viewport_.w, this->viewport_.h};
}
void CameraComponent::Bind(GameObject *object, const Math::Vec2D &offset) {
  this->bind_object_ = object;
  this->offset_ = offset;
}
CameraComponent::CameraComponent(const std::string_view &name)
    : Component(name), bind_object_(nullptr), offset_({0, 0}) {}
const SDL_FRect &CameraComponent::GetViewport() { return viewport_; }
}  // namespace CSPill::EngineCore
