//
// Created by Jeromy Zhang on 4/16/23.
//

#include "CameraComponent.h"

#include "TransformComponent.h"
#include "Utils.h"

namespace CSPill::EngineCore {

void CameraComponent::Update(GameObject *object, float dt) {
  auto transform_component = (TransformComponent *)(object->GetComponent(
      ::EngineCore::Utils::TRANSFORM_COMPONENT));
  if (this->bind_object_) {
    if (auto transform_bind_object = this->bind_object_->GetComponent(
            ::EngineCore::Utils::TRANSFORM_COMPONENT)) {
      transform_component->position() =
          dynamic_cast<TransformComponent *>(transform_bind_object)->position();
    }
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
CameraComponent::CameraComponent()
    : Component(::EngineCore::Utils::CAMERA_COMPONENT),
      bind_object_(nullptr),
      offset_({0, 0}) {}
const SDL_FRect &CameraComponent::GetViewport() { return viewport_; }
void CameraComponent::SetViewport(float w, float h) {
  this->viewport_.w = w;
  this->viewport_.h = h;
}

CameraComponent::~CameraComponent() = default;
}  // namespace CSPill::EngineCore
