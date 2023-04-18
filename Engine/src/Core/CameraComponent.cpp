//
// Created by Jeromy Zhang on 4/16/23.
//

#include "CameraComponent.h"

#include "TransformComponent.h"
#include "Utils.h"

namespace CSPill::EngineCore {

void CameraComponent::Update(GameObject *object, float dt) {
  auto transform_component = dynamic_cast<TransformComponent *>(
      object->GetComponent(::EngineCore::Utils::TRANSFORM_COMPONENT));
  Math::Vec2D velocity = {0, 0};
  if (bind_object_) {
    if (auto transform_bind_object = bind_object_->GetComponent(
            ::EngineCore::Utils::TRANSFORM_COMPONENT)) {
      transform_component->position() =
          dynamic_cast<TransformComponent *>(transform_bind_object)
              ->position() +
          offset_;
      velocity = {dynamic_cast<TransformComponent *>(transform_bind_object)
                      ->velocity()
                      .x,
                  dynamic_cast<TransformComponent *>(transform_bind_object)
                      ->velocity()
                      .y};
    }
  }
  float x = viewport_.x, y = viewport_.y;
  if (transform_component->position().x - x > view_range_.x) {
    velocity.x *= 2;
  }
  if (transform_component->position().y - y > view_range_.y) {
    velocity.y *= 2;
  }
  x = std::fmax(
      0.f, std::fmin(x + dt * velocity.x * transit_,
                     transform_component->position().x + view_range_.x / 2));
  y = std::fmax(
      0.f, std::fmin(y + dt * velocity.y * transit_,
                     transform_component->position().y + view_range_.y / 2));
  viewport_ = {x, y, viewport_.w, viewport_.h};
}
void CameraComponent::Bind(GameObject *object, const Math::Vec2D &view_range) {
  bind_object_ = object;
  if (auto transform = dynamic_cast<TransformComponent *>(
          object->GetComponent(::EngineCore::Utils::TRANSFORM_COMPONENT))) {
    offset_ = {-transform->position().x, -transform->position().y};
  }
  view_range_ = view_range;
}
CameraComponent::CameraComponent()
    : Component(::EngineCore::Utils::CAMERA_COMPONENT),
      bind_object_(nullptr),
      offset_({0, 0}) {}
const SDL_FRect &CameraComponent::GetViewport() { return viewport_; }
void CameraComponent::SetViewport(float w, float h) {
  viewport_.w = w;
  viewport_.h = h;
}
const Math::Vec2D &CameraComponent::GetViewRange() const { return view_range_; }
void CameraComponent::SetViewRange(const Math::Vec2D &view_range) {
  view_range_ = view_range;
}
float CameraComponent::GetTransit() const { return transit_; }
void CameraComponent::SetTransit(float transit) { transit_ = transit; }

CameraComponent::~CameraComponent() = default;
}  // namespace CSPill::EngineCore
