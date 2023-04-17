//
// Created by Jeromy Zhang on 4/16/23.
//

#include "TransformComponent.h"
#include "Utils.h"

namespace CSPill::EngineCore {

void TransformComponent::Update(GameObject *object, float dt) {
  position_ = position_ + velocity_ * dt;
}
CSPill::Math::Vec2D &TransformComponent::position() { return position_; }
CSPill::Math::Vec2D &TransformComponent::velocity() { return velocity_; }
TransformComponent::TransformComponent() : Component(::EngineCore::Utils::TRANSFORM_COMPONENT) {
  position_ = {0, 0};
  velocity_ = {0, 0};
}

}  // namespace CSPill::EngineCore